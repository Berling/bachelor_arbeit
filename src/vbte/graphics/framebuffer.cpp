#include <vbte/graphics/framebuffer.hpp>

namespace vbte {
	namespace graphics {
		framebuffer::framebuffer() noexcept {
			glGenFramebuffers(1, &id_);
		}

		framebuffer::~framebuffer() {
			glDeleteFramebuffers(1, &id_);
		}

		framebuffer::framebuffer(framebuffer&& rhs) noexcept
		: id_(rhs.id_), attachments_(std::move(rhs.attachments_)) {
			rhs.id_ = 0;
		}

		framebuffer& framebuffer::operator=(framebuffer&& rhs) noexcept {
			id_ = rhs.id_;
			attachments_ = std::move(rhs.attachments_);
			rhs.id_ = 0;
			return *this;
		}

		void framebuffer::bind() const noexcept {
			glBindFramebuffer(GL_FRAMEBUFFER, id_);
		}

		void framebuffer::bind_read() const noexcept {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id_);
		}

		void framebuffer::bind_draw() const noexcept {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id_);
		}

		texture& framebuffer::attachment(GLenum attachment) const {
			return *attachments_.at(attachment);
		}

		void framebuffer::bind_default() noexcept {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}
