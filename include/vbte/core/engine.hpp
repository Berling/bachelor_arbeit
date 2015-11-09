#ifndef __VBTE_CORE_ENGINE_HPP__
#define __VBTE_CORE_ENGINE_HPP__

#include <vbte/asset/asset_manager.hpp>
#include <vbte/utils/config_manager.hpp>
#include <vbte/utils/config.hpp>

namespace vbte {
	namespace core {
		class camera;
	}

	namespace graphics {
		class graphics_system;
	}

	namespace rendering {
		class rendering_system;
	}

	namespace utils {
		class config;
	}
}

namespace vbte {
	namespace core {
		class engine {
		private:
			bool quit_;
			asset::asset_manager asset_manager_;
			utils::config_manager config_manager_;
			std::shared_ptr<utils::config> config_;
			std::unique_ptr<graphics::graphics_system> graphics_system_;
			std::unique_ptr<rendering::rendering_system> rendering_system_;
			std::unique_ptr<core::camera> camera_;

		public:
			engine();
			~engine() noexcept;

			engine(const engine&) = delete;
			engine& operator=(const engine&) = delete;

			engine(engine&&) = delete;
			engine& operator=(engine&&) = delete;

			void update(float delta_time);
			void run();

			void quit() noexcept {
				quit_ = true;
			}

			asset::asset_manager& asset_manager() noexcept {
				return asset_manager_;
			}

			utils::config& config() noexcept {
				return *config_;
			}

			utils::config_manager& config_manager() noexcept {
				return config_manager_;
			}

			graphics::graphics_system& graphics_system() noexcept {
				return *graphics_system_;
			}

			auto& rendering_system() noexcept {
				return *rendering_system_;
			}

			auto& camera() noexcept {
				return *camera_;
			}
		};
	}
}

#endif
