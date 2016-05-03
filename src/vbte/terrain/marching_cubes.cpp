#include <chrono>
#include <iostream>

#include <glm/gtc/constants.hpp>

#include <tbb/blocked_range3d.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

#include <vbte/terrain/marching_cubes.hpp>
#include <vbte/terrain/volume_data.hpp>

namespace vbte {
	namespace terrain {
		std::vector<rendering::basic_vertex> generate_triangles(const volume_data& grid, const cell& grid_cell, size_t resolution, float isovalue) {
			auto cube_index = 0;
			if (grid_cell.values[0] < isovalue) { cube_index |= 1; }
			if (grid_cell.values[1] < isovalue) { cube_index |= 2; }
			if (grid_cell.values[2] < isovalue) { cube_index |= 4; }
			if (grid_cell.values[3] < isovalue) { cube_index |= 8; }
			if (grid_cell.values[4] < isovalue) { cube_index |= 16; }
			if (grid_cell.values[5] < isovalue) { cube_index |= 32; }
			if (grid_cell.values[6] < isovalue) { cube_index |= 64; }
			if (grid_cell.values[7] < isovalue) { cube_index |= 128; }

			if (cube_index == 0) {
				return std::vector<rendering::basic_vertex>{};
			}

			auto sample_rate = grid.grid_length() / resolution;
			auto half_sample_rate = sample_rate / 2.f;

			std::array<rendering::basic_vertex, 12> vertex_list;
			if (edge_table[cube_index] & 1) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[1], grid_cell.values[0], grid_cell.values[1]);
				vertex_list[0] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 2) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[2], grid_cell.values[1], grid_cell.values[2]);
				vertex_list[1] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 4) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[2], grid_cell.values[3], grid_cell.values[2]);
				vertex_list[2] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 8) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[0], grid_cell.values[3], grid_cell.values[0]);
				vertex_list[3] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 16) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[4], grid_cell.vertices[5], grid_cell.values[4], grid_cell.values[5]);
				vertex_list[4] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 32) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[5], grid_cell.vertices[6], grid_cell.values[5], grid_cell.values[6]);
				vertex_list[5] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 64) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[6], grid_cell.vertices[7], grid_cell.values[6], grid_cell.values[7]);
				vertex_list[6] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 128) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[7], grid_cell.vertices[4], grid_cell.values[7], grid_cell.values[4]);
				vertex_list[7] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 256) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[4], grid_cell.values[0], grid_cell.values[4]);
				vertex_list[8] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 512) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[5], grid_cell.values[1], grid_cell.values[5]);
				vertex_list[9] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 1024) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[2], grid_cell.vertices[6], grid_cell.values[2], grid_cell.values[6]);
				vertex_list[10] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}
			if (edge_table[cube_index] & 2048) {
				auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[7], grid_cell.values[3], grid_cell.values[7]);
				vertex_list[11] = rendering::basic_vertex{
					glm::vec4{vertex, 0.f},
					glm::vec4{calculate_normal(grid, vertex, resolution, half_sample_rate), 0.f}
				};
			}

			std::vector<rendering::basic_vertex> vertices;
			for (auto i = 0; triangle_table[cube_index][i] != -1; i += 3) {
				vertices.emplace_back(vertex_list[triangle_table[cube_index][i]]);
				vertices.emplace_back(vertex_list[triangle_table[cube_index][i + 1]]);
				vertices.emplace_back(vertex_list[triangle_table[cube_index][i + 2]]);
			}

			return vertices;
		}

		std::vector<rendering::basic_vertex> marching_cubes(const volume_data& grid, size_t resolution) {
			std::vector<rendering::basic_vertex> vertices;
			//tbb::concurrent_vector<rendering::basic_vertex> vertices;

			auto sample_rate = grid.grid_length() / resolution;
			//tbb::parallel_for(tbb::blocked_range3d<size_t>(0, resolution, 0, resolution, 0, resolution),
			//	[&](const tbb::blocked_range3d<size_t>& r) {
			//		for (auto x = r.pages().begin(); x != r.pages().end(); ++x) {
			//			for (auto y = r.rows().begin(); y != r.rows().end(); ++y) {
			//				for (auto z = r.cols().begin(); z != r.cols().end(); ++z) {

			auto start = std::chrono::high_resolution_clock::now();
			std::cout << "start" << std::endl;

			for (auto x = 0; x < resolution; ++x) {
				for (auto y = 0; y < resolution; ++y) {
					for (auto z = 0; z < resolution; ++z) {
								auto p = glm::vec3{x, y , z} * sample_rate;
								cell c;
								c.vertices = std::array<glm::vec3, 8>{
									p,
									p + glm::vec3{sample_rate, 0.f, 0.f},
									p + glm::vec3{sample_rate, 0.f, sample_rate},
									p + glm::vec3{0.f, 0.f, sample_rate},
									p + glm::vec3{0.f, sample_rate, 0.f},
									p + glm::vec3{sample_rate, sample_rate, 0.f},
									p + glm::vec3{sample_rate, sample_rate, sample_rate},
									p + glm::vec3{0.f, sample_rate, sample_rate}
								};
								c.values = std::array<float, 8>{
									grid.sample(c.vertices[0], resolution),
									grid.sample(c.vertices[1], resolution),
									grid.sample(c.vertices[2], resolution),
									grid.sample(c.vertices[3], resolution),
									grid.sample(c.vertices[4], resolution),
									grid.sample(c.vertices[5], resolution),
									grid.sample(c.vertices[6], resolution),
									grid.sample(c.vertices[7], resolution)
								};
								auto cell_triangles = generate_triangles(grid, c, resolution, 0.f);
								vertices.insert(vertices.end(), cell_triangles.begin(), cell_triangles.end());
							}
						}
					}

					auto end = std::chrono::high_resolution_clock::now();
					std::cout << "time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
								//vertices.grow_by(cell_triangles.begin(), cell_triangles.end());
			//				}
			//			}
			//		}
			//	}
			//);

			return std::vector<rendering::basic_vertex>{vertices.begin(), vertices.end()};
		}

		glm::vec3 interpolate_vertex(float isovalue, const glm::vec3& p0, const glm::vec3& p1, float s0, float s1) {
			auto alpha = (isovalue - s0) / (s1 - s0);
			return alpha * p1 + (1 - alpha) * p0;
		}

		glm::vec3 calculate_normal(const volume_data& grid, const glm::vec3& p, size_t resolution, float step_size) {
			auto gradient = glm::vec3{
				(grid.sample(p - glm::vec3{step_size, 0.f, 0.f}, resolution) - grid.sample(p + glm::vec3{step_size, 0.f, 0.f}, resolution)) / (2.f * step_size),
				(grid.sample(p - glm::vec3{0.f, step_size, 0.f}, resolution) - grid.sample(p + glm::vec3{0.f, step_size, 0.f}, resolution)) / (2.f * step_size),
				(grid.sample(p - glm::vec3{0.f, 0.f, step_size}, resolution) - grid.sample(p + glm::vec3{0.f, 0.f, step_size}, resolution)) / (2.f * step_size)
			};

			return glm::normalize(gradient);
		}

		size_t estimate_vertex_count(const volume_data& grid, size_t resolution) {
			auto vertex_count = size_t{0};
			auto sample_rate = grid.grid_length() / resolution;

			for (auto x = 0; x < resolution; ++x) {
				for (auto y = 0; y < resolution; ++y) {
					for (auto z = 0; z < resolution; ++z) {
						auto p = glm::vec3{x, y , z} * sample_rate;
						cell grid_cell;
						grid_cell.vertices = std::array<glm::vec3, 8>{
							p,
							p + glm::vec3{sample_rate, 0.f, 0.f},
							p + glm::vec3{sample_rate, 0.f, sample_rate},
							p + glm::vec3{0.f, 0.f, sample_rate},
							p + glm::vec3{0.f, sample_rate, 0.f},
							p + glm::vec3{sample_rate, sample_rate, 0.f},
							p + glm::vec3{sample_rate, sample_rate, sample_rate},
							p + glm::vec3{0.f, sample_rate, sample_rate}
						};
						grid_cell.values = std::array<float, 8>{
							grid.sample(grid_cell.vertices[0], resolution),
							grid.sample(grid_cell.vertices[1], resolution),
							grid.sample(grid_cell.vertices[2], resolution),
							grid.sample(grid_cell.vertices[3], resolution),
							grid.sample(grid_cell.vertices[4], resolution),
							grid.sample(grid_cell.vertices[5], resolution),
							grid.sample(grid_cell.vertices[6], resolution),
							grid.sample(grid_cell.vertices[7], resolution)
						};

						auto isovalue = 0.f;
						auto cube_index = 0;
						if (grid_cell.values[0] < isovalue) { cube_index |= 1; }
						if (grid_cell.values[1] < isovalue) { cube_index |= 2; }
						if (grid_cell.values[2] < isovalue) { cube_index |= 4; }
						if (grid_cell.values[3] < isovalue) { cube_index |= 8; }
						if (grid_cell.values[4] < isovalue) { cube_index |= 16; }
						if (grid_cell.values[5] < isovalue) { cube_index |= 32; }
						if (grid_cell.values[6] < isovalue) { cube_index |= 64; }
						if (grid_cell.values[7] < isovalue) { cube_index |= 128; }

						if (cube_index == 0) {
							continue;
						}

						for (auto i = 0; triangle_table[cube_index][i] != -1; i += 3) {
							vertex_count += 3;
						}
					}
				}
			}

			return vertex_count;
		}
	}
}
