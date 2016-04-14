#include <vbte/graphics/transform_feedback_buffer.hpp>
#include <vbte/graphics/program.hpp>

namespace vbte {
	namespace graphics {
		transform_feedback_buffer::transform_feedback_buffer(GLenum usage) :
		buffer<GL_TRANSFORM_FEEDBACK_BUFFER>{usage} {}

		transform_feedback_buffer::transform_feedback_buffer(size_t size, const void* data, GLenum usage) :
		buffer<GL_TRANSFORM_FEEDBACK_BUFFER>(size, data, usage) {}

		void transform_feedback_buffer::bind_range(GLuint index, GLintptr offset, GLsizeiptr size) {
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, id_);
		}

		void transform_feedback_buffer::begin(GLenum primitive_mode) {
			glBeginTransformFeedback(primitive_mode);
		}

		void transform_feedback_buffer::end() {
			glEndTransformFeedback();
		}
	}
}
