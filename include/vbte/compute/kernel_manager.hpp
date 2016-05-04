#pragma once

#include <memory>
#include <string>

#include <vbte/compute/kernel.hpp>
#include <vbte/utils/cached_resource_manager.hpp>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace terrain {
		class terrain_system;
	}
}

namespace vbte {
	namespace compute {
		using kernel_ptr = std::shared_ptr<kernel>;

		class kernel_manager : public utils::cached_resource_manager<kernel, kernel_manager> {
		private:
			core::engine& engine_;
			terrain::terrain_system& terrain_system_;

			friend class utils::cached_resource_manager<kernel, kernel_manager>;

		public:
			kernel_manager(core::engine& engine, terrain::terrain_system& terrain_system) noexcept;
			~kernel_manager() = default;

		protected:
			kernel_ptr load_new(const std::string& file_name, const std::string& kernel_name);
		};
	}
}
