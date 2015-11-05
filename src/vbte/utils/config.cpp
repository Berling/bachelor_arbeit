#include <cstdlib>
#include <fstream>
#include <sstream>

#include <vbte/core/engine.hpp>
#include <vbte/utils/config.hpp>

namespace vbte {
	namespace utils {
		config::config(core::engine& engine, const std::string& name, const std::string& data)
		: engine_(engine), name_(name) {
			std::istringstream ss(data);
			std::string line;
			auto line_number = 1;
			while (std::getline(ss, line)) {
				auto separator_position = line.find(":");
				if (separator_position == std::string::npos) {
					throw std::runtime_error("missing separator in " + name_ + ":" + std::to_string(line_number));
				}

				auto key = line.substr(0, separator_position - 1);
				if (key.empty()) {
					throw std::runtime_error("missing key before separator in " + name_ + ":" + std::to_string(line_number));
				}

				auto value = line.substr(separator_position + 2);
				if (value.empty()) {
					throw std::runtime_error("missing value after separator in " + name_ + ":" + std::to_string(line_number));
				}

				values_.emplace(std::make_pair(key, value));

				++line_number;
			}
		}

		void config::write() const {
			auto full_name = engine_.asset_manager().native_name(name_);
			std::ofstream of(full_name, std::ios_base::trunc);
			if (of) {
				for (auto& value : values_) {
					of << value.first << " : " << value.second << std::endl;
				}
				of.close();
			} else {
				throw std::runtime_error("could not open " + full_name + " to write");
			}
		}

		template <>
		bool config::get(const std::string& key, bool default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return it->second == "true";
			}
			return default_value;
		}

		template <>
		int config::get(const std::string& key, int default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atoi(it->second.data());
			}
			return default_value;
		}

		template <>
		long config::get(const std::string& key, long default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atol(it->second.data());
			}
			return default_value;
		}

		template <>
		long long config::get(const std::string& key, long long default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atoll(it->second.data());
			}
			return default_value;
		}

		template <>
		unsigned int config::get(const std::string& key, unsigned default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atoi(it->second.data());
			}
			return default_value;
		}

		template <>
		unsigned long config::get(const std::string& key, unsigned long default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atol(it->second.data());
			}
			return default_value;
		}

		template <>
		unsigned long long config::get(const std::string& key, unsigned long long default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atoll(it->second.data());
			}
			return default_value;
		}

		template <>
		float config::get(const std::string& key, float default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return std::atof(it->second.data());
			}
			return default_value;
		}

		template <>
		std::string config::get(const std::string& key, std::string default_value) const {
			auto it = values_.find(key);
			if (it != values_.end()) {
				return it->second;
			}
			return default_value;
		}

		template <>
		void config::set(const std::string& key, bool value) {
			values_[key] = value ? "true" : "false";
		}
	}
}
