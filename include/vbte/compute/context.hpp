#pragma once

#include <CL/cl.hpp>
#include <GL/glew.h>

namespace vbte {
	namespace compute {
		class buffer;
		class kernel;
		class shared_buffer;
	}
}

namespace vbte {
	namespace compute {
		class context {
		private:
			cl::Context default_context_;
			cl::Device default_device_;
			cl::CommandQueue command_queue_;

		public:
			context(bool shared = false);
			~context() = default;

			context(const context&) = delete;
			context& operator=(const context&) = delete;

			context(context&&) = delete;
			context& operator=(context&&) = delete;

			void enqueue_write_buffer(buffer& buffer, bool blocking, size_t size, const void* data);
			void enqueue_read_buffer(buffer& buffer, bool blocking, size_t size, void* data);
			void enqueue_acquire_gl_buffer(shared_buffer& buffer);
			void enqueue_release_gl_buffer(shared_buffer& buffer);
			cl::Event enqueue_kernel(kernel& kernel, const cl::NDRange& global_range, const cl::NDRange& local_range);

			auto& get() noexcept {
				return default_context_;
			}

			auto& device() noexcept {
				return default_device_;
			}

			auto& command_queue() noexcept {
				return command_queue_;
			}

			void begin() const noexcept {
				glFinish();
			}

			void end() const noexcept {
				command_queue_.finish();
			}
		};
	}
}
