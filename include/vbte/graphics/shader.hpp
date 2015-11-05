#ifndef __VBTE_GRAPHICS_SHADER_HPP__
#define __VBTE_GRAPHICS_SHADER_HPP__

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

namespace vbte {
	namespace graphics {
		class shader {
		private:
			GLenum type_;
			GLuint id_;

			friend class program;

		public:
			shader(const std::string& name, GLenum type, const std::string& source);
			~shader();

			shader(const shader&) = delete;
			shader& operator=(const shader&) = delete;

			shader(shader&& rhs) noexcept;
			shader& operator&(shader&& rhs) noexcept;
		};
	}
}

#endif
