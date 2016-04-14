#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <vbte/graphics/buffer.hpp>

namespace vbte {
	namespace graphics {
		class program;
	}
}

namespace vbte {
	namespace graphics {
		class transform_feedback_buffer : public buffer<GL_TRANSFORM_FEEDBACK_BUFFER> {
		public:
			transform_feedback_buffer(GLenum usage);
			transform_feedback_buffer(size_t size, const void* data, GLenum usage);

			void bind_range(GLuint index, GLintptr offset, GLsizeiptr size);
			void begin(GLenum primitive_mode);
			void end();

			template <typename t>
			t* map_read() {
				auto ptr = reinterpret_cast<t*>(glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY));
				if (!ptr) {
					throw std::runtime_error{"could not map transform feedback buffer " + std::to_string(id_)};
				}
				return ptr;
			}

			void unmap() {
				glUnmapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
			}
		};
	}
}
