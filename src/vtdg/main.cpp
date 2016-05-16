#include <string>
#include <iostream>
#include <fstream>

#include <vtdg/perlin_noise.hpp>
#include <vtdg/uniform_grid.hpp>

struct terrain_header {
	const uint32_t magic = 0x31495456;
	uint32_t cell_count = 0;
	uint64_t cell_resolution = 0;
	float cell_length = 0.f;
	uint64_t size = 0;
};

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
		static auto zero = (grid_length / grid_resolution) / 3.f;
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
		grid.serialize(argv[4]);
	} else if (sampling_function == "noise") {
		grid.fill(noise);
		grid.serialize(argv[4]);
	} else if (sampling_function == "plane") {
		grid.fill(plane);
		grid.serialize(argv[4]);
	} else if (sampling_function == "terrain") {
		auto cell_length = grid_length / grid_resolution;
		auto cell_resolution = 64;

		terrain_header head;
		head.cell_count = grid_resolution;
		head.cell_resolution = cell_resolution;
		head.cell_length = cell_length;

		std::vector<std::string> cell_paths;

		size_t size = 2 * sizeof(uint32_t) + 2 * sizeof(uint64_t) + sizeof(float);
		for (auto x = 0; x < grid_resolution; ++x) {
			for (auto y = 0; y < grid_resolution; ++y) {
				auto x_offset = x * cell_length;
				auto y_offset = y * cell_length;
				vtdg::uniform_grid cell{cell_length, 64};
				cell.fill(terrain, glm::vec3{x_offset, 0.f, y_offset});
				auto prefix = std::string{"../assets/"};
				auto file_name = std::string{argv[4]};
				auto path_without_prefix = "terrain/" + file_name + std::string{"_"} + std::to_string(x) + "_" + std::to_string(y) + std::string{".vol"};
				auto path = prefix + path_without_prefix;
				size += path_without_prefix.size() + sizeof(uint64_t);
				cell_paths.emplace_back(path_without_prefix);
				cell.serialize(path);
			}
		}

		head.size = size;

		auto path = std::string{"../assets/terrain/"} + argv[4] + std::string{".ter"};
		std::ofstream os{path, std::ios_base::trunc | std::ios_base::binary};
		if (!os.is_open()) {
			throw std::runtime_error{"could not open output file " + path};
		}

		os.write(reinterpret_cast<const char*>(&head.magic), sizeof(uint32_t));
		os.write(reinterpret_cast<char*>(&head.cell_count), sizeof(uint32_t));
		os.write(reinterpret_cast<char*>(&head.cell_resolution), sizeof(uint64_t));
		os.write(reinterpret_cast<char*>(&head.cell_length), sizeof(float));
		os.write(reinterpret_cast<char*>(&head.size), sizeof(uint64_t));
		for (auto& p : cell_paths) {
			uint64_t size = p.size();
			os.write(reinterpret_cast<char*>(&size), sizeof(uint64_t));
			os.write(p.c_str(), size);
		}

	} else {
		throw std::runtime_error{"no sampling function named " + sampling_function};
	}

	return 0;
}
