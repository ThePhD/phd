// Copyright � 2018 ThePhD

#pragma once

#ifndef PHD_OPTIONAL_BENCHMARKS_BENCHMARK_COMPUTE_HPP
#define PHD_OPTIONAL_BENCHMARKS_BENCHMARK_COMPUTE_HPP

#include <vector>

double compute_mean(const std::vector<double>& v);

double compute_variance(const std::vector<double>& v);

double compute_stddev(const std::vector<double>& v);

double compute_index_of_dispersion(const std::vector<double>& v);

double compute_max(const std::vector<double>& v);

double compute_min(const std::vector<double>& v);

uint64_t rdtsc();

struct rdtsc_time {
	uint64_t start;

	rdtsc_time()
	: start(rdtsc()) {
	}

	uint64_t time() const {
		return rdtsc() - start;
	}

	double d_time() const {
		return static_cast<double>(this->time());
	}
};

#endif // PHD_OPTIONAL_BENCHMARKS_BENCHMARK_COMPUTE_HPP