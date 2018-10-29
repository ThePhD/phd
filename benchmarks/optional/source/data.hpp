// Copyright � 2018 ThePhD

#ifndef PHD_OPTIONAL_BENCHMARKS_LIMITS_HPP
#define PHD_OPTIONAL_BENCHMARKS_LIMITS_HPP

#include <cstddef>
#include <random>
#include <vector>

constexpr std::size_t vector_size = 8;
constexpr std::size_t int_value = 2;
constexpr std::size_t int_fail_value = 1;
constexpr std::size_t vector_fail_value = 1;

std::default_random_engine& get_engine();

std::vector<std::size_t> create_random_vector(std::size_t vec_size = vector_size);

#endif // PHD_OPTIONAL_BENCHMARKS_LIMITS_HPP
