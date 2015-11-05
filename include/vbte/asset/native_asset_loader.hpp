#ifndef __VBTE_ASSET_NATIVE_ASSET_LOADER_HPP__
#define __VBTE_ASSET_NATIVE_ASSET_LOADER_HPP__

#include <memory>
#include <string>

namespace vbte {
	namespace asset {
		class asset;
		class asset_loader;
	}
}

namespace vbte {
	namespace asset {
		class native_asset_loader : public asset_loader {
		private:
			std::string prefix_;

		public:
			native_asset_loader(const std::string& prefix) noexcept;
			virtual ~native_asset_loader() = default;

			virtual std::shared_ptr<const asset> load_asset(const std::string& name) const override;
			virtual std::string native_name(const std::string& name) const override;
		};
	}
}

#endif
