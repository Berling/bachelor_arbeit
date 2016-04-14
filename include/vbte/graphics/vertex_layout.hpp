#ifndef __VBTE_GRAPHICS_VERTEX_LAYOUT_HPP__
#define __VBTE_GRAPHICS_VERTEX_LAYOUT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <vbte/graphics/buffer.hpp>
#include <vbte/graphics/program.hpp>
#include <vbte/graphics/vertex_array.hpp>

namespace vbte {
	namespace graphics {
		struct vertex_attribute {
			std::string name;
			uint32_t size;
			GLenum type;
			bool normalized;
			size_t stride;
			intptr_t offset;
			int index;
			intptr_t component_offset;
			int factor;
			int divisor;
		};

		class vertex_layout {
		private:
			std::vector<vertex_attribute> vertex_attributes_;
			std::vector<std::string> transform_feedback_varyings_;

		public:
			vertex_layout() = default;
			~vertex_layout() = default;

			vertex_layout(const vertex_layout&) = delete;
			vertex_layout& operator=(const vertex_layout&) = delete;

			vertex_layout(vertex_layout&&) noexcept = default;
			vertex_layout& operator=(vertex_layout&&) = default;

			void emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride, intptr_t offset, int index = 0, int divisor = 0);
			void emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride, intptr_t offset, intptr_t attribute_size, int factor, int index = 0, int divisor = 0);
			void emplace_back(const std::string& varying_name);
			void setup_layout(const vertex_array& vertex_array, const std::unique_ptr<vertex_buffer>* buffers) noexcept;
			void setup_layout(const vertex_array& vertex_array, const vertex_buffer* buffers) noexcept;
			void setup_layout(const vertex_array& vertex_array, const vertex_buffer** buffers) noexcept;
			void setup_program(program& program, const std::string& fragcolor_name) noexcept;
		};
	}
}

#endif
