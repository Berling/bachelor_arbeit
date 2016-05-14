#pragma once

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
	}
}

namespace vbte {
	namespace terrain {
		class terrain_cell : public rendering::drawable {
		private:
			std::shared_ptr<const terrain::volume_data> volume_data_;
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			size_t index_count_;
			std::unique_ptr<compute::buffer> volume_buffer_;
			std::unique_ptr<compute::buffer> vertex_buffer_;
			std::unique_ptr<compute::buffer> vertex_count_buffer_;
			int vertex_count_;

		public:
			terrain_cell(core::engine& engine, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name);
			~terrain_cell() = default;

			void draw() const override;
			void update_geometry();

			auto& volume_data() const noexcept {
				return *volume_data_;
			}

		private:
			std::vector<rendering::basic_vertex> marching_cubes(const terrain::volume_data& grid, size_t resolution);
		};
	}
}
