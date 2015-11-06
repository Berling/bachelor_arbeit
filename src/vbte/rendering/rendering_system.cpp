#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/rendering/basic_vertex.hpp>
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

		}
	}
}