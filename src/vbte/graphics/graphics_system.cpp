#include <stdexcept>
#include <string>

#include <GL/glew.h>

#include <vbte/core/engine.hpp>
#include <vbte/core/window.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/shader_manager.hpp>
#include <vbte/graphics/texture.hpp>
#include <vbte/graphics/texture_manager.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace graphics {
		graphics_system::graphics_system(core::engine& engine)
		: engine_(engine), context_(nullptr), clear_color_(glm::vec4(0.47f, 0.62f, 0.64f, 1.f)),
		shader_manager_(std::make_unique<graphics::shader_manager>(engine_)),
		texture_manager_(std::make_unique<graphics::texture_manager>(engine_)) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8 );

			window_ = std::make_unique<core::window>(engine_);

			if (!window_) {
				throw std::runtime_error("could not create window");
			}

			context_ = SDL_GL_CreateContext(window_->get());

			SDL_GL_SetSwapInterval(1);

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				throw std::runtime_error("could not create OpenGL 3.3 context");
			}

			vbte::utils::log << "OpenGL version " << std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))) << std::endl;

			clear_color(clear_color_);
		}

		graphics_system::~graphics_system() {
			SDL_GL_DeleteContext(context_);
		}

		void graphics_system::begin() noexcept {
			glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void graphics_system::end(float delta_time) noexcept {
			const auto smooth_factor = 0.1f;
			static auto delta_time_smoothed = 0.f;
			static auto time_since_last_fps_output = 0.f;

			delta_time_smoothed = ( 1.0f - smooth_factor) * delta_time_smoothed + smooth_factor * delta_time;

			time_since_last_fps_output += delta_time;
			if(time_since_last_fps_output >= 1.0f){
				time_since_last_fps_output = 0.0f;
				std::ostringstream osstr;
				osstr << window_->title() << " (" << (int((1.0f / delta_time_smoothed) * 10.0f) / 10.0f) << " FPS, ";
				osstr << (int(delta_time_smoothed * 10000.0f) / 10.0f) << " ms / frame)";
				SDL_SetWindowTitle(window_->get(), osstr.str().c_str());
			}
			SDL_GL_SwapWindow(window_->get());
		}

		void graphics_system::clear_color(const glm::vec4& color) {
			clear_color_ = color;
			glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
		}
	}
}
