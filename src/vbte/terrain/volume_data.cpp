#include <stdexcept>
#include <string>

#include <vbte/terrain/volume_data.hpp>
#include <vbte/utils/logger.hpp>

namespace vbte {
	namespace terrain {
		volume_data::volume_data(const std::string& file_name, const std::vector<char>& data) {
			auto data_ptr = data.data();

			auto file_header = *reinterpret_cast<const volume_data_header*>(data_ptr);
			if (file_header.magic != 0x31647674) {
				throw std::runtime_error{file_name + " contains no volume data"};
			}

			auto calculated_size = sizeof(volume_data_header)
				+ file_header.resolution * file_header.resolution * file_header.resolution * sizeof(float);
			if (calculated_size != data.size()) {
				throw std::runtime_error{file_name + " actual size: " + std::to_string(data.size())
					+ " calculated size: " + std::to_string(calculated_size)};
			}

			length_ = file_header.length;
			resolution_ = file_header.resolution;
			sample_rate_ = length_ / resolution_;

			data_ptr += sizeof(volume_data_header);

			for (auto x = 0; x < resolution_; ++x) {
				grid_.emplace_back(std::vector<std::vector<float>>{});
				for (auto y = 0; y < resolution_; ++y) {
					grid_[x].emplace_back(std::vector<float>{});
					for (auto z = 0; z < resolution_; ++z) {
						grid_[x][y].emplace_back(*reinterpret_cast<const float*>(data_ptr));
						data_ptr += sizeof(float);
					}
				}
			}
		}

		float volume_data::sample(const glm::vec3& p) const {
			auto index_x = static_cast<int>(p.x / sample_rate_);
			auto index_y = static_cast<int>(p.y / sample_rate_);
			auto index_z = static_cast<int>(p.z / sample_rate_);

			auto x0 = index_x * sample_rate_;
			auto y0 = index_y * sample_rate_;
			auto z0 = index_z * sample_rate_;
			auto x1 = (index_x + 1) * sample_rate_;
			if (p.x >= (resolution_ - 1) * sample_rate_) {
				x1 = (index_x - 1) * sample_rate_;
			} else if (p.x <= 0.f) {
				x1 = (index_x + 1) * sample_rate_;
			}
			auto y1 = (index_y + 1) * sample_rate_;
			if (p.y >= (resolution_ - 1) * sample_rate_) {
				y1 = (index_y - 1) * sample_rate_;
			} else if (p.y <= 0.f) {
				y1 = (index_y + 1) * sample_rate_;
			}
			auto z1 = (index_z + 1) * sample_rate_;
			if (p.z >= (resolution_ - 1) * sample_rate_) {
				z1 = (index_z - 1) * sample_rate_;
			} else if (p.x <= 0.f) {
				z1 = (index_z + 1) * sample_rate_;
			}

			auto xd = (p.x - x0) / (x1 - x0);
			auto yd = (p.y - y0) / (y1 - y0);
			auto zd = (p.z - z0) / (z1 - z0);

			auto c00 = value(int(x0 / sample_rate_), int(y0 / sample_rate_), int(z0 / sample_rate_)) * (1.f - xd)
				+ value(int(x1 / sample_rate_), int(y0 / sample_rate_), int(z0 / sample_rate_)) * xd;
			auto c01 = value(int(x0 / sample_rate_), int(y0 / sample_rate_), int(z1 / sample_rate_)) * (1.f - xd)
				+ value(int(x1 / sample_rate_), int(y0 / sample_rate_), int(z1 / sample_rate_)) * xd;
			auto c10 = value(int(x0 / sample_rate_), int(y1 / sample_rate_), int(z0 / sample_rate_)) * (1.f - xd)
				+ value(int(x1 / sample_rate_), int(y1 / sample_rate_), int(z0 / sample_rate_)) * xd;
			auto c11 = value(int(x0 / sample_rate_), int(y1 / sample_rate_), int(z1 / sample_rate_)) * (1.f - xd)
				+ value(int(x1 / sample_rate_), int(y1 / sample_rate_), int(z1 / sample_rate_)) * xd;

			auto c0 = c00 * (1.f - yd) + c10 * yd;
			auto c1 = c01 * (1.f - yd) + c11 * yd;

			return c0 * (1.f - zd) + c1 * zd;
		}

		float volume_data::sample(const glm::vec3& p, size_t resolution_) const {
			return 0.f;
		}
	}
}
