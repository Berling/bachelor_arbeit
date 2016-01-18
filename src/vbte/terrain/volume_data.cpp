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
	}
}
