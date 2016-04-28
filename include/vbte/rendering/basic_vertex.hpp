#ifndef __VBTE_RENDRING_BASIC_VERTEX_HPP__
#define __VBTE_RENDRING_BASIC_VERTEX_HPP__

#include <glm/glm.hpp>

namespace vbte {
	namespace rendering {
		struct basic_vertex {
			glm::vec4 position;
			glm::vec4 normal;
		};
	}
}

#endif
