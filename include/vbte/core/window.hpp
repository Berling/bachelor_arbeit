#ifndef __VBTE_CORE_WINDOW_HPP__
#define __VBTE_CORE_WINDOW_HPP__

#include <functional>
#include <memory>
#include <string>

#include <SDL2/SDL.h>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace core {
		class window {
		private:
			engine& engine_;
			std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> impl_;
			std::string title_;
			size_t width_;
			size_t height_;
			bool fullscreen_;
			bool borderless_;

		public:
			window(engine& engine);
			~window() noexcept = default;

			window(const window&) = delete;
			window& operator=(const window&) = delete;

			window(window&&) = default;
			window& operator=(window&&) = default;

			const std::string& title() const noexcept {
				return title_;
			}

			void rename(const std::string& title) noexcept {
				title_ = title;
				SDL_SetWindowTitle(impl_.get(), title.c_str());
			}

			size_t width() const noexcept {
				return width_;
			}

			size_t height() const noexcept {
				return height_;
			}

			void resize(size_t width, size_t height) {
				width_ = width;
				height_ = height;
				SDL_SetWindowSize(impl_.get(), width_, height_);
				SDL_SetWindowPosition(impl_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			}

			bool is_fullscreen() const noexcept {
				return fullscreen_;
			}

			void fullscreen(bool fullscreen) {
				fullscreen_ = fullscreen;
				SDL_SetWindowFullscreen(impl_.get(), fullscreen_ ? SDL_WINDOW_FULLSCREEN : 0);
			}

			bool is_borderless() const noexcept {
				return borderless_;
			}

			void borderless(bool borderless) {
				borderless_ = borderless;
				SDL_SetWindowBordered(impl_.get(), borderless_ ? SDL_FALSE : SDL_TRUE);
			}

			SDL_Window* get() noexcept {
				return impl_.get();
			}
		};
	}
}

#endif
