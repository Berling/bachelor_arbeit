#include <glm/gtc/type_ptr.hpp>

#include <vbte/graphics/program.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace graphics {
		program::program() noexcept
		: id_(glCreateProgram()) {}

		program::~program() {
			for (auto& shader : shaders_) {
				glDetachShader(id_, shader->id_);
			}
			glDeleteProgram(id_);
		}

		program::program(program&& rhs) noexcept
		: id_(rhs.id_), shaders_(rhs.shaders_) {
			rhs.id_ = 0;
		}

		program& program::operator=(program&& rhs) noexcept {
			id_ = rhs.id_;
			shaders_ = rhs.shaders_;
			rhs.id_ = 0;
			return *this;
		}

		void program::attach_shader(shader_ptr shader) {
			shaders_.emplace_back(shader);
			glAttachShader(id_, shader->id_);
		}

		void program::link() {
			glLinkProgram(id_);

			auto log_length = 0;
			glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_length);
			if (log_length > 1) {
				auto log_buffer = std::vector<char>(log_length);
				glGetProgramInfoLog(id_, log_length, nullptr, log_buffer.data());
				utils::log(utils::log_level::debug) << "link log of program " << id_ << ":" << std::endl;
				utils::log(utils::log_level::debug) << std::string(log_buffer.begin(), log_buffer.end() - 2) << std::endl;
			}

			auto link_status = 0;
			glGetProgramiv(id_, GL_LINK_STATUS, &link_status);
			if (!link_status) {
				for (auto& shader : shaders_) {
					glDetachShader(id_, shader->id_);
				}
				glDeleteProgram(id_);
				throw std::runtime_error("an error occured during linking of program " + std::to_string(id_));
			}
		}

		void program::uniform(const std::string& name, float value) noexcept {
			glUniform1f(resolve_uniform_location(name), value);
		}

		void program::uniform(const std::string& name, int32_t value) noexcept {
			glUniform1i(resolve_uniform_location(name), value);
		}

		void program::uniform(const std::string& name, uint32_t value) noexcept {
			glUniform1ui(resolve_uniform_location(name), value);
		}

		void program::uniform(const std::string& name, const glm::vec2& value) noexcept {
			glUniform2fv(resolve_uniform_location(name), 1, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, const glm::vec3& value) noexcept {
			glUniform3fv(resolve_uniform_location(name), 1, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, const glm::vec4& value) noexcept {
			glUniform4fv(resolve_uniform_location(name), 1, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, const glm::ivec2& value) noexcept {
			glUniform2iv(resolve_uniform_location(name), 1, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, const glm::ivec3& value) noexcept {
			glUniform3iv(resolve_uniform_location(name), 1, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, const glm::ivec4& value) noexcept {
			glUniform4iv(resolve_uniform_location(name), 1, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, bool transpose, const glm::mat2& value) noexcept {
			glUniformMatrix2fv(resolve_uniform_location(name), 1, transpose, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, bool transpose, const glm::mat3& value) noexcept {
			glUniformMatrix3fv(resolve_uniform_location(name), 1, transpose, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, bool transpose, const glm::mat4& value) noexcept {
			glUniformMatrix4fv(resolve_uniform_location(name), 1, transpose, glm::value_ptr(value));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<float>& values) noexcept {
			glUniform1fv(resolve_uniform_location(name), count, reinterpret_cast<const float*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<int32_t>& values) noexcept {
			glUniform1iv(resolve_uniform_location(name), count, reinterpret_cast<const int32_t*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<uint32_t>& values) noexcept {
			glUniform1uiv(resolve_uniform_location(name), count, reinterpret_cast<const uint32_t*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<glm::vec2>& values) noexcept {
			glUniform2fv(resolve_uniform_location(name), count,reinterpret_cast<const float*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<glm::vec3>& values) noexcept {
			glUniform3fv(resolve_uniform_location(name), count, reinterpret_cast<const float*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<glm::vec4>& values) noexcept {
			glUniform4fv(resolve_uniform_location(name), count, reinterpret_cast<const float*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<glm::ivec2>& values) noexcept {
			glUniform2iv(resolve_uniform_location(name), count, reinterpret_cast<const int32_t*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<glm::ivec3>& values) noexcept {
			glUniform3iv(resolve_uniform_location(name), count, reinterpret_cast<const int32_t*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, const std::vector<glm::ivec4>& values) noexcept {
			glUniform4iv(resolve_uniform_location(name), count, reinterpret_cast<const int32_t*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, bool transpose, const std::vector<glm::mat2>& values) noexcept {
			glUniformMatrix2fv(resolve_uniform_location(name), count, transpose, reinterpret_cast<const float*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, bool transpose, const std::vector<glm::mat3>& values) noexcept {
			glUniformMatrix3fv(resolve_uniform_location(name), count, transpose, reinterpret_cast<const float*>(values.data()));
		}

		void program::uniform(const std::string& name, size_t count, bool transpose, const std::vector<glm::mat4>& values) noexcept {
			glUniformMatrix4fv(resolve_uniform_location(name), count, transpose, reinterpret_cast<const float*>(values.data()));
		}

		void program::bind_attribute_location(const std::string& name, uint32_t index) noexcept {
			glBindAttribLocation(id_, index, name.c_str());
		}

		void program::bind_frag_data_location(const std::string& name, uint32_t color_number) noexcept {
			glBindFragDataLocation(id_, color_number, name.c_str());
		}

		GLuint program::resolve_uniform_location(const std::string& name) {
			auto it = uniform_locations_.find(name);
			if (it == uniform_locations_.end()) {
				uniform_locations_[name] = glGetUniformLocation(id_, name.c_str());
			}
			return uniform_locations_[name];
		}
	}
}
