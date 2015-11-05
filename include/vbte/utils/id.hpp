#ifndef __VBTE_UTILS_ID_HPP__
#define __VBTE_UTILS_ID_HPP__

namespace vbte {
	namespace utils {
		template <typename t>
		auto id_generator() {
			static auto next_id = 0ull;
			return ++next_id;
		}

		template <typename t>
		auto type_id() {
			static auto type_id = id_generator<void>();
			return type_id;
		}
	}
}

#endif
