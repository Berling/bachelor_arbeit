#ifndef __VBTE_GRAPHICS_BUFFER_HPP__
#define __VBTE_GRAPHICS_BUFFER_HPP__

#include <stdexcept>

#include <GL/glew.h>

namespace vbte {
	namespace graphics {
		template <GLenum target>
		class buffer {
		protected:
			GLuint id_ = 0;
			GLenum usage_;

		public:
			buffer(GLenum usage) noexcept
			: usage_(usage) {
				glGenBuffers(1, &id_);
				if (id_ == 0) {
					throw std::runtime_error{"no buffer created"};
				}
				bind();
			}

			buffer(size_t size, const void* data, GLenum usage) noexcept
			: usage_{usage} {
				glGenBuffers(1, &id_);
				bind();
				glBufferData(target, size, data, usage_);
			}

			~buffer() {
				glDeleteBuffers(1, &id_);
			}

			buffer(const buffer&) = delete;
			buffer& operator=(const buffer&) = delete;

			buffer(buffer&& rhs) noexcept
			: id_(rhs.id_), usage_(rhs.usage_) {
				rhs.id_ = 0;
			}

			buffer& operator=(buffer&& rhs) noexcept {
				id_ = rhs.id_;
				usage_ = rhs.usage_;
				rhs.id_ = 0;
				return *this;
			}

			void data(size_t size, const void* data_ptr) noexcept {
				bind();
				data(size, data_ptr, usage_);
			}

			void data(size_t size, const void* data_ptr, GLenum usage) noexcept {
				usage_ = usage;
				bind();
				glBufferData(target, size, data_ptr, usage);
			}

			void sub_data(intptr_t offset, size_t size, const void* data) noexcept {
				bind();
				glBufferSubData(target, offset, size, data);
			}

			void bind() const noexcept {
				bind(target);
			}

			void bind(GLenum bind_target) const noexcept {
				glBindBuffer(bind_target, id_);
			}
		};

		using vertex_buffer = buffer<GL_ARRAY_BUFFER>;
		using index_buffer = buffer<GL_ELEMENT_ARRAY_BUFFER>;
	}
}

#endif
