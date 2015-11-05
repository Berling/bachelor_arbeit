#include <chrono>
#include <ctime>
#include <stdexcept>
#include <utility>

#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace utils {
		logger log;

		logger::logger()
		: level_(log_level::info), new_line_(true) {}

		logger::~logger() {
			if (os_.is_open()) {
				os_.close();
			}
		}

		std::string logger::format_time(const std::string& format) {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			char buffer[100];

			auto time_info = localtime(&in_time_t);

			std::strftime(buffer, 100, format.c_str(), time_info);

			return std::string(buffer);
		}

		void logger::operator()(const std::string& filename) {
			os_.open(filename);
			if (!os_.is_open()) {
				throw std::runtime_error("could not open logfile: " + filename);
			}
		}

		logger& logger::operator()(log_level level) noexcept {
			this->level_ = level;
			return *this;
		}

		logger& logger::operator<<(std::ostream& (*ostream_function)(std::ostream&)) {
			ostream_function(os_);
			ostream_function(std::cout);

			new_line_ = true;

			if (level_ == log_level::fatal) {
				std::exit(EXIT_FAILURE);
			}

			level_ = log_level::info;

			return *this;
		}
	}
}
