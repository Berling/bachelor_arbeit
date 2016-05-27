#include <algorithm>
#include <stdexcept>

#include <glm/gtx/string_cast.hpp>

#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_manager.hpp>
#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/terrain/terrain.hpp>
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>

namespace vbte {
	namespace terrain {
		terrain::terrain(core::engine& engine, terrain_system& terrain_system, const std::string& path) : engine_{engine},
		                                                                                                  terrain_system_{terrain_system} {
			auto& asset_manager = engine_.asset_manager();
			auto terrain_info = asset_manager.load(path);
			if (!terrain_info) {
				throw std::runtime_error{"could not load terrain info " + path};
			}

			auto& content = terrain_info->content();
			auto data_ptr = content.data();
			terrain_header head;

			head.magic = *reinterpret_cast<const uint32_t*>(data_ptr);
			if (head.magic != 0x31495456) {
				throw std::runtime_error{path + " contains no terrain data"};
			}
			data_ptr += sizeof(uint32_t);

			head.cell_count = *reinterpret_cast<const uint32_t*>(data_ptr);
			data_ptr += sizeof(uint32_t);
			cells_per_dimension_ = head.cell_count;

			head.cell_resolution = *reinterpret_cast<const uint64_t*>(data_ptr);
			data_ptr += sizeof(uint64_t);

			head.cell_length = *reinterpret_cast<const float*>(data_ptr);
			data_ptr += sizeof(float);

			head.size = *reinterpret_cast<const uint64_t*>(data_ptr);
			if (head.size != content.size()) {
				throw std::runtime_error{path + " actual size: " + std::to_string(content.size()) + " calculated size: " + std::to_string(head.size)};
			}
			data_ptr += sizeof(uint64_t);

			for (auto x = 0; x < head.cell_count; ++x) {
				for (auto y = 0; y < 2; ++y) {
					for (auto z = 0; z < head.cell_count; ++z) {
						auto path_size = *reinterpret_cast<const uint64_t*>(data_ptr);
						data_ptr += sizeof(uint64_t);
						std::string cell_path{data_ptr, path_size};
						data_ptr += path_size;

						cells_.emplace_back(
							std::make_unique<terrain_cell>(
								engine_,
								terrain_system_,
								*this,
								glm::ivec3{x, y, z},
								glm::vec3{x * head.cell_length, y * head.cell_length, z * head.cell_length},
								glm::angleAxis(0.f, glm::vec3{0.f}),
								cell_path
							)
						);
					}
				}
			}

			auto& camera = engine_.camera();
			update_lod_levels(camera.position(), false);
		}

		void terrain::draw() {
			static auto& rendering_system = engine_.rendering_system();

			for (auto& cell : cells_) {
				if (!cell->is_empty()) {
					rendering_system.draw(cell.get());
				}
				rendering_system.draw_bounding_box(glm::vec3{cell->volume_data().grid_length() / 2.f}, cell->position() + glm::vec3{cell->volume_data().grid_length() / 2.f}, glm::angleAxis(0.f, glm::vec3{0.f}));
			}
		}

		void terrain::update_lod_levels(const glm::vec3& position, bool update_geometry) {
			auto shortest_distance = [&](const auto& cell, const auto& position) {
				std::vector<float> dists;
				dists.emplace_back(glm::length2((cell->position() + cell->volume_data().grid_length() / 2.f) - position));
				dists.emplace_back(glm::length2((cell->position()) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{cell->volume_data().grid_length(), 0.f, 0.f}) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{0.f, cell->volume_data().grid_length(), 0.f}) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{0.f, 0.f, cell->volume_data().grid_length()}) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{cell->volume_data().grid_length(), cell->volume_data().grid_length(), 0.f}) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{cell->volume_data().grid_length(), 0.f, cell->volume_data().grid_length()}) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{cell->volume_data().grid_length(), cell->volume_data().grid_length(), 0.f}) - position));
				dists.emplace_back(glm::length2((cell->position() + glm::vec3{cell->volume_data().grid_length(), cell->volume_data().grid_length(), cell->volume_data().grid_length()}) - position));
				std::sort(dists.begin(), dists.end());
				return dists.front();
			};

			auto magic_distance = 400.f;
			auto magic_distance2 = 1000.f;
			for (auto& cell : cells_) {
				auto distance = shortest_distance(cell, position);
				if (distance > magic_distance2) {
					cell->lod_level(2);
				} else if(distance > magic_distance) {
					cell->lod_level(1);
				} else {
					cell->lod_level(0);
				}
			}

			for (auto& cell: cells_) {
				auto resolution = cell->volume_data().resolution();
				cell->update_adjacent_cells_info();
				if (update_geometry) {
					cell->update_geometry(resolution >> cell->lod_level());
				}
			}
		}
	}
}
