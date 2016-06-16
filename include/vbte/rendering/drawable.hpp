#ifndef __VBTE_RENDERING_DRAWABLE_HPP__
#define __VBTE_RENDERING_DRAWABLE_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace rendering {
		class drawable {
		protected:
			core::engine& engine_;
			glm::vec3 position_;
			glm::quat rotation_;

		public:
			drawable(core::engine& engine, const glm::vec3& position, const glm::quat& rotation) noexcept
			: engine_{engine}, position_{position}, rotation_{rotation} {}
			virtual ~drawable() = default;

			drawable(const drawable&) = delete;
			drawable& operator=(const drawable&) = delete;

			drawable(drawable&&) = default;
			drawable& operator=(drawable&&) = default;

			virtual void draw() const = 0;
			virtual void draw_normals() const {}

			auto& position() const noexcept {
				return position_;
			}

			void position(const glm::vec3& position) noexcept {
				position_ = position;
			}

			auto& rotation() const noexcept {
				return rotation_;
			}

			void rotation(const glm::quat& rotation) noexcept {
				rotation_ = rotation;
			}

			auto transform() const noexcept {
				auto transformation = glm::toMat4(rotation_);
				transformation[3].x = position_.x;
				transformation[3].y = position_.y;
				transformation[3].z = position_.z;
				return transformation;
			}

			virtual void prepare_textures() {}
		};
	}
}

#endif
