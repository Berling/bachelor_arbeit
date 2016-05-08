#include <SDL2/SDL.h>
#include <iostream>

#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_loader.hpp>
#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/core/window.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/rendering/cube.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>
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
			rendering_system_ = std::make_unique<rendering::rendering_system>(*this);
			terrain_system_ = std::make_unique<terrain::terrain_system>(*this);
			camera_ = std::make_unique<core::camera>(*this, glm::vec3{0.f, 0.f, 10.f}, glm::radians(45.f), 0.1f, 1000.f);

			/*auto& volume_data_manager = terrain_system_->volume_data_manager();
			auto sphere = volume_data_manager.load("terrain/test.vol");
			if (!sphere) {
				throw std::runtime_error{"could not load terrain/test.vol"};
			}
			utils::log(utils::log_level::debug) << sphere->value(64, 26, 64) << std::endl;*/

			SDL_ShowCursor(SDL_FALSE);

			auto& window = graphics_system_->window();
			auto width = static_cast<float>(window.width());
			auto height = static_cast<float>(window.height());
			SDL_WarpMouseInWindow(window.get(), width / 2.f, height / 2.f);
		}

		engine::~engine() noexcept {
			SDL_Quit();
		}

		void engine::update(float delta_time) {
			//static rendering::cube c{*this, glm::vec3{-2.f, 0.f, 0.f}, glm::angleAxis(glm::radians(0.f), glm::vec3{0.f}), glm::vec3{1.f}};
			//static rendering::cube c1{*this, glm::vec3{2.f, 0.f, 0.f}, glm::angleAxis(glm::radians(0.f), glm::vec3{0.f}), glm::vec3{1.f}};
			static terrain::terrain_cell t{*this, glm::vec3{0.f}, glm::angleAxis(glm::radians(0.f), glm::vec3{0.f}), "terrain/test.vol"};
			//static terrain::terrain_cell t2{*this, glm::vec3{5.f - 5.f / 64, 0.f, 0.f}, glm::angleAxis(glm::radians(0.f), glm::vec3{0.f}), "terrain/test2.vol"};
			//static terrain::terrain_cell sphere{*this, glm::vec3{0.f, -2.5f, 0.f}, glm::angleAxis(glm::radians(0.f), glm::vec3{0.f}), "terrain/sphere.vol"};
			//rendering_system_->draw(&c);
			//rendering_system_->draw(&c1);
			t.update_geometry();
			rendering_system_->draw(&t);
			rendering_system_->draw_bounding_box(glm::vec3{t.volume_data().grid_length() / 2.f}, glm::vec3{t.volume_data().grid_length() / 2.f}, glm::angleAxis(0.f, glm::vec3{0.f}));
			//rendering_system_->draw(&t2);
			//rendering_system_->draw(&sphere);

			graphics_system_->begin();
			rendering_system_->update(delta_time);
			graphics_system_->end(delta_time);
		}

		void engine::update_input(float delta_time) {
			const Uint8* state = nullptr;
			state = SDL_GetKeyboardState(nullptr);

			if (state[SDL_SCANCODE_W]) {
				camera_->move_forward(true);
			} else {
				camera_->move_forward(false);
			}

			if (state[SDL_SCANCODE_S]) {
				camera_->move_backward(true);
			} else {
				camera_->move_backward(false);
			}

			if (state[SDL_SCANCODE_A]) {
				camera_->move_left(true);
			} else {
				camera_->move_left(false);
			}

			if (state[SDL_SCANCODE_D]) {
				camera_->move_right(true);
			} else {
				camera_->move_right(false);
			}

			auto& window = graphics_system_->window();
			auto width = static_cast<float>(window.width());
			auto height = static_cast<float>(window.height());
			auto x = 0;
			auto y = 0;
			SDL_GetMouseState(&x, &y);
			auto xrel = 2.f * (x / width) - 1.f;
			auto yrel = 2.f * (y / height) - 1.f;

			auto local_rot_z = xrel * 40.f * delta_time;
			auto azimuth = camera_->azimuth() + local_rot_z;
			camera_->azimuth(azimuth);

			auto elevation = camera_->elevation() - yrel * 30.f * delta_time;
			if (glm::abs(elevation) <= glm::pi<float>() / 2.f) {
				camera_->elevation(elevation);
			}

			SDL_WarpMouseInWindow(window.get(), width / 2.f, height / 2.f);

			camera_->update(delta_time);
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
					} else if (event.type == SDL_KEYDOWN) {
						switch (event.key.keysym.scancode) {
							case SDL_SCANCODE_1:
								rendering_system_->change_mode(rendering::rendering_mode::shaded);
								break;
							case SDL_SCANCODE_2:
								rendering_system_->change_mode(rendering::rendering_mode::solid);
								break;
							case SDL_SCANCODE_3:
								rendering_system_->change_mode(rendering::rendering_mode::wireframe_filled);
								break;
							case SDL_SCANCODE_4:
								rendering_system_->change_mode(rendering::rendering_mode::wireframe);
								break;
							default:
								break;
						}
					}
				}
				last_time = current_time;
				current_time = static_cast<float>(SDL_GetTicks()) / 1000.f;
				delta_time = current_time - last_time;

				update_input(delta_time);
				update(delta_time);
			}
		}
	}
}
