#include <stdexcept>

#include <vbte/core/engine.hpp>
#include <vbte/core/window.hpp>
#include <vbte/utils/config.hpp>
#include <vbte/utils/config_manager.hpp>

namespace vbte {
	namespace core {
		window::window(engine& engine)
		: engine_(engine), impl_(nullptr, +[](SDL_Window*) {}) {

			title_ = engine_.config().get<std::string>("title", "engine");
			width_ = engine_.config().get<unsigned long long>("width", 800);
			height_ = engine_.config().get<unsigned long long>("height", 600);
			fullscreen_ = engine_.config().get<bool>("fullscreen", false);
			borderless_ = engine_.config().get<bool>("borderless", false);

			auto window_mask = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
			if (fullscreen_) {
				window_mask |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			}
			if (borderless_) {
				window_mask |= SDL_WINDOW_BORDERLESS;
			}

			impl_ = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(
				SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, window_mask),
				SDL_DestroyWindow
			);

			if (!impl_) {
				throw std::runtime_error("could not create window");
			}
		}
	}
}
