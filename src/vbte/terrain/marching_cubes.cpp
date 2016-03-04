#include <iostream>

#include <glm/gtc/constants.hpp>

#include <vbte/terrain/marching_cubes.hpp>
#include <vbte/terrain/volume_data.hpp>

namespace vbte {
  namespace terrain {
    std::vector<rendering::basic_vertex> generate_triangles(const volume_data& grid, const cell& grid_cell, float isovalue) {
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

      auto half_sample_rate = grid.sample_rate() / 2.f;

      std::array<rendering::basic_vertex, 12> vertex_list;
      if (edge_table[cube_index] & 1) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[1], grid_cell.values[0], grid_cell.values[1]);
        vertex_list[0] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 2) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[2], grid_cell.values[1], grid_cell.values[2]);
        vertex_list[1] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 4) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[2], grid_cell.values[3], grid_cell.values[2]);
        vertex_list[2] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 8) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[0], grid_cell.values[3], grid_cell.values[0]);
        vertex_list[3] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 16) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[4], grid_cell.vertices[5], grid_cell.values[4], grid_cell.values[5]);
        vertex_list[4] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 32) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[5], grid_cell.vertices[6], grid_cell.values[5], grid_cell.values[6]);
        vertex_list[5] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 64) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[6], grid_cell.vertices[7], grid_cell.values[6], grid_cell.values[7]);
        vertex_list[6] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 128) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[7], grid_cell.vertices[4], grid_cell.values[7], grid_cell.values[4]);
        vertex_list[7] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 256) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[0], grid_cell.vertices[4], grid_cell.values[0], grid_cell.values[4]);
        vertex_list[8] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 512) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[1], grid_cell.vertices[5], grid_cell.values[1], grid_cell.values[5]);
        vertex_list[9] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 1024) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[2], grid_cell.vertices[6], grid_cell.values[2], grid_cell.values[6]);
        vertex_list[10] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
        };
      }
      if (edge_table[cube_index] & 2048) {
        auto vertex = interpolate_vertex(isovalue, grid_cell.vertices[3], grid_cell.vertices[7], grid_cell.values[3], grid_cell.values[7]);
        vertex_list[11] = rendering::basic_vertex{
          vertex,
          calculate_normal(grid, vertex, half_sample_rate)
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
            auto cell_triangles = generate_triangles(grid, c, 1.f);
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

    glm::vec3 calculate_normal(const volume_data& grid, const glm::vec3& p, float step_size) {
      static auto sample = [](const volume_data& data, const glm::vec3& point) {
        auto sample_rate = data.sample_rate();
        auto index_x = static_cast<int>(point.x / sample_rate);
        auto index_y = static_cast<int>(point.y / sample_rate);
        auto index_z = static_cast<int>(point.z / sample_rate);

        auto resolution = data.resolution();
        auto x0 = index_x * sample_rate;
        auto y0 = index_y * sample_rate;
        auto z0 = index_z * sample_rate;
        auto x1 = (index_x + 1) * sample_rate;
        if (point.x >= (resolution - 1) * sample_rate) {
          x1 = (index_x - 1) * sample_rate;
        } else if (point.x <= 0.f) {
          x1 = (index_x + 1) * sample_rate;
        }
        auto y1 = (index_y + 1) * sample_rate;
        if (point.y >= (resolution - 1) * sample_rate) {
          y1 = (index_y - 1) * sample_rate;
        } else if (point.y <= 0.f) {
          y1 = (index_y + 1) * sample_rate;
        }
        auto z1 = (index_z + 1) * sample_rate;
        if (point.z >= (resolution - 1) * sample_rate) {
          z1 = (index_z - 1) * sample_rate;
        } else if (point.x <= 0.f) {
          z1 = (index_z + 1) * sample_rate;
        }

        auto xd = (point.x - x0) / (x1 - x0);
        auto yd = (point.y - y0) / (y1 - y0);
        auto zd = (point.z - z0) / (z1 - z0);

        auto c00 = data.value(int(x0 / sample_rate), int(y0 / sample_rate), int(z0 / sample_rate)) * (1.f - xd)
          + data.value(int(x1 / sample_rate), int(y0 / sample_rate), int(z0 / sample_rate)) * xd;
        auto c01 = data.value(int(x0 / sample_rate), int(y0 / sample_rate), int(z1 / sample_rate)) * (1.f - xd)
          + data.value(int(x1 / sample_rate), int(y0 / sample_rate), int(z1 / sample_rate)) * xd;
        auto c10 = data.value(int(x0 / sample_rate), int(y1 / sample_rate), int(z0 / sample_rate)) * (1.f - xd)
          + data.value(int(x1 / sample_rate), int(y1 / sample_rate), int(z0 / sample_rate)) * xd;
        auto c11 = data.value(int(x0 / sample_rate), int(y1 / sample_rate), int(z1 / sample_rate)) * (1.f - xd)
          + data.value(int(x1 / sample_rate), int(y1 / sample_rate), int(z1 / sample_rate)) * xd;

        auto c0 = c00 * (1.f - yd) + c10 * yd;
        auto c1 = c01 * (1.f - yd) + c11 * yd;

        return c0 * (1.f - zd) + c1 * zd;
      };

      auto gradient = glm::vec3{
        (sample(grid, p - glm::vec3{step_size, 0.f, 0.f}) - sample(grid, p + glm::vec3{step_size, 0.f, 0.f})) / (2.f * step_size),
        (sample(grid, p - glm::vec3{0.f, step_size, 0.f}) - sample(grid, p + glm::vec3{0.f, step_size, 0.f})) / (2.f * step_size),
        (sample(grid, p - glm::vec3{0.f, 0.f, step_size}) - sample(grid, p + glm::vec3{0.f, 0.f, step_size})) / (2.f * step_size)
      };

      return glm::normalize(gradient);
    }
  }
}
