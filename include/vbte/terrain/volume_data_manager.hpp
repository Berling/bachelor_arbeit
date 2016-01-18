#pragma once

#include <memory>

#include <vbte/utils/cached_resource_manager.hpp>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace terrain {
		class volume_data;
	}
}

namespace vbte {
	namespace terrain {
		using volume_data_ptr = std::shared_ptr<const volume_data>;

		class volume_data_manager : public utils::cached_resource_manager<const volume_data, volume_data_manager> {
		private:
			core::engine& engine_;

			friend class utils::cached_resource_manager<const volume_data, volume_data_manager>;

		public:
			volume_data_manager(core::engine& engine) noexcept;
			~volume_data_manager() = default;

		protected:
			volume_data_ptr load_new(const std::string& file_name);
		};
	}
}
