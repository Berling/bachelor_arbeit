#ifndef __VBTE_UTILS_CONFIG_HPP__
#define __VBTE_UTILS_CONFIG_HPP__

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace utils {
		class config {
		private:
			core::engine& engine_;
			std::string name_;
			std::unordered_map<std::string, std::string> values_;

		public:
			config(core::engine& engine, const std::string& name, const std::string& data);
			~config() noexcept = default;

			config(const config&) = delete;
			config& operator=(const config&) = delete;

			config(config&&) = default;
			config& operator=(config&&) = default;

			template <typename value_type>
			value_type get(const std::string& key, value_type default_value) const;

			template <typename value_type>
			void set(const std::string& key, value_type value) {
				values_[key] = std::to_string(value);
			}

			const std::string& name() const noexcept {
				return name_;
			}

			void write() const;
		};
	}
}

#endif
