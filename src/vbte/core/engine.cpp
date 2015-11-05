#include <SDL2/SDL.h>

#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_loader.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/utils/config.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace core {
		engine::engine()
		: quit_(false), config_manager_(*this) {
			utils::log("game.log");
			config_ = this->config_manager().load("config/settings");
			if (!config_) {
				throw std::runtime_error{"could not load config from file config/settings.user or config/settings.main"};
			}
			if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
				auto sdl_error = std::string(SDL_GetError());
				SDL_ClearError();
				throw std::runtime_error("could not initialize SDL" + sdl_error);
			}

			graphics_system_ = std::make_unique<graphics::graphics_system>(*this);
		}

		engine::~engine() noexcept {
			SDL_Quit();
		}

		void engine::update(float delta_time) {
			graphics_system_->begin();
			graphics_system_->end(delta_time);


		}

		void engine::run() {
			auto event = SDL_Event();
			auto last_time = 0.f;
			auto current_time = static_cast<float>(SDL_GetTicks()) / 1000.f;
			auto delta_time = 0.f;

			while (!quit_) {
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						quit_ = true;
					}
				}
				last_time = current_time;
				current_time = static_cast<float>(SDL_GetTicks()) / 1000.f;
				delta_time = current_time - last_time;

				update(delta_time);
			}
		}
	}
}
