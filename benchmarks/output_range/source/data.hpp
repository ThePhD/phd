// Copyright � 2018-2020 ThePhD

#ifndef PHD_OUTPUT_RANGE_BENCHMARKS_LIMITS_HPP
#define PHD_OUTPUT_RANGE_BENCHMARKS_LIMITS_HPP

#include <cstddef>
#include <random>
#include <vector>

inline constexpr std::size_t vector_size = 8;
inline constexpr std::size_t int_value = 2;
inline constexpr std::size_t int_fail_value = 1;
inline constexpr std::size_t vector_fail_value = 1;

std::default_random_engine& get_engine();

std::vector<std::size_t> create_random_vector(std::size_t vec_size = vector_size);

const std::vector<std::size_t>& get_constant_random_vector();

#endif // PHD_OUTPUT_RANGE_BENCHMARKS_LIMITS_HPP
