#ifndef __VBTE_UTILS_STACKTRACE_HPP__
#define __VBTE_UTILS_STACKTRACE_HPP__


#include <string>
#include <stdexcept>

#ifdef STACKTRACE

namespace vbte {
	namespace utils {
		extern void initStacktrace(std::string exeName);

		extern bool isStacktraceAvailable();

		extern std::string genStacktrace(std::size_t framesToSkip=0);

		struct Error : public std::runtime_error {
			explicit Error(const std::string& msg)
					: std::runtime_error(msg+"\n At "+genStacktrace(1)) {}
		};
	}
}

#endif
#endif
