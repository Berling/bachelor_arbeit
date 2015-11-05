#ifndef __VBTE_UTILS_LOGGER_HPP__
#define __VBTE_UTILS_LOGGER_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace vbte {
	namespace utils {
		enum class log_level {
			info,
			debug,
			warning,
			error,
			fatal
		};
	}
}

namespace vbte {
	namespace utils {
		class logger {
		private:
			std::ofstream os_;
			log_level level_;
			bool new_line_;

		public:
			logger();
			~logger();

			logger(const logger&) = delete;
			logger& operator=(const logger&) = delete;

			logger(logger&&) = delete;
			logger& operator=(logger&&) = delete;

			std::string format_time(const std::string& format);

			void operator()(const std::string& filename);
			logger& operator()(log_level level) noexcept;
			logger& operator<<(std::ostream& (*ostream_function)(std::ostream&));

			template <typename type>
			logger& operator<<(const type& rhs) {
				std::stringstream ss;

				if (new_line_) {
					ss << format_time("[%Y-%m-%d %X]");

					switch(level_) {
						case log_level::info:
							ss << "[INFO] ";
							break;
						case log_level::debug:
							ss << "[DEBUG] ";
							break;
						case log_level::warning:
							ss << "[WARNING] ";
							break;
						case log_level::error:
							ss << "[ERROR] ";
							break;
						case log_level::fatal:
							ss << "[FATAL] ";
							break;
					}

					new_line_ = false;
				}

				ss << rhs;

				os_ << ss.str();
				std::cout << ss.str();

				return *this;
			}
		};

		extern logger log;

		inline void logger_assert(bool value, const char* expression, const char* file, size_t line) {
			if(!value) {
				log(log_level::fatal) << "assertion failed: \"" << expression << "\"" << " at " << file << ":" << line << std::endl;
			}
		}
	}
}

#define LOGGER_ASSERT(EXPRESSION) \
	gdw::logger_assert(bool(EXPRESSION), #EXPRESSION, __FILE__, __LINE__);

#endif
