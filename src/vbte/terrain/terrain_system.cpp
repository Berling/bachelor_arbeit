#include <vbte/core/engine.hpp>
#include <vbte/terrain/terrain_system.hpp>

namespace vbte {
	namespace terrain {
		terrain_system::terrain_system(core::engine& engine)
		: engine_{engine}, volume_data_manager_{engine} {}

		void terrain_system::update(float delta_time) {

		}
	}
}
