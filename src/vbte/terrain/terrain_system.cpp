#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <stdexcept>

#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/terrain/terrain.hpp>
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace terrain {
		terrain_system::terrain_system(core::engine& engine) : engine_{engine},
																													 volume_data_manager_{engine},
																													 context_{false},
																													 kernel_manager_{engine_, *this} {
			marching_cubes_kernel_ = kernel_manager_.load("opencl/marching_cubes_kernel.cl", "marching_cubes");

			init_transform_feedback_layout();
			init_volume_data_layout();

			auto& shader_manager = engine_.graphics_system().shader_manager();
			init_marching_cubes_program(shader_manager);

			terrain_ = std::make_unique<class terrain>(engine_, *this, "terrain/test.ter");
		}

		void terrain_system::update(float delta_time) {
			auto& camera = engine_.camera();
			terrain_->update_lod_levels(camera.position());
			terrain_->draw();
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
