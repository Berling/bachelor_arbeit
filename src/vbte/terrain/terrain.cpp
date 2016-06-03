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

						sorted_cells_.emplace_back(cells_.size());
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

			cells_[0]->lod_level(0);
			cells_[1]->lod_level(1);
			cells_[2]->lod_level(0);
			cells_[3]->lod_level(0);
			cells_[4]->lod_level(0);
			cells_[5]->lod_level(0);
			cells_[6]->lod_level(0);
			cells_[7]->lod_level(0);

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
/*			std::unordered_map<intptr_t, float> distances;

			auto shortest_distance = [&](const auto& cell) {
				std::vector<float> dists;
				if (!cell) {
					utils::log << "this should not be possible!" << std::endl;
					return 0.f;
				}
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

				if (distances.find(reinterpret_cast<intptr_t>(cell.get())) == distances.end()) {
					distances[reinterpret_cast<intptr_t>(cell.get())] = dists.front();
				}

				return dists.front();
			};

			auto comperator = [&](const auto& first, const auto& second) {
				return shortest_distance(cells_[first]) <= shortest_distance(cells_[second]);
			};

			std::sort(sorted_cells_.begin(), sorted_cells_.end(), comperator);

			std::unordered_map<intptr_t, bool> vistied_cells;

			const auto magic_distance = 400.f;
			const auto magic_distance2 = 1000.f;
			for (auto& cell_index : sorted_cells_) {
				auto& cell = cells_[cell_index];
				auto current_lod_level = cell->lod_level();
				auto key = reinterpret_cast<intptr_t>(cell.get());
				vistied_cells[key] = true;
				if (distances[key] > magic_distance2) {
					cell->lod_level(2);
				} else if (distances[key] > magic_distance) {
					cell->lod_level(1);
				} else {
					cell->lod_level(0);
				}

				for (auto& adjacent_cell_info : cell->adjacent_cells()) {
					if (adjacent_cell_info.index != -1 &&  !cells_[adjacent_cell_info.index]->is_empty()) {
						auto& adjacent_cell = cells_[adjacent_cell_info.index];
						auto adjacent_cell_key = reinterpret_cast<intptr_t>(adjacent_cell.get());
						if (vistied_cells.find(adjacent_cell_key) != vistied_cells.end() && vistied_cells[adjacent_cell_key]) {
							auto adjacent_cell_lod_level = adjacent_cell->lod_level();
							auto lod_level = cell->lod_level();
							if ((lod_level == 0 && adjacent_cell_lod_level == 2) || (lod_level == 2 && adjacent_cell_lod_level == 0)) {
								cell->lod_level(1);
							}
						}
					}
				}

				if (current_lod_level != cell->lod_level()) {
					cell->build();
				}
			}*/

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
