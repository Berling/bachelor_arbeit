#pragma once

#include <CL/cl.hpp>

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

			auto& get() noexcept {
				return default_context_;
			}

			auto& device() noexcept {
				return default_device_;
			}

			auto& command_queue() noexcept {
				return command_queue_;
			}
		};
	}
}
