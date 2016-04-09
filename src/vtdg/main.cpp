#include <string>
#include <iostream>

#include <vtdg/perlin_noise.hpp>
#include <vtdg/uniform_grid.hpp>

int main(int argc, char* argv[]) {
	if (argc != 5) {
		throw std::runtime_error{"illegal number of command line arguments"};
	}

	auto grid_length = std::stof(argv[1]);
	auto grid_resolution = std::stoul(argv[2]);

	vtdg::uniform_grid grid{grid_length, grid_resolution};

	auto sphere = [&](const glm::vec3& p) {
		static auto center = glm::vec3{grid_length / 2.f};
		static auto radius = grid_length / 3.f;

		return radius - glm::length(p - center);
	};

	auto noise = [&](const glm::vec3& p) {
		return 5.f * vtdg::perlin_noise::noise(p.x, p.y, p.z);
	};

	auto plane = [&](const glm::vec3& p) {
		return -p.y;
	};

	auto sampling_function = std::string{argv[3]};
	if (sampling_function == "sphere") {
		grid.fill(sphere);
	} else if (sampling_function == "noise") {
		grid.fill(noise);
	} else if (sampling_function == "plane") {
		grid.fill(plane);
	} else {
		throw std::runtime_error{"no sampling function named " + sampling_function};
	}
	grid.serialize(argv[4]);

	return 0;
}
