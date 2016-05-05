#pragma once

#include <memory>
#include <string>

#include <CL/cl.hpp>

namespace vbte {
	namespace compute {
		class buffer;
		class context;
		class shared_buffer;
	}
}

namespace vbte {
	namespace compute {
		class kernel {
		private:
			cl::Kernel kernel_;

		public:
			kernel(context& compute_context, const std::string& name, const std::string& source);
			~kernel() = default;

			kernel(const kernel&) = default;
			kernel& operator=(const kernel&) = default;

			kernel(kernel&&) = default;
			kernel& operator=(kernel&&) = default;

			template <typename t>
			void arg(size_t index, const t& argument) {
				kernel_.setArg(index, argument);
			}

			auto& get() noexcept {
				return kernel_;
			}
		};

		template <>
		void kernel::arg<buffer>(size_t index, const buffer& argument);

		template <>
		void kernel::arg<shared_buffer>(size_t index, const shared_buffer& argument);
	}
}
