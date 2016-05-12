#include <vector>

#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/graphics/graphics_system.hpp>
#include <vbte/graphics/texture.hpp>
#include <vbte/graphics/texture_manager.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/rendering/skydome.hpp>
#include <vbte/rendering/vertex.hpp>

namespace vbte {
	namespace rendering {
		skydome::skydome(core::engine& engine, const glm::vec3& position, float radius)
		: drawable{engine, position, glm::angleAxis(glm::radians(-90.f), glm::vec3{1.f, 0.f, 0.f})}, vbo_{GL_STATIC_DRAW}, ibo_{GL_STATIC_DRAW} {
			auto max_phi = 32;
			auto max_theta = 16;
			std::vector<vertex> vertices;
			for (auto theta = 0; theta <= max_theta; ++theta) {
				for (auto phi = 0; phi <= max_phi; ++phi) {
					auto angle_phi = float(phi) / float(max_phi) * (2.f * glm::pi<float>());
					auto angle_theta = float(theta) / float(max_theta) * glm::pi<float>();
					auto position = glm::vec3{
						radius * glm::cos(angle_theta) * glm::cos(angle_phi),
						radius * glm::cos(angle_theta) * glm::sin(angle_phi),
						radius * glm::sin(angle_theta)
					};
					auto texcoord = glm::vec2{
						0.5f * (position.x / radius) + 0.5f,
						0.5f * (position.y / radius) + 0.5f
					};
					texcoord = 0.98f * texcoord + glm::vec2{0.01f};
					auto normal = -glm::normalize(position);
					vertices.emplace_back(vertex{position, texcoord, normal});
				}
			}
			vbo_.data(sizeof(vertex) * vertices.size(), vertices.data());

			std::vector<unsigned int> indices;
			for (auto theta = 0; theta < max_theta; ++theta) {
				for (auto phi = 0; phi < max_phi; ++phi) {
					auto first = (theta * (max_phi + 1)) + phi;
					auto second = first + max_phi + 1;

					indices.emplace_back(first);
					indices.emplace_back(second);
					indices.emplace_back(first + 1);

					indices.emplace_back(second);
					indices.emplace_back(second + 1);
					indices.emplace_back(first + 1);
				}
			}
			ibo_.data(sizeof(unsigned int) * indices.size(), indices.data());
			index_count_ = indices.size();

			vao_.bind_index_buffer(ibo_);
			engine_.rendering_system().layout().setup_layout(vao_, &vbo_);

			sky_ = engine_.graphics_system().texture_manager().load("textures/environment/skydome.dds");
			if (!sky_) {
				throw std::runtime_error{"could not load textures/environment/skydome.dds"};
			}
		}

		void skydome::draw() const {
			auto& camera = engine_.camera();
			auto& program = engine_.rendering_system().skydome_program();
			program.use();
			program.uniform("projection", false, camera.projection());
			program.uniform("view", false, camera.view());
			program.uniform("model", false, transform());
			program.uniform("sky", 0);
			sky_->bind(0);
			vao_.bind();
			glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
		}
	}
}
