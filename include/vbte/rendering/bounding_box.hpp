#pragma once

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
	namespace rendering {
		class bounding_box : public drawable {
		private:
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			graphics::index_buffer ibo_;

		public:
			bounding_box(core::engine& engine);
			~bounding_box() = default;

			void draw() const override;
		};
	}
}
