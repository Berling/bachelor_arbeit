#include <glm/gtc/matrix_transform.hpp>

#include <vbte/core/camera.hpp>
#include <vbte/core/engine.hpp>
#include <vbte/core/window.hpp>
#include <vbte/graphics/graphics_system.hpp>

namespace vbte {
	namespace core {
		camera::camera(core::engine& engine, const glm::vec3& position, float fovy, float z_near, float z_far) noexcept
		: engine_{engine}, position_{position}, rotation_{glm::angleAxis(glm::radians(0.f), glm::vec3{0.f, 1.f, 0.f})},
		move_forward_{false}, move_backward_{false}, move_left_{false}, move_right_{false}, elevation_{glm::radians(0.f)}, azimuth_{glm::radians(-90.f)} {
			auto& window = engine_.graphics_system().window();
			auto aspect = static_cast<float>(window.width()) / static_cast<float>(window.height());
			projection_ = glm::perspective(fovy, aspect, z_near, z_far);
		}

		void camera::update(float delta_time) {
			static auto forward_direction = glm::vec3{0.f, 0.f, -1.f};
			static auto right_direction = glm::vec3{1.f, 0.f, 0.f};

			auto fd = glm::rotate(rotation_, forward_direction);
			auto rd = glm::rotate(rotation_, right_direction);

			auto direction = glm::vec3{0.f};
			if (move_forward_) {
				direction += fd;
			} else if (move_backward_) {
				direction -= fd;
			}

			if (move_right_) {
				direction += rd;
			} else if (move_left_) {
				direction -= rd;
			}

			if (glm::zero<glm::vec3>() != direction) {
				direction = glm::normalize(direction);
			}

			static auto velocity = 10.f;
			position_ += direction * velocity * delta_time;

			auto r = 10.f;
			auto camera_pos = glm::vec3{
				r * glm::cos(elevation_) * glm::cos(azimuth_),
				r * glm::sin(elevation_),
				r * glm::cos(elevation_) * glm::sin(azimuth_)
			};
			camera_pos += position_;
			auto view = glm::lookAt(position_, camera_pos, glm::vec3{0.f, 1.f, 0.f});
			auto rot = glm::quat_cast(view);
			rotation_ = glm::inverse(rot);
		}
	}
}