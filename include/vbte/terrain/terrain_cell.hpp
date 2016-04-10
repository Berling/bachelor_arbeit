#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include <vbte/graphics/buffer.hpp>
#include <vbte/graphics/vertex_array.hpp>
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
			std::shared_ptr<const volume_data> volume_data_;
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			size_t index_count_;

		public:
			terrain_cell(core::engine& engine, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name);
			~terrain_cell() = default;

			void draw() const override;

			auto& volume_data() const noexcept {
				return *volume_data_;
			}
		};
	}
}
