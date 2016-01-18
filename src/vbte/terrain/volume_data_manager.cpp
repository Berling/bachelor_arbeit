#include <vbte/asset/asset.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/terrain/volume_data.hpp>
#include <vbte/terrain/volume_data_manager.hpp>

namespace vbte {
	namespace terrain {
		volume_data_manager::volume_data_manager(core::engine& engine) noexcept :
		engine_{engine} {}

		volume_data_ptr volume_data_manager::load_new(const std::string& file_name) {
			auto asset = engine_.asset_manager().load(file_name);
			if (!asset) {
				return nullptr;
			}
			return std::make_shared<const volume_data>(file_name, asset->content());
		}
	}
}
