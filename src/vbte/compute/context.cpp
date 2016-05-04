#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <CL/cl_gl.h>

#ifdef LINUX
	#include <GL/glx.h>
#elif WINDOWS
	#include <GL/wgl.h>
#endif

#include <vbte/compute/context.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace compute {
		context::context(bool shared) {
			if (shared) {
				try {
					std::vector<cl::Platform> platforms;
					cl::Platform::get(&platforms);
					auto& default_platform = platforms.at(0);
					std::string platform_vendor;
					default_platform.getInfo(static_cast<cl_platform_info>(CL_PLATFORM_VENDOR), &platform_vendor);
#ifdef LINUX
					cl_context_properties properties[] = {
						CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
						CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
						CL_CONTEXT_PLATFORM, (cl_context_properties)(default_platform)(),
						0
					};
#elif WINDOWS
					cl_context_properties properties[] = {
						CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
						CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
						CL_CONTEXT_PLATFORM, (cl_context_properties)(default_platform)(),
						0
					};
#endif
					default_context_ = cl::Context{CL_DEVICE_TYPE_GPU, properties};

					const auto& devices = default_context_.getInfo<CL_CONTEXT_DEVICES>();
					default_device_ = devices.at(0);
					auto device_name = default_device_.getInfo<CL_DEVICE_NAME>();
					auto device_version = default_device_.getInfo<CL_DEVICE_VERSION>();
					utils::log << device_name << " " << device_version << std::endl;

					command_queue_ = cl::CommandQueue{default_context_, default_device_};
				} catch (const cl::Error& error) {
					utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
				}
			} else {
				try {
					std::vector<cl::Platform> platforms;
					cl::Platform::get(&platforms);
					auto& default_platform = platforms.at(0);
					std::string platform_vendor;
					default_platform.getInfo(static_cast<cl_platform_info>(CL_PLATFORM_VENDOR), &platform_vendor);

					cl_context_properties properties[3] = {
						CL_CONTEXT_PLATFORM,
						(cl_context_properties)(default_platform)(),
						0
					};
					default_context_ = cl::Context{CL_DEVICE_TYPE_GPU, properties};

					const auto& devices = default_context_.getInfo<CL_CONTEXT_DEVICES>();
					default_device_ = devices.at(0);
					auto device_name = default_device_.getInfo<CL_DEVICE_NAME>();
					auto device_version = default_device_.getInfo<CL_DEVICE_VERSION>();
					utils::log << device_name << " " << device_version << std::endl;

					command_queue_ = cl::CommandQueue{default_context_, default_device_};
				} catch (const cl::Error& error) {
					utils::log(utils::log_level::fatal) << error.what() << "(" << error.err() << ")" << std::endl;
				}
			}
		}
	}
}
