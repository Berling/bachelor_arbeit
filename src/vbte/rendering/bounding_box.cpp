#include <vector>

#include <vbte/core/engine.hpp>
#include <vbte/rendering/basic_vertex.hpp>
#include <vbte/rendering/bounding_box.hpp>
#include <vbte/rendering/rendering_system.hpp>

namespace vbte {
	namespace rendering {
		bounding_box::bounding_box(core::engine& engine)
		: drawable{engine, glm::vec3{0.f}, glm::angleAxis(0.f, glm::vec3(0.f))}, vbo_{GL_STATIC_DRAW}, ibo_{GL_STATIC_DRAW} {
			auto zero = glm::vec4{0.f};
			std::vector<basic_vertex> vertices = {
				{glm::vec4{-0.5f, -0.5f, 0.5f, 0.f}, zero},
				{glm::vec4{0.5f, -0.5f, 0.5f, 0.f}, zero},
				{glm::vec4{-0.5f, 0.5f, 0.5f, 0.f}, zero},
				{glm::vec4{0.5f, 0.5f, 0.5f, 0.f}, zero},

				{glm::vec4{-0.5f, -0.5f, -0.5f, 0.f}, zero},
				{glm::vec4{0.5f, -0.5f, -0.5f, 0.f}, zero},
				{glm::vec4{-0.5f, 0.5f, -0.5f, 0.f}, zero},
				{glm::vec4{0.5f, 0.5f, -0.5f, 0.f}, zero}
			};
			vbo_.data(sizeof(basic_vertex) * vertices.size(), vertices.data());

			std::vector<unsigned int> indices = {
				0, 1,
				2, 3,
				0, 2,
				1, 3,
				4, 5,
				6, 7,
				4, 6,
				5, 7,
				0, 4,
				1, 5,
				2, 6,
				3, 7
			};
			ibo_.data(sizeof(unsigned int) * indices.size(), indices.data());

			vao_.bind_index_buffer(ibo_);

			engine_.rendering_system().basic_layout().setup_layout(vao_, &vbo_);
		}

		void bounding_box::draw() const {
			vao_.bind();
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
		}
	}
}
