#include <fstream>

#include <vtdg/uniform_grid.hpp>

namespace vtdg {
	uniform_grid::uniform_grid(float length, size_t resolution) :
	length_{length},
	resolution_{resolution},
	sample_rate_{length / resolution} {
		auto actual_resolution = resolution_ + 1;
		grid_.assign(actual_resolution * actual_resolution * actual_resolution, 0.f);
	}

	void uniform_grid::fill(const std::function<float(const glm::vec3&)>& f) {
		auto actual_resolution = resolution_ + 1;

		for (auto x = 0; x <= resolution_; ++x) {
			for (auto y = 0; y <= resolution_; ++y) {
				for (auto z = 0; z <= resolution_; ++z) {
					grid_[z + actual_resolution * (y + actual_resolution * x)] = f(glm::vec3{x * sample_rate_, y * sample_rate_, z * sample_rate_});
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

		auto actual_resolution = resolution_ + 1;

		os.write(reinterpret_cast<char*>(&h), sizeof(volume_data_header));
		for (auto x = 0; x <= resolution_; ++x) {
			for (auto y = 0; y <= resolution_; ++y) {
				for (auto z = 0; z <= resolution_; ++z) {
					os.write(reinterpret_cast<char*>(&grid_[z + actual_resolution * (y + actual_resolution * x)]), sizeof(float));
				}
			}
		}
	}

	float uniform_grid::value(size_t x, size_t y, size_t z) {
		auto actual_resolution = resolution_ + 1;
		return grid_.at(z + actual_resolution * (y + actual_resolution * x));
	}
}
