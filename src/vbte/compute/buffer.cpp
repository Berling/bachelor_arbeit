#include <vbte/compute/buffer.hpp>

namespace vbte {
	namespace compute {
		buffer::buffer(context& compute_context, cl_mem_flags flags, size_t size) : compute_context_{compute_context},
																																								flags_{flags},
																																								size_{size} {
			try {
				buffer_ = std::make_unique<cl::Buffer>(compute_context_.get(), flags_, size_);
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
			}
		}
	}
}
