#include <iostream>
#include <stdexcept>

#include <glm/gtx/string_cast.hpp>

#include <vbte/core/engine.hpp>
#include <vbte/graphics/transform_feedback_buffer.hpp>
#include <vbte/graphics/vertex_layout.hpp>
#include <vbte/rendering/rendering_system.hpp>
#include <vbte/terrain/marching_cubes.hpp>
#include <vbte/terrain/terrain_cell.hpp>
#include <vbte/terrain/terrain_system.hpp>
#include <vbte/terrain/volume_data.hpp>
#include <vbte/terrain/volume_data_manager.hpp>

namespace vbte {
	namespace terrain {
		terrain_cell::terrain_cell(core::engine& engine, const glm::vec3& position, const glm::quat& rotation, const std::string& file_name) :
		rendering::drawable{engine, position, rotation}, vbo_{GL_DYNAMIC_DRAW} {
			volume_data_ = engine_.terrain_system().volume_data_manager().load(file_name);
			if (!volume_data_) {
				throw std::runtime_error{"could not load file " + file_name};
			}

			auto vertices = marching_cubes(*volume_data_, volume_data_->resolution());
			index_count_ = vertices.size();
			vbo_.data(sizeof(rendering::basic_vertex) * index_count_, vertices.data());

			engine_.rendering_system().basic_layout().setup_layout(vao_, &vbo_);

			graphics::vertex_array volume_vao_;
			std::array<glm::vec4, 4> cell = {
				glm::vec4{1.f, 0.f, 0.f, 0.f},
				glm::vec4{0.f, 1.f, 0.f, 0.f},
				glm::vec4{0.f, 0.f, 1.f, 0.f},
				glm::vec4{0.f, 0.f, 0.f, 1.f}
			};
			graphics::vertex_buffer volume_vbo_{4 * sizeof(glm::vec4), &cell[0], GL_STATIC_DRAW};
			auto& volume_data_layout = engine.terrain_system().volume_data_layout();
			volume_data_layout.setup_layout(volume_vao_, &volume_vbo_);
			auto& marching_cubes_program = engine.terrain_system().marching_cubes_program();
			graphics::transform_feedback_buffer tfb{3 * sizeof(glm::vec4), nullptr, GL_STATIC_READ};
			volume_vao_.bind();
			glEnable(GL_RASTERIZER_DISCARD);
			tfb.bind_range(0, 0, 3 * sizeof(glm::vec4));
			marching_cubes_program.use();
			tfb.begin(GL_TRIANGLES);
			glDrawArrays(GL_POINTS, 0, 1);
			tfb.end();
			glDisable(GL_RASTERIZER_DISCARD);

			auto buffer_ptr = tfb.map_read<glm::vec3>();
			for (auto i = 0; i < 3; ++i) {
				std::cout << glm::to_string(buffer_ptr[i]) << std::endl;
			}
		}

		void terrain_cell::draw() const {
			vao_.bind();
			glDrawArrays(GL_TRIANGLES, 0, index_count_);
		}
	}
}
