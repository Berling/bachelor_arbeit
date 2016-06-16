#include <algorithm>
#include <stdexcept>

#include <glm/gtx/string_cast.hpp>

#include <vbte/asset/asset.hpp>
#include <vbte/asset/asset_manager.hpp>
#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/texture.hpp>
#include <vbte/graphics/texture_manager.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/terrain/terrain.hpp>
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>

namespace vbte {
	namespace terrain {
		terrain::terrain(core::engine& engine, terrain_system& terrain_system, const std::string& path) : engine_{engine},
		                                                                                                  terrain_system_{terrain_system},
		                                                                                                  loaded_{false} {
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

			auto load_task = [&]() {
				for (auto& cell : cells_) {
					cell->load();
				}
				loaded_.store(true);
			};
			load_thread_ = std::thread(load_task);

			auto& texture_manager = engine_.graphics_system().texture_manager();

			auto texture = texture_manager.load("textures/terrain/rock_diffuse.dds");
			if (!texture) {
				utils::log(utils::log_level::fatal) << "could not load texture textures/terrain/rock_diffuse.dds" << std::endl;
			}
			textures_.emplace_back(texture);

			texture = texture_manager.load("textures/terrain/grass_diffuse.dds");
			if (!texture) {
				utils::log(utils::log_level::fatal) << "could not load texture textures/terrain/grass_diffuse.dds" << std::endl;
			}
			textures_.emplace_back(texture);
		}

		terrain::~terrain() noexcept {
			load_thread_.join();
		}

		void terrain::draw(bool with_bounding_box) {
			static auto& rendering_system = engine_.rendering_system();

			auto i = 0;
			for (auto& cell : cells_) {
				if (!cell->is_empty()) {
					rendering_system.draw(cell.get());
				}
				if (with_bounding_box && cell->is_loaded()) {
					rendering_system.draw_bounding_box(glm::vec3{cell->volume_data().grid_length() / 2.f}, cell->position() + glm::vec3{cell->volume_data().grid_length() / 2.f}, glm::angleAxis(0.f, glm::vec3{0.f}));
				}
			}
		}

		void terrain::update_lod_levels(const glm::vec3& position, bool update_geometry) {
			for (auto& cell : cells_) {
				cell->initialize();
			}

			if (loaded_.load()) {
				auto& camera = engine_.camera();

				const auto magic_size_1 = 1.f;
				const auto magic_size_2 = 0.8f;

				std::unordered_map<intptr_t, float> distances;

				auto projected_size = [&](auto& cell) {
					if (cell->is_loaded()) {
						auto center = cell->position() + glm::vec3{cell->volume_data().grid_length() / 2.f};
						auto half_extend = glm::vec3{cell->volume_data().grid_length() / 2.f};
						auto radius = glm::vec3{glm::length(half_extend), 0.f, 0.f};
						auto center_view = camera.view() * glm::vec4{center, 1.f};
						auto radius_view = center_view + glm::vec4{radius, 0.f};

						auto center_projected = camera.projection() * glm::vec4{center_view.x, center_view.y, center_view.z, 1.f};
						center_projected /= center_projected.w;
						auto radius_projected = camera.projection() * glm::vec4{radius_view.x, radius_view.y, radius_view.z, 1.f};
						radius_projected /= radius_projected.w;
						auto projected_size = glm::length(center_projected - radius_projected);

						auto key = reinterpret_cast<intptr_t>(cell.get());
						distances[key] = projected_size;

						return projected_size;
					} else {
						return 0.f;
					}
				};

				auto cmp = [&](const auto& a, const auto& b) {
					return projected_size(cells_.at(a)) > projected_size(cells_.at(b));
				};

				std::sort(sorted_cells_.begin(), sorted_cells_.end(), cmp);

				std::unordered_map<size_t, bool> vistied_cells;

				for (auto& index : sorted_cells_) {
					vistied_cells[index] = true;
					auto& cell = cells_[index];
					auto key = reinterpret_cast<intptr_t>(cell.get());
					auto size = distances[key];
					auto current_lod_level = cell->lod_level();

					if (size > magic_size_1) {
						cell->lod_level(0);
					} else if (size > magic_size_2) {
						cell->lod_level(1);
					} else {
						cell->lod_level(2);
					}

					auto& adjacent_cells = cell->adjacent_cells();
					for (auto& adjacent_cell_info : adjacent_cells) {
						if (adjacent_cell_info.index != -1 &&
								vistied_cells.find(adjacent_cell_info.index) != vistied_cells.end() &&
								vistied_cells[adjacent_cell_info.index]) {
							auto& adjacent_cell = cells_[adjacent_cell_info.index];
							auto lod_level = cell->lod_level();
							auto adj_lod_level = adjacent_cell->lod_level();

							if (lod_level - adj_lod_level > 1) {
								cell->lod_level(lod_level - 1);
							} else if (lod_level - adj_lod_level < -1) {
								cell->lod_level(lod_level + 1);
							}
						}
					}

					if (current_lod_level != cell->lod_level()) {
						cell->build();
					}
				}

				for (auto& cell : cells_) {
					cell->update_adjacent_cells_info();
				}

				for (auto& cell : cells_) {
					auto resolution = cell->volume_data().resolution();
					if (update_geometry) {
						cell->update_geometry(resolution >> cell->lod_level());
					}
				}
			}
		}
	}
}
