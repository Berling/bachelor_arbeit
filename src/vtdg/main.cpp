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

	auto terrain = [&](const glm::vec3& p) {
		static auto zero = grid_length / 3.f;
		auto noise1 = noise(p * 4.03f) * 0.025f;
		auto noise2 = noise(p * 1.96f) * 0.05f;
		auto noise3 = noise(p * 1.01f) * 0.01f;
		auto noise4 = noise(p * 0.54f) * 0.05f;
		auto noise5 = noise(p * 0.23f) * 0.75f;
		auto noise6 = noise(p * 0.14f) * 1.01f;
		auto noise7 = noise(p * 0.09f) * 0.34f;
		auto noise8 = noise(p * 0.071f) * 0.75f;
		auto noise9 = noise(p * 0.025f);
		return (zero - p.y) + noise1 + noise2 + noise3 + noise4 + noise5 + noise6 + noise7 + noise8 + noise9;
	};

	auto sampling_function = std::string{argv[3]};
	if (sampling_function == "sphere") {
		grid.fill(sphere);
	} else if (sampling_function == "noise") {
		grid.fill(noise);
	} else if (sampling_function == "plane") {
		grid.fill(plane);
	} else if (sampling_function == "terrain") {
		grid.fill(terrain);
	} else {
		throw std::runtime_error{"no sampling function named " + sampling_function};
	}
	grid.serialize(argv[4]);

	return 0;
}
