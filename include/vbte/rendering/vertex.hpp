#pragma once

#include <glm/glm.hpp>

namespace vbte {
	namespace rendering {
		struct vertex {
			glm::vec3 position;
			glm::vec2 texcoord;
			glm::vec3 normal;
		};
	}
}
