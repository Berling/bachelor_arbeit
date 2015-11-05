#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_loader.hpp>
#include <vbte/asset/asset_manager.hpp>
#include <vbte/asset/native_asset_loader.hpp>

namespace vbte {
	namespace asset {
		asset_manager::asset_manager() {
			loaders_.emplace_back(std::make_unique<native_asset_loader>(""));
			loaders_.emplace_back(std::make_unique<native_asset_loader>("assets/"));
			loaders_.emplace_back(std::make_unique<native_asset_loader>("../assets/"));
			loaders_.emplace_back(std::make_unique<native_asset_loader>("../../assets/"));
		}

		std::string asset_manager::native_name(const std::string& name) const {
			for (auto& loader : loaders_) {
				auto native_name = loader->native_name(name);
				if (!native_name.empty()) {
					return native_name;
				}
			}
			return std::string();
		}

		asset_ptr asset_manager::load_new(const std::string& name) {
			for (auto& loader : loaders_) {
				auto ptr = loader->load_asset(name);
				if (ptr) {
					return ptr;
				}
			}
			return nullptr;
		}
	}
}
