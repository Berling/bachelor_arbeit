#ifndef __VBTE_RENDERING_DRAWABLE_HPP__
#define __VBTE_RENDERING_DRAWABLE_HPP__

namespace vbte {
	namespace core {
		class engine;
	}
}

namespace vbte {
	namespace rendering {
		class drawable {
		protected:
			core::engine& engine_;

		public:
			drawable(core::engine& engine) noexcept
			: engine_{engine} {}
			virtual ~drawable() = default;

			drawable(const drawable&) = delete;
			drawable& operator=(const drawable&) = delete;

			drawable(drawable&&) = default;
			drawable& operator=(drawable&&) = default;

			virtual void draw() const = 0;
		};
	}
}

#endif