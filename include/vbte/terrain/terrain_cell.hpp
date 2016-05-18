#pragma once

#include <cassert>
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
		class terrain_system;
	}
}

namespace vbte {
	namespace terrain {
		class terrain_cell : public rendering::drawable {
		private:
			terrain_system& terrain_system_;
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
			int current_lod_level_ = 0;
			bool build_ = true;
			bool write_data_ = false;
			size_t maximum_vertex_count_ = 0;

		public:
			terrain_cell(core::engine& engine, terrain_system& terrain_system, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name);
			~terrain_cell() = default;

			void draw() const override;
			void update_geometry(size_t resolution);

			auto& volume_data() const noexcept {
				return *volume_data_;
			}

			auto is_empty() const noexcept {
				return empty_;
			}

			auto is_dirty() const noexcept {
				return dirty_.load();
			}

			void lod_level(int level) noexcept {
				assert(level <= 2);
				if (current_lod_level_ != level) {
					build_ = true;
				}
				current_lod_level_ = level;
			}

			auto lod_level() const noexcept {
				return current_lod_level_;
			}

		private:
			void marching_cubes(const class volume_data& grid, size_t resolution);
		};
	}
}
