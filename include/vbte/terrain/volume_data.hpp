#pragma once

#include <string>
#include <vector>

namespace vbte {
	namespace terrain {
		struct volume_data_header {
			const uint32_t magic = 0x31647674;
			float length = 0.f;
			int64_t resolution = 0;
		};
	}

	namespace terrain {
		class volume_data {
		private:
			float length_;
			size_t resolution_;
			float sample_rate_;
			std::vector<std::vector<std::vector<float>>> grid_;

		public:
			volume_data(const std::string& file_name, const std::vector<char>& data);
			~volume_data() = default;

			volume_data(const volume_data&) = default;
			volume_data& operator=(const volume_data&) = default;

			volume_data(volume_data&&) = default;
			volume_data& operator=(volume_data&&) = default;

			auto& operator[](const size_t index) noexcept {
				return grid_[index];
			}

			auto& operator[](const size_t index) const noexcept {
				return grid_[index];
			}

			auto& value(size_t x, size_t y, size_t z) {
				return grid_.at(x).at(y).at(z);
			}

			auto& value(size_t x, size_t y, size_t z) const {
				return grid_.at(x).at(y).at(z);
			}

			auto grid_length() const noexcept {
				return length_;
			}

			auto resolution() const noexcept {
				return resolution_;
			}

			auto sample_rate() const noexcept {
				return sample_rate_;
			}
		};
	}
}
