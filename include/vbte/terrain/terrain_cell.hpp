#pragma once

#include <string>

#include <glm/glm.hpp>

#include <vbte/graphics/buffer.hpp>
#include <vbte/graphics/vertex_array.hpp>
#include <vbte/rendering/drawable.hpp>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace terrain {
		class terrain_cell : public rendering::drawable {
		private:
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			size_t index_count;

		public:
			terrain_cell(core::engine& engine, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name);
			~terrain_cell() = default;

			void draw() const override;
		};
	}
}
