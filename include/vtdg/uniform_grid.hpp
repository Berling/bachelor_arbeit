#pragma once

#include <functional>
#include <vector>

#include <glm/glm.hpp>

namespace vtdg {
	struct volume_data_header {
		const uint32_t magic = 0x31647674;
		float length = 0.f;
		int64_t resolution = 0;
	};
}

namespace vtdg {
	class uniform_grid {
	private:
		float length_;
		size_t resolution_;
		float sample_rate_;
		std::vector<float> grid_;

	public:
		uniform_grid(float length, size_t resolution);
		~uniform_grid() = default;

		uniform_grid(const uniform_grid&) = default;
		uniform_grid& operator=(const uniform_grid&) = default;

		uniform_grid(uniform_grid&&) = default;
		uniform_grid& operator=(uniform_grid&&) = default;

		void fill(const std::function<float(const glm::vec3&)>& f);
		void serialize(const std::string& file_path);
		float value(size_t x, size_t y, size_t z);
	};
}
