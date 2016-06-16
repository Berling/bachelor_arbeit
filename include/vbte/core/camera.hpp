#ifndef __VBTE_CORE_CAMERA_HPP__
#define __VBTE_CORE_CAMERA_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace core {
		class camera {
		private:
			core::engine& engine_;
			glm::vec3 position_;
			glm::quat rotation_;
			glm::mat4 projection_;
			bool move_forward_;
			bool move_backward_;
			bool move_left_;
			bool move_right_;
			float elevation_;
			float azimuth_;

		public:
			camera(core::engine& engine, const glm::vec3& position, float fovy, float z_near, float z_far) noexcept;
			~camera() noexcept = default;

			camera(const camera&) = default;
			camera& operator= (const camera&) = default;

			camera(camera&&) = default;
			camera& operator=(camera&&) = default;

			void update(float delta_time);

			auto projection() const noexcept {
				return projection_;
			}

			auto view() const noexcept {
				auto view = glm::toMat4(rotation_);
				view[3].x = position_.x;
				view[3].y = position_.y;
				view[3].z = position_.z;
				return glm::inverse(view);
			}

			auto projection_view() const noexcept {
				return projection() * view();
			}

			void move_forward(bool move) noexcept {
				move_forward_ = move;
			}

			void move_backward(bool move) noexcept {
				move_backward_ = move;
			}

			void move_left(bool move) noexcept {
				move_left_ = move;
			}

			void move_right(bool move) noexcept {
				move_right_ = move;
			}

			void azimuth(float azimuth) noexcept {
				azimuth_ = azimuth;
			}

			auto azimuth() const noexcept {
				return azimuth_;
			}

			void elevation(float elevation) noexcept {
				elevation_ = elevation;
			}

			auto elevation() const noexcept {
				return elevation_;
			}

			auto& position() const noexcept {
				return position_;
			}
		};
	}
}

#endif
