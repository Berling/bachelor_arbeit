#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <chrono>
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>

#include <glm/gtx/string_cast.hpp>

#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_manager.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/transform_feedback_buffer.hpp>
#include <vbte/graphics/vertex_layout.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/terrain/marching_cubes.hpp>
#include <vbte/terrain/terrain.hpp>
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>
#include <vbte/terrain/volume_data_manager.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace terrain {
		terrain_cell::terrain_cell(core::engine& engine,
		                           terrain_system& terrain_system,
		                           terrain& owner,
		                           const glm::ivec3& index,
		                           const glm::vec3& position,
		                           const glm::quat& rotation,
		                           const std::string& file_name) : rendering::drawable{engine, position, rotation},
		                                                           terrain_system_{terrain_system},
		                                                           owner_{owner},
		                                                           vbo_{GL_DYNAMIC_DRAW},
		                                                           vbo2_{GL_DYNAMIC_DRAW},
		                                                           vertex_count_{0},
		                                                           empty_{false},
		                                                           dirty_{false} {
			volume_data_ = terrain_system_.volume_data_manager().load(file_name);
			if (!volume_data_) {
				throw std::runtime_error{"could not load file " + file_name};
			}

			maximum_vertex_count_ = estimate_vertex_count(*volume_data_, volume_data_->resolution());
			if (maximum_vertex_count_ == 0) {
				empty_ = true;
			}

			if (!empty_) {
				auto cells_per_dimension = owner_.cells_per_dimension();
				adjacent_cells_[0] = index.x == 0 ? -1 : (index.z + cells_per_dimension * (index.y + 2 * (index.x - 1)));
				adjacent_cells_[1] = index.x == (cells_per_dimension - 1) ? -1 : (index.z + cells_per_dimension * (index.y + 2 * (index.x + 1)));
				adjacent_cells_[2] = index.y == 0 ? -1 : (index.z + cells_per_dimension * ((index.y - 1) + 2 * index.x));
				adjacent_cells_[3] = index.y == 1 ? -1 : (index.z + cells_per_dimension * ((index.y + 1) + 2 * index.x));
				adjacent_cells_[4] = index.z == 0 ? -1 : ((index.z - 1) + cells_per_dimension * (index.y + 2 * index.x));
				adjacent_cells_[5] = index.z == (cells_per_dimension - 1) ? -1 : ((index.z + 1) + cells_per_dimension * (index.y + 2 * index.x));

				auto adjacent_cell_count = 0;
				for (auto index : adjacent_cells_) {
					if (index != -1) {
						++adjacent_cell_count;
					}
				}

				auto& compute_context = terrain_system_.compute_context();
				volume_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_ONLY, volume_data_->grid().size() * sizeof(float));
				vertex_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_WRITE_ONLY, maximum_vertex_count_ * sizeof(rendering::basic_vertex));
				vertex_count_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_WRITE, sizeof(int));
				adjacent_cells_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_ONLY, 6 * sizeof(int));

				vertices_.resize(maximum_vertex_count_);

				engine_.rendering_system().basic_layout().setup_layout(vao_, &vbo_);
				engine_.rendering_system().basic_layout().setup_layout(vao2_, &vbo2_);
			}
		}

		void terrain_cell::draw() const {
			if (front_) {
				vao_.bind();
				glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
			} else {
				vao2_.bind();
				glDrawArrays(GL_TRIANGLES, 0, vertex_count2_);
			}
		}

		void terrain_cell::update_geometry(size_t resolution) {
			if (build_) {
				this->marching_cubes(*volume_data_, resolution);
				build_ = false;
			}
			if (!dirty_.load() && write_data_) {
				if (initial_build_ || !front_) {
					vbo_.data(vertex_count_ * sizeof(rendering::basic_vertex), vertices_.data());
					front_ = true;
					initial_build_ = false;
				} else {
					vbo2_.data(vertex_count2_ * sizeof(rendering::basic_vertex), vertices_.data());
					front_ = false;
				}
				write_data_ = false;
			}
		}

		void terrain_cell::marching_cubes(const class volume_data& grid, size_t resolution) {
			if (!is_empty() && !dirty_.load()) {
				dirty_.store(true);

				auto& compute_context = terrain_system_.compute_context();

				compute_context.enqueue_write_buffer(*volume_buffer_, false, grid.grid().size() * sizeof(float), grid.grid().data());
				compute_context.enqueue_write_buffer(*adjacent_cells_buffer_, false, 6 * sizeof(int), adjacent_cells_.data());
				if (initial_build_ || !front_) {
					vertex_count_ = 0;
					compute_context.enqueue_write_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count_);
				} else {
					vertex_count2_ = 0;
					compute_context.enqueue_write_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count2_);
				}

				auto& kernel = terrain_system_.marching_cubes_kernel();

				kernel.arg(0, *volume_buffer_);
				kernel.arg(1, static_cast<uint32_t>(grid.resolution()));
				kernel.arg(2, static_cast<uint32_t>(resolution));
				kernel.arg(3, grid.grid_length());
				kernel.arg(4, *vertex_buffer_);
				kernel.arg(5, *vertex_count_buffer_);
				kernel.arg(6, *adjacent_cells_buffer_);

				auto& cells = owner_.cells();
				auto argument_index = 7;
				for (auto index : adjacent_cells_) {
					auto& adjacent_cell = *cells[index];
					if (!adjacent_cell.is_empty() && index != - 1) {
						auto& data = adjacent_cell.volume_data();
						compute_context.enqueue_write_buffer(adjacent_cell.volume_buffer(), false, data.grid().size() * sizeof(float), data.grid().data());
						kernel.arg(argument_index, adjacent_cell.volume_buffer());
					} else {
						kernel.arg(argument_index, nullptr);
					}
					++argument_index;
				}

				auto event = compute_context.enqueue_kernel(kernel, cl::NDRange{resolution, resolution, resolution}, cl::NDRange{4, 4, 4});
				compute_context.enqueue_read_buffer(*vertex_buffer_, false, maximum_vertex_count_ * sizeof(rendering::basic_vertex), vertices_.data());

				if (initial_build_ || !front_) {
					compute_context.enqueue_read_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count_);
				} else {
					compute_context.enqueue_read_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count2_);
				}

				auto result = event.setCallback(
					CL_COMPLETE,
					+[](cl_event event, cl_int command_exec_status, void* user_data) {
						reinterpret_cast<class terrain_cell*>(user_data)->dirty_.store(false);
					},
					this
				);
				write_data_ = true;
			}
		}
	}
}
