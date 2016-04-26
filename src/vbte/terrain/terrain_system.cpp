#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <stdexcept>

#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace terrain {
		terrain_system::terrain_system(core::engine& engine)
		: engine_{engine}, volume_data_manager_{engine} {
			init_transform_feedback_layout();
			init_volume_data_layout();
			init_opencl();

			auto& shader_manager = engine_.graphics_system().shader_manager();
			init_marching_cubes_program(shader_manager);
		}

		void terrain_system::update(float delta_time) {

		}

		void terrain_system::init_marching_cubes_program(graphics::shader_manager& shader_manager) {
			auto vertex_shader = shader_manager.load("shaders/marching_cubes.vert", GL_VERTEX_SHADER);
			if (!vertex_shader) {
				throw std::runtime_error{"could not load shader shaders/marching_cubes.vert"};
			}
			marching_cubes_program_.attach_shader(vertex_shader);
			auto geometry_shader = shader_manager.load("shaders/marching_cubes.geom", GL_GEOMETRY_SHADER);
			if (!geometry_shader) {
				throw std::runtime_error{"could not load shader shaders/marching_cubes.geom"};
			}
			marching_cubes_program_.attach_shader(geometry_shader);
			volume_data_layout_.setup_program(marching_cubes_program_, "none");
			transform_feedback_layout_.setup_program(marching_cubes_program_, "none");
			marching_cubes_program_.link();
		}

		void terrain_system::init_opencl() {
			try {
				std::vector<cl::Platform> platforms;
				cl::Platform::get(&platforms);
				auto& default_platform = platforms.at(0);
				std::string platform_vendor;
				default_platform.getInfo(static_cast<cl_platform_info>(CL_PLATFORM_VENDOR), &platform_vendor);

				cl_context_properties properties[3] = {
					CL_CONTEXT_PLATFORM,
					(cl_context_properties)(default_platform)(),
					0
				};
				default_context_ = cl::Context{CL_DEVICE_TYPE_GPU, properties};

				const auto& devices = default_context_.getInfo<CL_CONTEXT_DEVICES>();
				default_device_ = devices.at(0);
				auto device_name = default_device_.getInfo<CL_DEVICE_NAME>();
				auto device_version = default_device_.getInfo<CL_DEVICE_VERSION>();
				utils::log << device_name << " " << device_version << std::endl;

				default_queue_ = cl::CommandQueue{default_context_, default_device_};
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
			}
		}

		void terrain_system::init_transform_feedback_layout() {
			transform_feedback_layout_.emplace_back("output_vertex.position");
		}

		void terrain_system::init_volume_data_layout() {
			auto size = 4 * sizeof(glm::vec4);
			volume_data_layout_.emplace_back("_c00", 4, GL_FLOAT, false, size, 0);
			volume_data_layout_.emplace_back("_c01", 4, GL_FLOAT, false, size, 16);
			volume_data_layout_.emplace_back("_c10", 4, GL_FLOAT, false, size, 32);
			volume_data_layout_.emplace_back("_c11", 4, GL_FLOAT, false, size, 48);
		}
	}
}
