#ifndef __VBTE_UTILS_CACHED_RESOURCE_MANAGER_HPP__
#define __VBTE_UTILS_CACHED_RESOURCE_MANAGER_HPP__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vbte {
	namespace utils {
		template <typename resource, typename manager>
		class cached_resource_manager {
		private:
			std::unordered_map<std::string, std::shared_ptr<resource>> cache_;
			std::vector<std::string> clean_buffer_;

		public:
			cached_resource_manager() = default;
			~cached_resource_manager() noexcept = default;

			cached_resource_manager(const cached_resource_manager& other) = delete;
			cached_resource_manager& operator= (const cached_resource_manager& other) = delete;

			cached_resource_manager(cached_resource_manager&& other) = delete;
			cached_resource_manager& operator= (cached_resource_manager&& other) = delete;

			template <typename... arguments>
			std::shared_ptr<resource> load(const std::string& name, arguments&&... args) {
				if (name.empty()) {
					return nullptr;
				}
				auto it = cache_.find(name);
				if (it != cache_.end()) {
					auto ptr = it->second;
					if (ptr) {
						return ptr;
					}
				}
				auto ptr = static_cast<manager*>(this)->load_new(name, std::forward<arguments>(args)...);
				if (ptr) {
					cache_[name] = ptr;
					return ptr;
				}
				return nullptr;
			}

			void clean_up() {
				for (auto& entry : cache_) {
					if (entry.second.unique()) {
						clean_buffer_.push_back(entry.first);
					}
				}
				for (auto& entry : clean_buffer_) {
					cache_.erase(entry);
				}
				clean_buffer_.clear();
			}
		};
	}
}

#endif
