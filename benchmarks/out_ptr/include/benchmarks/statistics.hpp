// Copyright � 2018-2020 ThePhD

#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_BENCHMARKS_BENCHMARK_COMPUTE_HPP
#define PHD_OUT_PTR_OUT_PTR_BENCHMARKS_BENCHMARK_COMPUTE_HPP

#include <cstddef>
#include <cstdint>

#include <vector>

double compute_mean(const std::vector<double>& v);

double compute_variance(const std::vector<double>& v);

double compute_stddev(const std::vector<double>& v);

double compute_index_of_dispersion(const std::vector<double>& v);

double compute_max(const std::vector<double>& v);

double compute_min(const std::vector<double>& v);

std::uint64_t rdtsc();

struct rdtsc_time {
	std::uint64_t start;

	rdtsc_time()
	: start(rdtsc()) {
	}

	std::uint64_t time() const {
		return rdtsc() - start;
	}

	double d_time() const {
		return static_cast<double>(this->time());
	}
};

#endif // PHD_OUT_PTR_OUT_PTR_BENCHMARKS_BENCHMARK_COMPUTE_HPP
