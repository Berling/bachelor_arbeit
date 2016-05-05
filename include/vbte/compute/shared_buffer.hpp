#pragma once

#include <memory>

#include <CL/cl.hpp>

#include <vbte/graphics/buffer.hpp>

namespace vbte {
	namespace compute {
		class context;
	}
}

namespace vbte {
	namespace compute {
		class shared_buffer : public graphics::vertex_buffer {
		private:
			std::unique_ptr<cl::BufferGL> shared_buffer_;

		public:
			shared_buffer(context& compute_context, cl_mem_flags flags, size_t size, const void* data, GLenum usage);
			~shared_buffer() = default;

			shared_buffer(const shared_buffer&) = delete;
			shared_buffer& operator=(const shared_buffer&) = delete;

			shared_buffer(shared_buffer&&) = default;
			shared_buffer& operator=(shared_buffer&&) = default;

			auto& get() noexcept {
				return *shared_buffer_;
			}

			auto& get() const noexcept {
				return *shared_buffer_;
			}
		};
	}
}
