#include <stdexcept>

#include <vbte/core/engine.hpp>
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
		}

		void terrain_cell::draw() const {
			vao_.bind();
			glDrawArrays(GL_TRIANGLES, 0, index_count_);
		}
	}
}
