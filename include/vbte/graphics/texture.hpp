#ifndef __VBTE_GRAPHICS_TEXTURE_HPP__
#define __VBTE_GRAPHICS_TEXTURE_HPP__

#include <GL/glew.h>
#include <gli/core/texture2d.hpp>

namespace vbte {
	namespace graphics {
		class texture {
		private:
			GLuint id_;
			GLenum target_;
			size_t width_;
			size_t height_;

			friend class framebuffer;

		public:
			texture(const gli::texture2D& texture) noexcept;
			texture(GLenum target, GLint internal_format, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data = nullptr) noexcept;
			~texture();

			texture(const texture&) = delete;
			texture& operator=(const texture&) = delete;

			texture(texture&& rhs) noexcept;
			texture& operator=(texture&& rhs) noexcept;

			void bind(uint32_t unit) const noexcept;
			void apply_settings() const noexcept;

			size_t width() const noexcept {
				return width_;
			}

			size_t height() const noexcept {
				return height_;
			}

		};
	}
}

#endif
