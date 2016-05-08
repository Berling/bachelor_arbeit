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
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>
#include <vbte/terrain/volume_data_manager.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace terrain {
		terrain_cell::terrain_cell(core::engine& engine, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name) :
		rendering::drawable{engine, position, rotation}, vbo_{GL_DYNAMIC_DRAW}, vertex_count_{0} {
			volume_data_ = engine_.terrain_system().volume_data_manager().load(file_name);
			if (!volume_data_) {
				throw std::runtime_error{"could not load file " + file_name};
			}

			auto& compute_context = engine_.terrain_system().compute_context();
			volume_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_ONLY, volume_data_->grid().size() * sizeof(float));
			auto estimated_vertex_count = estimate_vertex_count(*volume_data_, volume_data_->resolution());
			vertex_buffer_ = std::make_unique<compute::shared_buffer>(compute_context, CL_MEM_WRITE_ONLY, estimated_vertex_count * sizeof(rendering::basic_vertex), nullptr, GL_DYNAMIC_DRAW);
			vertex_count_buffer_ = std::make_unique<compute::buffer>(compute_context, CL_MEM_READ_WRITE, sizeof(int));

			this->marching_cubes(*volume_data_, volume_data_->resolution());

			engine_.rendering_system().basic_layout().setup_layout(vao_, vertex_buffer_.get());
		}

		void terrain_cell::draw() const {
			vao_.bind();
			glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
		}

		void terrain_cell::update_geometry() {
			this->marching_cubes(*volume_data_, volume_data_->resolution());
		}

		void terrain_cell::marching_cubes(const terrain::volume_data& grid, size_t resolution) {
			auto& compute_context = engine_.terrain_system().compute_context();

			auto estimated_vertex_count = 744;//estimate_vertex_count(grid, resolution);
			auto vertex_count = 0;

			compute_context.begin();
			compute_context.enqueue_acquire_gl_buffer(*vertex_buffer_);

			compute_context.enqueue_write_buffer(*volume_buffer_, false, grid.grid().size() * sizeof(float), grid.grid().data());
			compute_context.enqueue_write_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count);

			auto& kernel = engine_.terrain_system().marching_cubes_kernel();

			kernel.arg(0, *volume_buffer_);
			kernel.arg(1, static_cast<uint32_t>(grid.resolution()));
			kernel.arg(2, static_cast<uint32_t>(resolution));
			kernel.arg(3, grid.grid_length());
			kernel.arg(4, *vertex_buffer_);
			kernel.arg(5, *vertex_count_buffer_);
			auto event = compute_context.enqueue_kernel(kernel, cl::NDRange{resolution, resolution, resolution}, cl::NDRange{4, 4, 4});
			compute_context.enqueue_read_buffer(*vertex_count_buffer_, false, sizeof(int), &vertex_count);
			event.wait();

			compute_context.end();
			compute_context.enqueue_release_gl_buffer(*vertex_buffer_);

			vertex_count_ = vertex_count;
		}
	}
}
