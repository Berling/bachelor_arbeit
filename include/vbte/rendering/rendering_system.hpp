#ifndef __VBTE_RENDERING_RENDERING_SYSTEM_HPP__
#define __VBTE_RENDERING_RENDERING_SYSTEM_HPP__

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vbte/graphics/program.hpp>
#include <vbte/graphics/vertex_layout.hpp>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace rendering {
		class drawable;

		enum class rendering_mode {
			wireframe,
			wireframe_filled,
			solid,
			shaded
		};

		struct bounding_box_uniforms {
			glm::vec3 half_extend;
			glm::vec3 position;
			glm::quat rotation;
		};
	}
}

namespace vbte {
	namespace rendering {
		class rendering_system {
		private:
			core::engine& engine_;
			graphics::vertex_layout basic_layout_;
			graphics::program debug_program_;
			glm::vec3 debug_face_color_;
			glm::vec3 debug_edge_color_;
			rendering_mode mode_;
			graphics::program light_program_;
			std::vector<drawable*> draw_queue_;
			std::vector<bounding_box_uniforms> bounding_box_draw_queue_;
			graphics::program skydome_program_;
			graphics::vertex_layout layout_;
			bool draw_normals_ = false;

		public:
			rendering_system(core::engine& engine);
			~rendering_system() noexcept;

			rendering_system(const rendering_system&) = delete;
			rendering_system& operator=(const rendering_system&) = delete;

			rendering_system(rendering_system&&) = default;
			rendering_system& operator=(rendering_system&&) = default;

			void update(float delta_time);

			auto& basic_layout() noexcept {
				return basic_layout_;
			}

			auto& layout() noexcept {
				return layout_;
			}

			void change_mode(rendering_mode mode) noexcept {
				mode_ = mode;
			}

			auto& skydome_program() noexcept {
				return skydome_program_;
			}

			void toggle_normals() noexcept {
				draw_normals_ = !draw_normals_;
			}

			auto& light_program() noexcept {
				return light_program_;
			}

			void draw(drawable* geometry);
			void draw_bounding_box(const glm::vec3& half_extend, const glm::vec3& position, const glm::quat& rotation);
		};
	}
}

#endif
