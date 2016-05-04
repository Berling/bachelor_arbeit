#include <GL/glew.h>

#include <vbte/asset/asset.hpp>
#include <vbte/compute/kernel_manager.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/terrain/terrain_system.hpp>

namespace vbte {
	namespace compute {
		kernel_manager::kernel_manager(core::engine& engine, terrain::terrain_system& terrain_system) noexcept
		: engine_{engine}, terrain_system_(terrain_system) {}

		kernel_ptr kernel_manager::load_new(const std::string& file_name, const std::string& kernel_name) {
			auto asset = engine_.asset_manager().load(file_name);
			if (!asset) {
				return nullptr;
			}
			auto source_ptr = std::string(asset->content().begin(), asset->content().end());
			return std::make_shared<kernel>(terrain_system_.compute_context(), kernel_name, source_ptr);
		}
	}
}
