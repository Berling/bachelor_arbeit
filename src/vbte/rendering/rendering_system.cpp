#include <glm/gtc/matrix_transform.hpp>

#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/rendering/basic_vertex.hpp>
#include <vbte/rendering/cube.hpp>
#include <vbte/rendering/rendering_system.hpp>

namespace vbte {
	namespace rendering {
		rendering_system::rendering_system(core::engine& engine)
		: engine_{engine}, debug_face_color_{0.f, 0.59f, 0.93f}, debug_edge_color_{0.09f, 0.19f, 0.49f} {
			basic_layout_.emplace_back("_position", 3, GL_FLOAT, false, sizeof(basic_vertex), offsetof(basic_vertex, position));
			
			auto& shader_manager = engine_.graphics_system().shader_manager();

			auto vertex_shader = shader_manager.load("shaders/debug.vert", GL_VERTEX_SHADER);
			if (!vertex_shader) {
				throw std::runtime_error{"could not load shaders/debug.vert"};
			}
			debug_program_.attach_shader(vertex_shader);
			auto fragment_shader = shader_manager.load("shaders/debug.frag", GL_FRAGMENT_SHADER);
			if (!fragment_shader) {
				throw std::runtime_error{"could not load shaders/debug.frag"};
			}
			debug_program_.attach_shader(fragment_shader);
			basic_layout_.setup_program(debug_program_, "frag_color");
			debug_program_.link();
		}

		rendering_system::~rendering_system() noexcept {

		}

		void rendering_system::update(float delta_time) {
			static cube c{engine_, glm::vec3{1.f}};
			
			glEnable(GL_DEPTH_TEST);

			debug_program_.use();
			debug_program_.uniform("projection", false, glm::perspective(glm::radians(45.f), 4.f / 3.f, 0.1f, 1000.f));
			debug_program_.uniform("view", false, glm::lookAt(glm::vec3{2.f, 3.f, 5.f}, glm::vec3{0.f}, glm::vec3{0.f, 1.f, 0.f}));
			debug_program_.uniform("model", false, glm::mat4{1.f});
			debug_program_.uniform("color", debug_edge_color_);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(8.f);
			c.draw();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.f);
			c.draw();

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.f, 1.f);
			glCullFace(GL_FRONT);

			auto clear_color = engine_.graphics_system().clear_color();
			debug_program_.uniform("color", debug_face_color_);//glm::vec3{clear_color});
			c.draw();

			glDisable(GL_POLYGON_OFFSET_FILL);

			glDisable(GL_DEPTH_TEST);
		}
	}
}