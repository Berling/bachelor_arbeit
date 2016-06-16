#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace graphics {
		class texture;
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
			std::vector<size_t> sorted_cells_;
			size_t cells_per_dimension_;
			std::thread load_thread_;
			std::atomic_bool loaded_;
			std::vector<std::shared_ptr<const graphics::texture>> textures_;

		public:
			terrain(core::engine& engine, terrain_system& terrain_system, const std::string& path);
			~terrain() noexcept;

			terrain(const terrain&) = delete;
			terrain& operator=(const terrain&) = delete;

			terrain(terrain&&) = default;
			terrain& operator=(terrain&&) = default;

			void draw(bool with_bounding_box = false);
			void draw_normals();
			void update_lod_levels(const glm::vec3& position, bool update_geometry = true);

			auto& cells() noexcept {
				return cells_;
			}

			auto cells_per_dimension() const noexcept {
				return cells_per_dimension_;
			}

			auto& textures() const noexcept {
				return textures_;
			}
		};
	}
}
