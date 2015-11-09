#ifndef __VBTE_RENDERING_CUBE_HPP__
#define __VBTE_RENDERING_CUBE_HPP__

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
		class cube : public drawable {
		private:
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			graphics::index_buffer ibo_;

		public:
			cube(core::engine& engine, const glm::vec3& half_extend);
			~cube() = default;

			void draw() const override;
		};
	}
}

#endif