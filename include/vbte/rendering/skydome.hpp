#pragma once

#include <glm/glm.hpp>

#include <vbte/graphics/buffer.hpp>
#include <vbte/graphics/vertex_array.hpp>
#include <vbte/rendering/drawable.hpp>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace graphics {
		class texture;
	}
}

namespace vbte {
	namespace rendering {
		class skydome : public drawable {
		private:
			graphics::vertex_array vao_;
			graphics::vertex_buffer vbo_;
			graphics::index_buffer ibo_;
			int index_count_;
			std::shared_ptr<const graphics::texture> sky_;

		public:
			skydome(core::engine& engine, const glm::vec3& position, float radius);
			~skydome() = default;

			void draw() const override;
		};
	}
}
