#include <string>

#include <vtdg/uniform_grid.hpp>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		throw std::runtime_error{"illegal number of command line arguments"};
	}

	vtdg::uniform_grid grid{5.f, 128};

	auto ball = [](const glm::vec3& p) {
		static auto center = glm::vec3{2.5f};
		static auto radius = 1.5f;

		if (glm::dot(p - center, p - center) < radius * radius) {
			return 1.f;
		} else {
			return 0.f;
		}
	};

	grid.fill(ball);
	grid.serialize(argv[1]);

	return 0;
}
