#include <vector>

#include <vbte/core/engine.hpp>
#include <vbte/rendering/basic_vertex.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/rendering/cube.hpp>

namespace vbte {
	namespace rendering {
		cube::cube(core::engine& engine, const glm::vec3& half_extend)
		: drawable{engine}, vbo_{GL_STATIC_DRAW}, ibo_{GL_STATIC_DRAW} {
			std::vector<basic_vertex> vertices = {
				{{half_extend}},
				{{half_extend * glm::vec3{-1.f, 1.f, 1.f}}},
				{{half_extend * glm::vec3{-1.f, 1.f, -1.f}}},
				{{half_extend * glm::vec3{1.f, 1.f, -1.f}}},
				{{half_extend * glm::vec3{1.f, -1.f, 1.f}}},
				{{half_extend * glm::vec3{-1.f, -1.f, 1.f}}},
				{{half_extend * glm::vec3{-1.f}}},
				{{half_extend * glm::vec3{1.f, -1.f, -1.f}}}
			};
			vbo_.data(sizeof(basic_vertex) * vertices.size(), vertices.data());

			std::vector<unsigned int> indices = {
				0, 3, 1,
				3, 2, 1,
				4, 5, 6,
				4, 6, 7,
				0, 1, 5,
				5, 4, 0,
				2, 3, 7,
				7, 6, 2,
				1, 2, 6,
				6, 5, 1,
				3, 0, 4,
				4, 7, 3
			};
			ibo_.data(sizeof(unsigned int) * indices.size(), indices.data());

			vao_.bind_index_buffer(ibo_);
			engine_.rendering_system().basic_layout().setup_layout(vao_, &vbo_);
		}

		void cube::draw() const {
			vao_.bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		}
	}
}