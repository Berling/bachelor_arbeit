#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <vbte/compute/context.hpp>
#include <vbte/compute/kernel.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace compute {
		kernel::kernel(context& compute_context, const std::string& name, const std::string& source) {
			auto error = CL_SUCCESS;
			auto& context = compute_context.get();
			auto& device = compute_context.device();
			cl::Program program;
			try {
				auto sources = cl::Program::Sources{1, std::make_pair(source.c_str(), source.length() + 1)};
				program = cl::Program{context, source};
				error = program.build({device});
				kernel_ = cl::Kernel{program, name.c_str()};
			} catch (const cl::Error& ex) {
				if (error != CL_SUCCESS) {
					utils::log(utils::log_level::error) << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device) << std::endl;
					utils::log(utils::log_level::error) << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(device) << std::endl;
					utils::log(utils::log_level::error) << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
				}
				utils::log(utils::log_level::fatal) << ex.what() << "(" << ex.err() << ")" << std::endl;
			}
		}
	}
}
