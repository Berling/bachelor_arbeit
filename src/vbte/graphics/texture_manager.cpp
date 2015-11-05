#include <vbte/asset/asset.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/texture.hpp>
#include <vbte/graphics/texture_manager.hpp>
#include <vbte/utils/load_dds.hpp>


namespace vbte {
	namespace graphics {
		texture_manager::texture_manager(core::engine& engine) noexcept
		: engine_(engine) {}

		texture_ptr texture_manager::load_new(const std::string& name) {
			auto asset = engine_.asset_manager().load(name);
			if (!asset) {
				return nullptr;
			}
			auto& content = asset->content();
			auto texture = gli::texture2D(gli::load_dds(content));
			if (texture.empty()) {
				return nullptr;
			}
			return std::make_shared<const graphics::texture>(texture);
		}
	}
}
