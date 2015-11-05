#include <string>
#include <vector>

#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_loader.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/utils/logger.hpp>
#include <vbte/utils/config.hpp>
#include <vbte/utils/config_manager.hpp>

namespace vbte {
	namespace utils {
		config_manager::config_manager(core::engine& engine) noexcept
		: engine_(engine) {}

		config_ptr config_manager::load_new(const std::string& name) {
			auto asset = engine_.asset_manager().load(name + ".user");
			if (!asset) {
				asset = engine_.asset_manager().load(name + ".main");;
				if (!asset) {
					return nullptr;
				}
			}

			auto& content = asset->content();
			return std::make_shared<config>(engine_, asset->name(), std::string{content.begin(), content.end()});
		}
	}
}