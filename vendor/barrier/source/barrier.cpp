// barrier
// The Kek License (KEK)

// Copyright � 2018-2020 ThePhD

// kek :3

#include <barrier/barrier.hpp>
#include <vector>

namespace barrier {

	std::vector<size_t>& inplace_transform_vector(std::vector<size_t>& source) {
		for (auto& v : source) {
			v *= 2;
		}
		return source;
	}

	size_t& inplace_transform_int(size_t& source) {
		return source *= 2;
	}

	std::vector<size_t> pure_transform_vector(std::vector<size_t> data) {
		for (auto& v : data) {
			v *= 2;
		}
		return data;
	}

	size_t pure_transform_int(size_t source) {
		return source * 2;
	}

	size_t pure_write_int(size_t source) {
		return source;
	}

	void write::operator()(std::size_t value) noexcept {
		*target = value;
		target++;
	}

} // namespace barrier
