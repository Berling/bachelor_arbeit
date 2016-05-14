#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <stdexcept>

#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/rendering/rendering_system.hpp>
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

			auto terrain_length = 100.f;
			auto subdivisions = 5;
			for (auto x = 0; x < subdivisions; ++x) {
				for (auto y = 0; y < subdivisions; ++y) {
					auto cell_length = terrain_length / subdivisions;
					auto x_offset = x * cell_length;
					auto y_offset = y * cell_length;
					auto path = "terrain/" + std::to_string(x) + "_" + std::to_string(y) + ".vol";
					utils::log << path << std::endl;
					terrain_.emplace_back(std::make_unique<terrain_cell>(engine_, *this, glm::vec3{x_offset, 0.f, y_offset}, glm::angleAxis(0.f, glm::vec3{0.f}), path));
				}
			}
		}

		void terrain_system::update(float delta_time) {
			auto& rendering_system = engine_.rendering_system();
			for (auto& tc : terrain_) {
				if (!tc->is_empty()) {
					rendering_system.draw(tc.get());
				}
				rendering_system.draw_bounding_box(glm::vec3{tc->volume_data().grid_length() / 2.f}, tc->position() + glm::vec3{tc->volume_data().grid_length() / 2.f}, glm::angleAxis(0.f, glm::vec3{0.f}));
			}
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
