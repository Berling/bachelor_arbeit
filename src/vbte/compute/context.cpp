#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include <CL/cl_gl.h>
#include <GL/glew.h>

#ifdef LINUX
#include <GL/glx.h>
#elif WINDOWS
#include <GL/wglext.h>
#endif

#include <vbte/compute/buffer.hpp>
#include <vbte/compute/context.hpp>
#include <vbte/compute/kernel.hpp>
#include <vbte/compute/shared_buffer.hpp>
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
					default_platform.getInfo(static_cast<cl_platform_info>(CL_PLATFORM_VENDOR),
					                         &platform_vendor);
					std::cout << "platform: " << platform_vendor << std::endl;
					cl_context_properties properties[] = {CL_GL_CONTEXT_KHR,
					                                      (cl_context_properties)glXGetCurrentContext(),
					                                      CL_GLX_DISPLAY_KHR,
					                                      (cl_context_properties)glXGetCurrentDisplay(),
					                                      CL_CONTEXT_PLATFORM,
					                                      (cl_context_properties)(default_platform)(),
					                                      0};
					default_context_ = cl::Context{CL_DEVICE_TYPE_GPU, properties};

					const auto& devices = default_context_.getInfo<CL_CONTEXT_DEVICES>();
					default_device_ = devices.at(0);
					auto device_name = default_device_.getInfo<CL_DEVICE_NAME>();
					auto device_version = default_device_.getInfo<CL_DEVICE_VERSION>();
					utils::log << device_name << " " << device_version << std::endl;

					command_queue_ = cl::CommandQueue{default_context_, default_device_};
				} catch (const cl::Error& error) {
					utils::log(utils::log_level::fatal)
					    << error.what() << "(" << error.err() << ")" << std::endl;
				}
			} else {
				try {
					std::vector<cl::Platform> platforms;
					cl::Platform::get(&platforms);

					cl::Platform* default_platform = nullptr;
					for (auto& platform : platforms) {
						std::vector<cl::Device> devices;
						if (platform.getDevices(CL_DEVICE_TYPE_CPU, &devices) != CL_SUCCESS) {
							continue;
						}
						if (!devices.empty()) {
							default_platform = &platform;
							break;
						}
					}

					if (!default_platform) {
						utils::log(utils::log_level::fatal)
						    << "could not find a platform that supports a cpu device" << std::endl;
					}

					std::string platform_vendor;
					default_platform->getInfo(static_cast<cl_platform_info>(CL_PLATFORM_VENDOR),
					                          &platform_vendor);
					utils::log(utils::log_level::info) << "platform: " << platform_vendor << std::endl;
					cl_context_properties properties[3] = {CL_CONTEXT_PLATFORM,
					                                       (cl_context_properties)(*default_platform)(), 0};
					default_context_ = cl::Context{CL_DEVICE_TYPE_CPU, properties};

					const auto& devices = default_context_.getInfo<CL_CONTEXT_DEVICES>();
					default_device_ = devices.at(0);
					auto device_name = default_device_.getInfo<CL_DEVICE_NAME>();
					auto device_version = default_device_.getInfo<CL_DEVICE_VERSION>();
					utils::log << device_name << " " << device_version << std::endl;

					command_queue_ = cl::CommandQueue{default_context_, default_device_};
				} catch (const cl::Error& error) {
					utils::log(utils::log_level::fatal)
					    << error.what() << "(" << error.err() << ")" << std::endl;
				}
			}
		}

		void context::enqueue_write_buffer(buffer& buffer, bool blocking, size_t size,
		                                   const void* data) {
			try {
				command_queue_.enqueueWriteBuffer(buffer.get(), blocking, 0, size, data);
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal)
				    << error.what() << "(" << error.err() << ")" << std::endl;
			}
		}

		cl::Event context::enqueue_read_buffer(buffer& buffer, bool blocking, size_t size, void* data) {
			cl::Event event;
			try {
				command_queue_.enqueueReadBuffer(buffer.get(), blocking, 0, size, data, nullptr, &event);
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal)
				    << error.what() << "(" << error.err() << ")" << std::endl;
			}
			return event;
		}

		void context::enqueue_acquire_gl_buffer(shared_buffer& buffer) {
			try {
				const std::vector<cl::Memory> gl_objects = {buffer.get()};
				command_queue_.enqueueAcquireGLObjects(&gl_objects, nullptr, nullptr);
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal)
				    << error.what() << "(" << error.err() << ")" << std::endl;
			}
		}

		void context::enqueue_release_gl_buffer(shared_buffer& buffer) {
			try {
				const std::vector<cl::Memory> gl_objects = {buffer.get()};
				command_queue_.enqueueReleaseGLObjects(&gl_objects, nullptr, nullptr);
			} catch (const cl::Error& error) {
				utils::log(utils::log_level::fatal)
				    << error.what() << "(" << error.err() << ")" << std::endl;
			}
		}

		cl::Event context::enqueue_kernel(kernel& kernel, const cl::NDRange& global_range,
		                                  const cl::NDRange& local_range, const cl::NDRange& offset) {
			cl::Event event;
			command_queue_.enqueueNDRangeKernel(kernel.get(), offset, global_range, local_range, nullptr,
			                                    &event);
			return event;
		}
	}
}
