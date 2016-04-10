#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/rendering/basic_vertex.hpp>
#include <vbte/rendering/bounding_box.hpp>
#include <vbte/rendering/cube.hpp>
#include <vbte/rendering/drawable.hpp>
#include <vbte/rendering/rendering_system.hpp>

namespace vbte {
	namespace rendering {
		rendering_system::rendering_system(core::engine& engine)
		: engine_{engine}, debug_face_color_{0.98f, 0.49f, 0.08f}, debug_edge_color_{1.f},
		mode_{rendering_mode::shaded} {
			basic_layout_.emplace_back("_position", 3, GL_FLOAT, false, sizeof(basic_vertex), offsetof(basic_vertex, position));
			basic_layout_.emplace_back("_normal", 3, GL_FLOAT, false, sizeof(basic_vertex), offsetof(basic_vertex, normal));

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

			vertex_shader = shader_manager.load("shaders/blinn_phong.vert", GL_VERTEX_SHADER);
			if (!vertex_shader) {
				throw std::runtime_error{"could not load shaders/blinn_phong.vert"};
			}
			light_program_.attach_shader(vertex_shader);
			fragment_shader = shader_manager.load("shaders/blinn_phong.frag", GL_FRAGMENT_SHADER);
			if (!fragment_shader) {
				throw std::runtime_error{"could not load shaders/blinn_phong.frag"};
			}
			light_program_.attach_shader(fragment_shader);
			basic_layout_.setup_program(light_program_, "frag_color");
			light_program_.link();
		}

		rendering_system::~rendering_system() noexcept {

		}

		void rendering_system::update(float delta_time) {
			glEnable(GL_DEPTH_TEST);

			auto& camera = engine_.camera();

			if (mode_ == rendering_mode::wireframe) {
				debug_program_.use();
				debug_program_.uniform("projection", false, camera.projection());
				debug_program_.uniform("view", false, camera.view());

				for (auto& geometry : draw_queue_) {
					debug_program_.uniform("color", debug_edge_color_);
					debug_program_.uniform("model", false, geometry->transform());

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glLineWidth(1.f);
					geometry->draw();

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(1.f, 1.f);
					glCullFace(GL_FRONT);

					auto clear_color = engine_.graphics_system().clear_color();
					debug_program_.uniform("color", glm::vec3{clear_color});
					geometry->draw();

					glDisable(GL_POLYGON_OFFSET_FILL);
				}
			} else if (mode_ == rendering_mode::wireframe_filled) {
				for (auto& geometry : draw_queue_) {
					debug_program_.use();
					debug_program_.uniform("projection", false, camera.projection());
					debug_program_.uniform("view", false, camera.view());
					debug_program_.uniform("color", debug_edge_color_);
					debug_program_.uniform("model", false, geometry->transform());

					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glLineWidth(1.f);
					geometry->draw();

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(1.f, 1.f);
					glCullFace(GL_FRONT);

					auto clear_color = engine_.graphics_system().clear_color();
					light_program_.use();
					light_program_.uniform("projection", false, camera.projection());
					light_program_.uniform("view", false, camera.view());
					light_program_.uniform("model", false, geometry->transform());
					light_program_.uniform("mit", false, glm::inverseTranspose(geometry->transform()));
					light_program_.uniform("view_vector", camera.position());
					light_program_.uniform("color", debug_face_color_);
					light_program_.uniform("light_direction", glm::vec3{0.f, 1.f, 0.f});
					light_program_.uniform("light_color", glm::vec3{1.f});
					light_program_.uniform("light_energy", 1.0f);
					light_program_.uniform("color", debug_face_color_);
					geometry->draw();

					glDisable(GL_POLYGON_OFFSET_FILL);
				}
			} else if (mode_ == rendering_mode::solid) {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				debug_program_.use();
				debug_program_.uniform("projection", false, camera.projection());
				debug_program_.uniform("view", false, camera.view());
				debug_program_.uniform("color", debug_face_color_);

				for (auto& geometry : draw_queue_) {
					debug_program_.uniform("model", false, geometry->transform());
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					geometry->draw();
				}

				glDisable(GL_CULL_FACE);
			} else if (mode_ == rendering_mode::shaded) {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				light_program_.use();
				light_program_.uniform("projection", false, camera.projection());
				light_program_.uniform("view", false, camera.view());
				light_program_.uniform("view_vector", camera.position());
				light_program_.uniform("color", debug_face_color_);
				light_program_.uniform("light_direction", glm::vec3{0.f, 1.f, 0.f});
				light_program_.uniform("light_color", glm::vec3{1.f});
				light_program_.uniform("light_energy", 1.0f);

				for (auto& geometry : draw_queue_) {
					light_program_.uniform("model", false, geometry->transform());
					light_program_.uniform("mit", false, glm::inverseTranspose(geometry->transform()));
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					geometry->draw();
				}

				glDisable(GL_CULL_FACE);
			}

			static bounding_box bb{engine_};

			debug_program_.use();
			debug_program_.uniform("projection", false, camera.projection());
			debug_program_.uniform("view", false, camera.view());
			debug_program_.uniform("color", glm::vec3{1.f, 0.f, 0.f});

			for (auto& bounding_box : bounding_box_draw_queue_) {
				auto scale = glm::scale(glm::mat4{1.f}, 2.f * bounding_box.half_extend);
				auto translation = glm::translate(glm::mat4{1.f}, bounding_box.position);
				auto rotation = glm::mat4_cast(bounding_box.rotation);
				auto model = translation * rotation * scale;
				debug_program_.uniform("model", false, model);
				bb.draw();
			}

			glDisable(GL_DEPTH_TEST);

			draw_queue_.clear();
		}

		void rendering_system::draw(drawable* geometry) {
			draw_queue_.emplace_back(geometry);
		}

		void rendering_system::draw_bounding_box(const glm::vec3& half_extend, const glm::vec3& position, const glm::quat& rotation) {
			bounding_box_draw_queue_.emplace_back(bounding_box_uniforms{half_extend, position, rotation});
		}
	}
}
