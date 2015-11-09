#ifndef __VBTE_GRAPHICS_GRAPHICS_SYSTEM_HPP__
#define __VBTE_GRAPHICS_GRAPHICS_SYSTEM_HPP__

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace graphics {
		class shader_manager;
		class texture_manager;
	}

	namespace core {
		class window;
	}
}

namespace vbte {
	namespace graphics {
		class graphics_system {
		private:
			core::engine& engine_;
			std::unique_ptr<core::window> window_;
			SDL_GLContext context_;
			glm::vec4 clear_color_;
			std::unique_ptr<graphics::shader_manager> shader_manager_;
			std::unique_ptr<graphics::texture_manager> texture_manager_;

		public:
			graphics_system(core::engine& engine);
			~graphics_system();

			graphics_system(const graphics_system&) = delete;
			graphics_system& operator=(const graphics_system&) = delete;

			graphics_system(graphics_system&&) = default;
			graphics_system& operator=(graphics_system&&) = default;

			void begin() noexcept;
			void end(float delta_time) noexcept;
			void clear_color(const glm::vec4& color);

			auto& clear_color() const noexcept {
				return clear_color_;
			}

			auto& window() noexcept {
				return *window_;
			}

			auto& shader_manager() {
				return *shader_manager_;
			}

			auto& texture_manager() {
				return *texture_manager_;
			}
		};
	}
}

#endif
