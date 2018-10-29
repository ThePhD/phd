

// barrier
// The Kek License (KEK)

// Copyright � 2018 ThePhD

// kek

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

} // namespace barrier
