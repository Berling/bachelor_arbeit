#ifndef __VBTE_RENDERING_RENDERING_SYSTEM_HPP__
#define __VBTE_RENDERING_RENDERING_SYSTEM_HPP__

#include <glm/glm.hpp>

#include <vbte/graphics/program.hpp>
#include <vbte/graphics/vertex_layout.hpp>

namespace vbte {
	namespace core {
		class engine;
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
		};
	}
}

#endif