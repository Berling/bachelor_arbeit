#include <fstream>

#include <vtdg/uniform_grid.hpp>

namespace vtdg {
	uniform_grid::uniform_grid(float length, size_t resolution) :
	length_{length},
	resolution_{resolution},
	sample_rate_{length / resolution} {
		for (auto x = 0; x < resolution_; ++x) {
			grid_.emplace_back(std::vector<std::vector<float>>{});
			for (auto y = 0; y < resolution_; ++y) {
				grid_[x].emplace_back(std::vector<float>{});
				for (auto z = 0; z < resolution_; ++z) {
					grid_[x][y].emplace_back(0.f);
				}
			}
		}
	}

	void uniform_grid::fill(const std::function<float(const glm::vec3&)>& f) {
		for (auto x = 0; x < resolution_; ++x) {
			for (auto y = 0; y < resolution_; ++y) {
				for (auto z = 0; z < resolution_; ++z) {
					grid_[x][y][z] = f(glm::vec3{x * sample_rate_, y * sample_rate_, z * sample_rate_});
				}
			}
		}
	}

	void uniform_grid::serialize(const std::string& file_path) {
		std::ofstream os{file_path, std::ios_base::trunc | std::ios_base::binary};
		if (!os) {
			throw std::runtime_error{"could not open file to write " + file_path};
		}

		volume_data_header h;
		h.length = length_;
		h.resolution = resolution_;

		os.write(reinterpret_cast<char*>(&h), sizeof(volume_data_header));
		for (auto x = 0; x < resolution_; ++x) {
			for (auto y = 0; y < resolution_; ++y) {
				for (auto z = 0; z < resolution_; ++z) {
					os.write(reinterpret_cast<char*>(&grid_[x][y][z]), sizeof(float));
				}
			}
		}
	}

	float uniform_grid::value(size_t x, size_t y, size_t z) {
		return grid_.at(x).at(y).at(z);
	}
}
