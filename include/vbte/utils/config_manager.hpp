#ifndef __VBTE_UTILS_CONFIG_MANAGER_HPP__
#define __VBTE_UTILS_CONFIG_MANAGER_HPP__

#include <memory>

#include <vbte/utils/cached_resource_manager.hpp>

namespace vbte {
	namespace core {
		class engine;
	}
		
	namespace utils {
		class config;
	}
	
}

namespace vbte {
	namespace utils {
		using config_ptr = std::shared_ptr<config>;

		class config_manager : public utils::cached_resource_manager<config, config_manager> {
		private:
			core::engine& engine_;

			friend class utils::cached_resource_manager<config, config_manager>;

		public:
			config_manager(core::engine& engine_) noexcept;
			~config_manager() = default;

		protected:
			config_ptr load_new(const std::string& name);
		};
	}
}

#endif
