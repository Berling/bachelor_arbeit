#include <vbte/terrain/marching_cubes.hpp>
#include <vbte/terrain/volume_data.hpp>

namespace vbte {
  namespace terrain {
    std::vector<rendering::basic_vertex> generate_triangles(const cell& grid_cell, float isovalue) {
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

      std::array<rendering::basic_vertex, 12> vertex_list;
      if (edge_table[cube_index] & 1) {
        vertex_list[0] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[1], grid_cell.values[0], grid_cell.values[1]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 2) {
        vertex_list[1] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[2], grid_cell.values[1], grid_cell.values[2]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 4) {
        vertex_list[2] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[2], grid_cell.values[3], grid_cell.values[2]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 8) {
        vertex_list[3] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[0], grid_cell.values[3], grid_cell.values[0]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 16) {
        vertex_list[4] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[4], grid_cell.vertices[5], grid_cell.values[4], grid_cell.values[5]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 32) {
        vertex_list[5] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[5], grid_cell.vertices[6], grid_cell.values[5], grid_cell.values[6]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 64) {
        vertex_list[6] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[6], grid_cell.vertices[7], grid_cell.values[6], grid_cell.values[7]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 128) {
        vertex_list[7] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[7], grid_cell.vertices[4], grid_cell.values[7], grid_cell.values[4]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 256) {
        vertex_list[8] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[4], grid_cell.values[0], grid_cell.values[4]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 512) {
        vertex_list[9] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[5], grid_cell.values[1], grid_cell.values[5]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 1024) {
        vertex_list[10] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[2], grid_cell.vertices[6], grid_cell.values[2], grid_cell.values[6]),
          glm::vec3{0.f}
        };
      }
      if (edge_table[cube_index] & 2048) {
        vertex_list[11] = rendering::basic_vertex{
          interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[7], grid_cell.values[3], grid_cell.values[7]),
          glm::vec3{0.f}
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

    std::vector<rendering::basic_vertex> marching_cubes(const volume_data& grid) {
      std::vector<rendering::basic_vertex> vertices;

      auto resolution = grid.resolution();
      auto sample_rate = grid.sample_rate();
      for (auto x = 0; x < resolution - 1; ++x) {
        for (auto y = 0; y < resolution - 1; ++y) {
          for (auto z = 0; z < resolution - 1; ++z) {
            auto p = glm::vec3{x, y , z} * sample_rate;
            cell c = {
              std::array<glm::vec3, 8>{
                p,
                p + glm::vec3{sample_rate, 0.f, 0.f},
                p + glm::vec3{sample_rate, 0.f, sample_rate},
                p + glm::vec3{0.f, 0.f, sample_rate},
                p + glm::vec3{0.f, sample_rate, 0.f},
                p + glm::vec3{sample_rate, sample_rate, 0.f},
                p + glm::vec3{sample_rate, sample_rate, sample_rate},
                p + glm::vec3{0.f, sample_rate, sample_rate}
              },
              std::array<float, 8>{
                grid.value(x, y, z),
                grid.value(x + 1, y, z),
                grid.value(x + 1, y, z + 1),
                grid.value(x, y, z + 1),
                grid.value(x, y + 1, z),
                grid.value(x + 1, y + 1, z),
                grid.value(x + 1, y + 1, z + 1),
                grid.value(x, y + 1, z + 1)
              }
            };
            auto cell_triangles = generate_triangles(c, 1.f);
            vertices.insert(vertices.end(), cell_triangles.begin(), cell_triangles.end());
          }
        }
      }

      return vertices;
    }

    glm::vec3 interpolate_vertex(float isovalue, const glm::vec3& p0, const glm::vec3& p1, float s0, float s1) {
      auto alpha = (isovalue - s0) / (s1 - s0);
      return alpha * p0 + (1 - alpha) * p1;
    }
  }
}
