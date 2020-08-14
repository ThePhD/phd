// Copyright � 2018-2020 ThePhD

#include "data.hpp"

#include <cstddef>
#include <random>
#include <vector>

std::default_random_engine& get_engine() {
	static std::default_random_engine re(0);
	return re;
}

std::vector<std::size_t> create_random_vector(std::size_t vec_size) {
	std::uniform_int_distribution<std::size_t> uniform_dist(2, 3);
	std::vector<std::size_t> values;
	values.reserve(vec_size);
	for (std::size_t i = 0; i < vec_size; ++i) {
		values.push_back(uniform_dist(get_engine()));
	}
	return values;
}

const std::vector<std::size_t>& get_constant_random_vector() {
	static auto value = create_random_vector(10000);
	return value;
}
