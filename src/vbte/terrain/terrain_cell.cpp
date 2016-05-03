#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <chrono>
#include <iostream>
#include <stdexcept>

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
		rendering::drawable{engine, position, rotation}, vbo_{GL_DYNAMIC_DRAW} {
			volume_data_ = engine_.terrain_system().volume_data_manager().load(file_name);
			if (!volume_data_) {
				throw std::runtime_error{"could not load file " + file_name};
			}

			const auto& vertices = this->marching_cubes(*volume_data_, volume_data_->resolution());
			index_count_ = vertices.size();
			vbo_.data(sizeof(rendering::basic_vertex) * index_count_, vertices.data());

			engine_.rendering_system().basic_layout().setup_layout(vao_, &vbo_);
		}

		void terrain_cell::draw() const {
			vao_.bind();
			glDrawArrays(GL_TRIANGLES, 0, index_count_);
		}

		std::vector<rendering::basic_vertex> terrain_cell::marching_cubes(const terrain::volume_data& grid, size_t resolution) {
			try {
				auto& terrain_system = engine_.terrain_system();
				auto& default_context = terrain_system.default_context();
				auto& default_device = terrain_system.default_device();

				auto source_file = engine_.asset_manager().load("opencl/marching_cubes_kernel.cl");
				if (!source_file) {
					throw std::runtime_error{"could not load kernel from opencl/marching_cubes_kernel.cl"};
				}
				auto source_code = std::string{source_file->content().begin(), source_file->content().end()};

				auto source = cl::Program::Sources{1, std::make_pair(source_code.c_str(), source_code.length() + 1)};
				auto program = cl::Program{default_context, source};
				auto error = program.build({default_device});
				if (error != CL_SUCCESS) {
					utils::log << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(default_device) << std::endl;
					utils::log << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(default_device) << std::endl;
					utils::log << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
				}

				auto kernel = cl::Kernel{program, "marching_cubes"};

				auto volume = cl::Buffer{default_context, CL_MEM_READ_ONLY, grid.grid().size() * sizeof(float)};
				auto estimated_vertex_count = estimate_vertex_count(grid, resolution);
				auto vertex_buffer = cl::Buffer{default_context, CL_MEM_WRITE_ONLY, estimated_vertex_count * sizeof(rendering::basic_vertex)};
				auto vertex_count = 0;
				auto vertex_counter = cl::Buffer{default_context, CL_MEM_READ_WRITE, sizeof(int)};

				auto& default_command_queue = terrain_system.default_command_queue();
				default_command_queue.enqueueWriteBuffer(volume, CL_TRUE, 0, grid.grid().size() * sizeof(float), grid.grid().data());
				default_command_queue.enqueueWriteBuffer(vertex_counter, CL_TRUE, 0, sizeof(int), &vertex_count);

				kernel.setArg(0, volume);
				kernel.setArg(1, static_cast<uint32_t>(grid.resolution()));
				kernel.setArg(2, static_cast<uint32_t>(resolution));
				kernel.setArg(3, grid.grid_length());
				kernel.setArg(4, vertex_buffer);
				kernel.setArg(5, vertex_counter);
				cl::Event event;
				default_command_queue.enqueueNDRangeKernel(
					kernel,
					cl::NullRange,
					cl::NDRange{resolution, resolution, resolution},
					cl::NDRange{4, 4, 4},
					nullptr,
					&event
				);
				std::vector<rendering::basic_vertex> vertices;
				vertices.assign(estimated_vertex_count, rendering::basic_vertex{glm::vec4{0.f}, glm::vec4{0.f}});
				default_command_queue.enqueueReadBuffer(vertex_buffer, CL_TRUE, 0, estimated_vertex_count * sizeof(rendering::basic_vertex), vertices.data());
				default_command_queue.enqueueReadBuffer(vertex_counter, CL_TRUE, 0, sizeof(int), &vertex_count);
				event.wait();

				vertices.resize(vertex_count);

				return vertices;
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
			}
			return std::vector<rendering::basic_vertex>{};
		}
	}
}
