#pragma once

#include <CL/cl.hpp>

#include <vbte/compute/context.hpp>
#include <vbte/compute/kernel_manager.hpp>
#include <vbte/graphics/program.hpp>
#include <vbte/graphics/vertex_layout.hpp>
#include <vbte/terrain/volume_data_manager.hpp>

namespace vbte {
	namespace compute {
		class kernel;
	}

	namespace core {
		class engine;
	}

	namespace graphics {
		class shader_manager;
	}
}

namespace vbte {
	namespace terrain {
		class terrain_system {
		private:
			core::engine& engine_;
			terrain::volume_data_manager volume_data_manager_;
			graphics::vertex_layout volume_data_layout_;
			graphics::vertex_layout transform_feedback_layout_;
			graphics::program marching_cubes_program_;
			compute::context context_;
			compute::kernel_manager kernel_manager_;
			std::shared_ptr<compute::kernel> marching_cubes_kernel_;

		public:
			terrain_system(core::engine& engine);
			~terrain_system() = default;

			terrain_system(const terrain_system&) = delete;
			terrain_system& operator=(const terrain_system&) = delete;

			terrain_system(terrain_system&&) = default;
			terrain_system& operator=(terrain_system&&) = default;

			void update(float delta_time);

			auto& volume_data_manager() noexcept {
				return volume_data_manager_;
			}

			auto& transform_feedback_layout() noexcept {
				return transform_feedback_layout_;
			}

			auto& volume_data_layout() noexcept {
				return volume_data_layout_;
			}

			auto& marching_cubes_program() noexcept {
				return marching_cubes_program_;
			}

			auto& compute_context() noexcept {
				return context_;
			}

			auto& kernel_manager() noexcept {
				return kernel_manager_;
			}

			auto& marching_cubes_kernel() noexcept {
				return *marching_cubes_kernel_;
			}

		private:
			void init_marching_cubes_program(graphics::shader_manager& shader_manager);
			void init_transform_feedback_layout();
			void init_volume_data_layout();
		};
	}
}
