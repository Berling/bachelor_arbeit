#pragma once

#include <cassert>
#include <array>
#include <atomic>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include <vbte/compute/buffer.hpp>
#include <vbte/compute/shared_buffer.hpp>
#include <vbte/graphics/buffer.hpp>
#include <vbte/graphics/vertex_array.hpp>
#include <vbte/rendering/basic_vertex.hpp>
#include <vbte/rendering/drawable.hpp>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace terrain {
		class volume_data;
		class terrain;
		class terrain_system;

		struct adjacent_cell {
			int32_t index = 0;
			uint32_t resolution = 0;
			uint32_t higher_resolution = false;
		};

		struct lod_cache_element {
			graphics::vertex_array vao;
			graphics::vertex_buffer vbo = graphics::vertex_buffer{GL_STATIC_DRAW};
			int vertex_count = 0;
			bool builded = false;
			std::atomic_bool write;

			lod_cache_element() : write{false} {}
		};
	}
}

namespace vbte {
	namespace terrain {
		class terrain_cell : public rendering::drawable {
		private:
			terrain_system& terrain_system_;
			terrain& owner_;
			std::shared_ptr<const class volume_data> volume_data_;
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			graphics::vertex_array vao2_;
			graphics::vertex_buffer vbo2_;
			bool front_ = true;
			bool initial_build_ = true;
			size_t index_count_;
			std::unique_ptr<compute::buffer> volume_buffer_;
			std::unique_ptr<compute::buffer> vertex_buffer_;
			std::unique_ptr<compute::buffer> vertex_count_buffer_;
			int vertex_count_ = 0;
			int vertex_count2_ = 0;
			bool empty_;
			std::atomic_bool dirty_;
			std::vector<rendering::basic_vertex> vertices_;
			std::vector<rendering::basic_vertex> vertices2_;
			int current_lod_level_ = 0;
			int old_lod_level_ = 0;
			bool build_ = true;
			bool write_data_ = false;
			size_t maximum_vertex_count_ = 0;
			std::array<adjacent_cell, 6> adjacent_cells_;
			std::unique_ptr<compute::buffer> adjacent_cells_buffer_;
			std::atomic_bool loaded_;
			std::string file_name_;
			bool initialized_ = false;
			std::vector<std::unique_ptr<lod_cache_element>> lod_cache_;
			bool build_lod_cache_ = true;
			std::vector<std::vector<rendering::basic_vertex>> lod_cache_vertices_;
			bool switch_lod_cache_ = false;
			lod_cache_element* front_lod_cache_element_ = nullptr;
			lod_cache_element* back_lod_cache_element_ = nullptr;


		public:
			terrain_cell(core::engine& engine, terrain_system& terrain_system, terrain& owner, const glm::ivec3& index, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name);
			~terrain_cell() = default;

			void draw() const override;
			void update_geometry(size_t resolution);
			void prepare_textures() override;

			auto& volume_data() const noexcept {
				return *volume_data_;
			}

			auto& volume_buffer() noexcept {
				return *volume_buffer_;
			}

			auto is_empty() const noexcept {
				return empty_;
			}

			auto is_dirty() const noexcept {
				return dirty_.load();
			}

			auto is_loaded() const noexcept {
				return loaded_.load();
			}

			auto is_initialized() const noexcept {
				return initialized_;
			}

			void lod_level(int level) noexcept {
				assert(level <= 2);
				current_lod_level_ = level;
			}

			void build() noexcept {
				build_ = true;
			}

			auto lod_level() const noexcept {
				return current_lod_level_;
			}

			void draw_normals() const override;

			auto& adjacent_cells() const noexcept {
				return adjacent_cells_;
			}

			void update_adjacent_cells_info() noexcept;
			void load();
			void initialize();

		private:
			cl::Event marching_cubes(const class volume_data& grid, size_t resolution, std::vector<rendering::basic_vertex>& vertices, int& vertex_count);
			void calculate_cell_borders(const class volume_data& grid, size_t resolution);
			void initialize_lod_cache();
		};
	}
}
