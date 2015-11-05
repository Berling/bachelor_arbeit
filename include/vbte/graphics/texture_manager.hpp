#ifndef __VBTE_GRAPHICS_TEXTURE_MANAGER_HPP__
#define __VBTE_GRAPHICS_TEXTURE_MANAGER_HPP__

#include <memory>
#include <string>

#include <vbte/utils/cached_resource_manager.hpp>

namespace vbte {
	namespace core {
		class engine;
	}

	namespace graphics {
		class texture;
	}
}

namespace vbte {
	namespace graphics {
		using texture_ptr = std::shared_ptr<const texture>;

		class texture_manager : public utils::cached_resource_manager<const texture, texture_manager> {
		private:
			core::engine& engine_;

			friend class utils::cached_resource_manager<const texture, texture_manager>;

		public:
			texture_manager(core::engine& engine) noexcept;
			~texture_manager() = default;

		protected:
			texture_ptr load_new(const std::string& name);
		};
	}
}

#endif
