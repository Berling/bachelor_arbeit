#include <vbte/asset/asset.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/shader_manager.hpp>

namespace vbte {
	namespace graphics {
		shader_manager::shader_manager(core::engine& engine) noexcept
		: engine_(engine) {}

		shader_ptr shader_manager::load_new(const std::string& name, GLenum type) {
			auto asset = engine_.asset_manager().load(name);
			if (!asset) {
				return nullptr;
			}
			auto source_ptr = std::string(asset->content().begin(), asset->content().end());
			return std::make_shared<const shader>(name, type, source_ptr);
		}
	}
}
