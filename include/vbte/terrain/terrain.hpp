#pragma once

#include <memory>
#include <string>
#include <vector>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace terrain {
		class terrain_cell;
		class terrain_system;

		struct terrain_header {
			uint32_t magic = 0;
			uint32_t cell_count = 0;
			uint64_t cell_resolution = 0;
			float cell_length = 0.f;
			uint64_t size = 0;
		};
	}
}

namespace vbte {
	namespace terrain {
		class terrain {
		private:
			core::engine& engine_;
			terrain_system& terrain_system_;
			std::vector<std::unique_ptr<terrain_cell>> cells_;
			size_t cells_per_dimension_;

		public:
			terrain(core::engine& engine, terrain_system& terrain_system, const std::string& path);
			~terrain() = default;

			terrain(const terrain&) = delete;
			terrain& operator=(const terrain&) = delete;

			terrain(terrain&&) = default;
			terrain& operator=(terrain&&) = default;

			void draw();
			void update_lod_levels(const glm::vec3& position);

			auto& cells() noexcept {
				return cells_;
			}

			auto cells_per_dimension() const noexcept {
				return cells_per_dimension_;
			}
		};
	}
}
