#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <GL/glew.h>

#include <vbte/compute/context.hpp>
#include <vbte/compute/shared_buffer.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace compute {
		shared_buffer::shared_buffer(context& compute_context, cl_mem_flags flags, size_t size, const void* data, GLenum usage) : buffer{size, data, usage} {
			try {
				shared_buffer_ = std::make_unique<cl::BufferGL>(compute_context.get(), flags, id_, nullptr);
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
			}
		}
	}
}
