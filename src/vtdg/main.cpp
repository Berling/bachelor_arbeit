#include <string>
#include <iostream>
#include <fstream>
#include <random>

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

	std::random_device generator;
	std::uniform_real_distribution<float> distribution(-1.f, 1.f);
	float noise_texture[16][16][16];
	for (auto x = 0; x < 16; ++x) {
		for (auto y = 0; y < 16; ++y) {
			for (auto z = 0; z < 16; ++z) {
				noise_texture[x][y][z] = distribution(generator);
			}
		}
	}

	float warp_texture[16][16][16];
	for (auto x = 0; x < 16; ++x) {
		for (auto y = 0; y < 16; ++y) {
			for (auto z = 0; z < 16; ++z) {
				noise_texture[x][y][z] = distribution(generator);
			}
		}
	}


	auto trilinear_sample = [&](float texture[16][16][16], const glm::vec3& p) {
		auto sample_rate = 1.f / 16.f;

		auto decimal_x = p.x - std::floor(p.x);
		auto index_x = static_cast<int>(decimal_x / sample_rate);
		auto index_x2 = ((index_x + 1) < 16 ? (index_x + 1) : 0);
		auto decimal_y = p.y - std::floor(p.y);
		auto index_y = static_cast<int>(decimal_y / sample_rate);
		auto index_y2 = ((index_y + 1) < 16 ? (index_y + 1) : 0);
		auto decimal_z = p.z - std::floor(p.z);
		auto index_z = static_cast<int>(decimal_z / sample_rate);
		auto index_z2 = ((index_z + 1) < 16 ? (index_z + 1) : 0);

		auto x0 = index_x * sample_rate;
		auto y0 = index_y * sample_rate;
		auto z0 = index_z * sample_rate;
		auto x1 = (index_x + 1) * sample_rate;
		auto y1 = (index_y + 1) * sample_rate;
		auto z1 = (index_z + 1) * sample_rate;

		auto xd = (decimal_x - x0) / (x1 - x0);
		auto yd = (decimal_y - y0) / (y1 - y0);
		auto zd = (decimal_z - z0) / (z1 - z0);

		auto c00 = texture[index_x][index_y][index_z] * (1.f - xd) + texture[index_x2][index_y][index_z] * xd;
		auto c01 = texture[index_x][index_y][index_z2] * (1.f - xd) + texture[index_x2][index_y][index_z2] * xd;
		auto c10 = texture[index_x][index_y2][index_z] * (1.f - xd) + texture[index_x2][index_y2][index_z] * xd;
		auto c11 = texture[index_x][index_y2][index_z2] * (1.f - xd) + texture[index_x2][index_y2][index_z2] * xd;

		auto c0 = c00 * (1.f - yd) + c10 * yd;
		auto c1 = c01 * (1.f - yd) + c11 * yd;

		return c0 * (1.f - zd) + c1 * zd;
	};

	auto terrain = [&](const glm::vec3& p) {
		static auto zero = (grid_length / grid_resolution) / 4.f;
		auto density = (zero - p.y);
		auto ws = p;

		auto warp = glm::vec3{trilinear_sample(warp_texture, ws * 0.004f)};
		ws += warp * 8.f;
		density += trilinear_sample(noise_texture, ws * 4.03f) * 0.0125f;
		density += trilinear_sample(noise_texture, ws * 1.96f) * 0.025f;
		density += trilinear_sample(noise_texture, ws * 1.01f) * 0.031f;
		density += trilinear_sample(noise_texture, ws * 0.49f) * 0.052f;
		density += trilinear_sample(noise_texture, ws * 0.23f) * 0.14f;
		density += trilinear_sample(noise_texture, ws * 0.101f) * 0.39f;
		density += trilinear_sample(noise_texture, ws * 0.052f) * 0.49f;
		density += trilinear_sample(noise_texture, ws * 0.022f) * 1.89f;
		density += trilinear_sample(noise_texture, ws * 0.014f) * 3.29f;
		density += trilinear_sample(noise_texture, ws * 0.0054f) * 4.89f;
		density += trilinear_sample(noise_texture, ws * 0.0021f) * 7.74f;
		density += trilinear_sample(noise_texture, ws * 0.0014f) * 10.01f;

		float hard_floor = 2.f;
		density += glm::clamp((hard_floor - p.y) * 3.f, 0.f, 1.f) * 40.f;

		return density;
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
		auto cell_resolution = size_t{128};

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
				vtdg::uniform_grid cell{cell_length, cell_resolution};
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
