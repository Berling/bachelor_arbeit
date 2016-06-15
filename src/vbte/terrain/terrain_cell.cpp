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
#include <vbte/core/camera.hpp>
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
		                                                           dirty_{false},
		                                                           loaded_{false},
		                                                           file_name_{file_name} {
			auto cells_per_dimension = owner_.cells_per_dimension();
			adjacent_cells_[0].index = index.x == 0 ? -1 : (index.z + cells_per_dimension * (index.y + 2 * (index.x - 1)));
			adjacent_cells_[1].index = index.x == (cells_per_dimension - 1) ? -1 : (index.z + cells_per_dimension * (index.y + 2 * (index.x + 1)));
			adjacent_cells_[2].index = index.y == 0 ? -1 : (index.z + cells_per_dimension * ((index.y - 1) + 2 * index.x));
			adjacent_cells_[3].index = index.y == 1 ? -1 : (index.z + cells_per_dimension * ((index.y + 1) + 2 * index.x));
			adjacent_cells_[4].index = index.z == 0 ? -1 : ((index.z - 1) + cells_per_dimension * (index.y + 2 * index.x));
			adjacent_cells_[5].index = index.z == (cells_per_dimension - 1) ? -1 : ((index.z + 1) + cells_per_dimension * (index.y + 2 * index.x));

			lod_cache_.emplace_back(std::make_unique<lod_cache_element>());
			lod_cache_.emplace_back(std::make_unique<lod_cache_element>());
			lod_cache_.emplace_back(std::make_unique<lod_cache_element>());
			front_lod_cache_element_ = lod_cache_[0].get();

			lod_cache_vertices_.emplace_back();
			lod_cache_vertices_.emplace_back();
			lod_cache_vertices_.emplace_back();
		}

		void terrain_cell::draw() const {
			if (is_initialized()) {
				if (!is_empty() && front_ && !initial_build_) {
					if (front_lod_cache_element_->builded) {
						front_lod_cache_element_->vao.bind();
						glDrawArrays(GL_TRIANGLES, 0, front_lod_cache_element_->vertex_count);
					}

					vao_.bind();
					glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
				} else if (!is_empty() && !front_) {
					if (back_lod_cache_element_->builded) {
						back_lod_cache_element_->vao.bind();
						glDrawArrays(GL_TRIANGLES, 0, back_lod_cache_element_->vertex_count);
					}

					vao2_.bind();
					glDrawArrays(GL_TRIANGLES, 0, vertex_count2_);
				}
			}
		}

		void terrain_cell::draw_normals() const {
			if (is_initialized()) {
				static auto& normal_program = terrain_system_.normal_program();
				static auto& camera = engine_.camera();
				normal_program.use();
				normal_program.uniform("model", false, transform());
				normal_program.uniform("view", false, camera.view());
				normal_program.uniform("projection", false, camera.projection());
				normal_program.uniform("normal_length", 0.3f);
				normal_program.uniform("color", glm::vec4{0.f, 0.f, 1.f, 1.f});

				if (front_) {
					if (front_lod_cache_element_->builded) {
						front_lod_cache_element_->vao.bind();
						glDrawArrays(GL_TRIANGLES, 0, front_lod_cache_element_->vertex_count);
					}

					vao_.bind();
					glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
				} else {
					if (back_lod_cache_element_->builded) {
						back_lod_cache_element_->vao.bind();
						glDrawArrays(GL_TRIANGLES, 0, back_lod_cache_element_->vertex_count);
					}

					vao2_.bind();
					glDrawArrays(GL_TRIANGLES, 0, vertex_count2_);
				}
			}
		}

		void terrain_cell::update_geometry(size_t resolution) {
			if (build_) {
				this->calculate_cell_borders(*volume_data_, resolution);
				build_ = false;
			}
			if (!dirty_.load() && write_data_) {
				if (initial_build_ || !front_) {
					vbo_.data(vertex_count_ * sizeof(rendering::basic_vertex), vertices_.data());
					front_ = true;
					initial_build_ = false;
					front_lod_cache_element_ = lod_cache_[current_lod_level_].get();
				} else {
					vbo2_.data(vertex_count2_ * sizeof(rendering::basic_vertex), vertices2_.data());
					front_ = false;
					back_lod_cache_element_ = lod_cache_[current_lod_level_].get();
				}
				write_data_ = false;
			}
		}

		cl::Event terrain_cell::marching_cubes(const class volume_data& grid, size_t resolution, std::vector<rendering::basic_vertex>& vertices, int& vertex_count) {
				auto& compute_context = terrain_system_.compute_context();
				vertex_count = 0;

				compute_context.enqueue_write_buffer(*volume_buffer_, false, grid.grid().size() * sizeof(float), grid.grid().data());
				compute_context.enqueue_write_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count);

				auto& kernel = terrain_system_.marching_cubes_kernel();

				kernel.arg(0, *volume_buffer_);
				kernel.arg(1, static_cast<uint32_t>(grid.resolution()));
				kernel.arg(2, static_cast<uint32_t>(resolution));
				kernel.arg(3, grid.grid_length());
				kernel.arg(4, *vertex_buffer_);
				kernel.arg(5, *vertex_count_buffer_);
				kernel.arg(6, nullptr);
				kernel.arg(7, nullptr);
				kernel.arg(8, nullptr);
				kernel.arg(9, nullptr);
				kernel.arg(10, nullptr);
				kernel.arg(11, nullptr);
				kernel.arg(12, nullptr);


				compute_context.enqueue_kernel(kernel, cl::NDRange{resolution - 2, resolution - 2, resolution - 2}, cl::NDRange{2, 2, 2}, cl::NDRange{1, 1, 1});
				compute_context.enqueue_read_buffer(*vertex_buffer_, false, maximum_vertex_count_ * sizeof(rendering::basic_vertex), vertices.data());
				auto event = compute_context.enqueue_read_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count);

				return event;
		}

		void terrain_cell::calculate_cell_borders(const class volume_data& grid, size_t resolution) {
			if (!is_empty() && !dirty_.load()) {
				dirty_.store(true);

				auto& compute_context = terrain_system_.compute_context();

				compute_context.enqueue_write_buffer(*volume_buffer_, false, grid.grid().size() * sizeof(float), grid.grid().data());
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

				auto& cells = owner_.cells();
				auto argument_index = 7;
				for (auto& adjacent_cell : adjacent_cells_) {
					if (adjacent_cell.index != - 1 && !cells[adjacent_cell.index]->is_empty()) {
						auto& cell = *cells[adjacent_cell.index];
						auto& data = cell.volume_data();
						compute_context.enqueue_write_buffer(cell.volume_buffer(), false, data.grid().size() * sizeof(float), data.grid().data());
						kernel.arg(argument_index, cell.volume_buffer());
					} else {
						kernel.arg(argument_index, nullptr);
					}
					++argument_index;
				}

				compute_context.enqueue_write_buffer(*adjacent_cells_buffer_, false, 6 * sizeof(adjacent_cell), adjacent_cells_.data());
				kernel.arg(6, *adjacent_cells_buffer_);
				compute_context.enqueue_kernel(kernel, cl::NDRange{1, resolution, resolution}, cl::NDRange{1, 4, 4});
				compute_context.enqueue_kernel(kernel, cl::NDRange{1, resolution, resolution}, cl::NDRange{1, 4, 4}, cl::NDRange{resolution - 1, 0, 0});
				compute_context.enqueue_kernel(kernel, cl::NDRange{resolution - 2, 1, resolution}, cl::NDRange{2, 1, 2}, cl::NDRange{1, 0, 0});
				compute_context.enqueue_kernel(kernel, cl::NDRange{resolution - 2, 1, resolution}, cl::NDRange{2, 1, 2}, cl::NDRange{1, resolution - 1, 0});
				compute_context.enqueue_kernel(kernel, cl::NDRange{resolution - 2, resolution - 2, 1}, cl::NDRange{2, 2, 1}, cl::NDRange{1, 1, 0});
				compute_context.enqueue_kernel(kernel, cl::NDRange{resolution - 2, resolution - 2, 1}, cl::NDRange{2, 2, 1}, cl::NDRange{1, 1, resolution - 1});

				cl::Event event;
				if (initial_build_ || !front_) {
					compute_context.enqueue_read_buffer(*vertex_buffer_, false, maximum_vertex_count_ * sizeof(rendering::basic_vertex), vertices_.data());
					event = compute_context.enqueue_read_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count_);
				} else {
					compute_context.enqueue_read_buffer(*vertex_buffer_, false, maximum_vertex_count_ * sizeof(rendering::basic_vertex), vertices2_.data());
					event = compute_context.enqueue_read_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count2_);
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

		void terrain_cell::update_adjacent_cells_info() noexcept {
			auto& cells = owner_.cells();
			for (auto& adjacent_cell : adjacent_cells_) {
				if (adjacent_cell.index != -1) {
					auto& cell = *cells[adjacent_cell.index];
					if (!cell.is_empty()) {
						auto& data = cell.volume_data();
						adjacent_cell.resolution = data.resolution() >> cell.lod_level();

						if (data.resolution() >> lod_level() < adjacent_cell.resolution) {
							if (!adjacent_cell.higher_resolution) {
								build_ = true;
							}
							adjacent_cell.higher_resolution = true;
						} else {
							if (adjacent_cell.higher_resolution) {
								build_ = true;
							}
							adjacent_cell.higher_resolution = false;
						}
					}
				}
			}
		}

		void terrain_cell::load() {
			volume_data_ = terrain_system_.volume_data_manager().load(file_name_);
			if (!volume_data_) {
				throw std::runtime_error{"could not load file " + file_name_};
			}

			maximum_vertex_count_ = estimate_vertex_count(*volume_data_, volume_data_->resolution());
			vertices_.resize(maximum_vertex_count_);
			vertices2_.resize(maximum_vertex_count_);
			for (auto& vertices : lod_cache_vertices_) {
				vertices.resize(maximum_vertex_count_);
			}

			if (maximum_vertex_count_ == 0) {
				empty_ = true;
			}
			loaded_.store(true);
		}

		void terrain_cell::initialize() {
			if (is_loaded() && !initialized_) {
				if (!empty_) {
					auto& compute_context = terrain_system_.compute_context();
					volume_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_ONLY, volume_data_->grid().size() * sizeof(float));
					vertex_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_WRITE_ONLY, maximum_vertex_count_ * sizeof(rendering::basic_vertex));
					vertex_count_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_WRITE, sizeof(int));
					adjacent_cells_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_ONLY, 6 * sizeof(adjacent_cell));

					engine_.rendering_system().basic_layout().setup_layout(vao_, &vbo_);
					engine_.rendering_system().basic_layout().setup_layout(vao2_, &vbo2_);
				}
				initialized_ = true;
			} else if (is_loaded() && is_initialized()) {
				initialize_lod_cache();
			}
		}

		void terrain_cell::initialize_lod_cache() {
				for (int i = 0; i < lod_cache_.size(); ++i) {
					auto& chache_element = *lod_cache_[i];

					if (build_lod_cache_) {
						engine_.rendering_system().basic_layout().setup_layout(chache_element.vao, &chache_element.vbo);

						auto event = marching_cubes(*volume_data_, volume_data_->resolution() >> i, lod_cache_vertices_[i], chache_element.vertex_count);

						event.setCallback(
							CL_COMPLETE,
							+[](cl_event event, cl_int command_exec_status, void* user_data) {
								reinterpret_cast<lod_cache_element*>(user_data)->write.store(true);
							},
							&chache_element
						);
					} else if (chache_element.write.load()) {
						chache_element.write.store(false);
						chache_element.builded = true;

						chache_element.vbo.data(chache_element.vertex_count * sizeof(rendering::basic_vertex), lod_cache_vertices_[i].data());
						lod_cache_vertices_[i].clear();
					}
				}
				build_lod_cache_ = false;
		}
	}
}
