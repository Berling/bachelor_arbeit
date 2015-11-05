#include <vbte/graphics/shader.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace graphics {
		shader::shader(const std::string& name, GLenum type, const std::string& source)
		: type_(type), id_(glCreateShader(type_)) {
			auto source_ptr = source.c_str();
			glShaderSource(id_, 1, &source_ptr, nullptr);

			auto source_length = 0;
			glGetShaderiv(id_, GL_SHADER_SOURCE_LENGTH, &source_length);
			if (source_length == 0) {
				throw std::runtime_error("length of shader source is zero");
			}

			glCompileShader(id_);

			auto log_length = 0;
			glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_length);
			if (log_length > 1) {
				auto log_buffer = std::vector<char>(log_length);
				glGetShaderInfoLog(id_, log_length, nullptr, log_buffer.data());
				utils::log(utils::log_level::debug) << "compilation log of " << name << ":" << std::endl;
				utils::log(utils::log_level::debug) << std::string(log_buffer.begin(), log_buffer.end() - 2) << std::endl;
			}

			auto compile_status = 0;
			glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);
			if (!compile_status) {
				glDeleteShader(id_);
				std::runtime_error("an error occured during compilation of shader " + name);
			}
		}

		shader::~shader() {
			glDeleteShader(id_);
		}

		shader::shader(shader&& rhs) noexcept
		: id_(rhs.id_) {
			rhs.id_ = 0;
		}

		shader& shader::operator&(shader&& rhs) noexcept {
			id_ = rhs.id_;
			rhs.id_ = 0;
			return *this;
		}
	}
}
