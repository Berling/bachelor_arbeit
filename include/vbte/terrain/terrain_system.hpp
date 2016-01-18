#pragma once

#include <vbte/terrain/volume_data_manager.hpp>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace terrain {
		class terrain_system {
		private:
			core::engine& engine_;
			terrain::volume_data_manager volume_data_manager_;

		public:
			terrain_system(core::engine& engine);
			~terrain_system() = default;

			terrain_system(const terrain_system&) = delete;
			terrain_system& operator=(const terrain_system&) = delete;

			terrain_system(terrain_system&&) = default;
			terrain_system& operator=(terrain_system&&) = default;

			void update(float delta_time);

			auto& volume_data_manager() noexcept {
				return volume_data_manager_;
			}
		};
	}
}
