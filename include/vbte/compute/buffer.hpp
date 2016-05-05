#pragma once

#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <memory>

#include <CL/cl.hpp>

#include <vbte/compute/context.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace compute {
		class buffer {
		protected:
			std::unique_ptr<cl::Buffer> buffer_;
			context& compute_context_;
			cl_mem_flags flags_;
			size_t size_;

		public:
			buffer(context& compute_context, cl_mem_flags flags, size_t size);
			~buffer() = default;

			buffer(const buffer&) = delete;
			buffer& operator=(const buffer&) = delete;

			buffer(buffer&&) = default;
			buffer& operator=(buffer&&) = default;

			auto& get() noexcept {
				return *buffer_;
			}

			auto& get() const noexcept {
				return *buffer_;
			}

			auto size() const noexcept {
				return size_;
			}

			void resize(size_t size) {
				if (size_ != size) {
					size_ = size;
					try {
						buffer_ = std::make_unique<cl::Buffer>(compute_context_.get(), flags_, size_);
					} catch (const cl::Error& error) {
						utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
					}
				}
			}
		};
	}
}
