// Copyright � 2018 ThePhD

#include "stats.hpp"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

double compute_mean(const std::vector<double>& v) {
	if (v.size() == 0) {
		return 0.0;
	}
	double mean = std::accumulate(std::begin(v), std::end(v), 0.0) / v.size();
	return mean;
}

double compute_variance(const std::vector<double>& v) {
	if (v.size() == 0) {
		return 0.0;
	}
	double mean = compute_mean(v);
	auto variance_accum = [&mean](double a, double s) {
		double diff = (s - mean);
		return a + (diff * diff);
	};
	double variance = std::accumulate(std::begin(v), std::end(v), 0.0, variance_accum) / v.size();
	return variance;
}

double compute_stddev(const std::vector<double>& v) {
	return std::sqrt(compute_variance(v));
}

double compute_index_of_dispersion(const std::vector<double>& v) {
	double variance = compute_variance(v);
	double mean = compute_mean(v);
	if (mean == 0.0) {
		return 0.0;
	}
	return variance / mean;
}

double compute_max(const std::vector<double>& v) {
	return *(std::max_element(std::begin(v), std::end(v)));
}

double compute_min(const std::vector<double>& v) {
	return *(std::min_element(std::begin(v), std::end(v)));
}

#include <cstdint>

//  Windows
#ifdef _WIN32

#include <intrin.h>
std::uint64_t rdtsc() {
	return __rdtsc();
}
//  Linux/GCC
#else
std::uint64_t rdtsc() {
	unsigned int lo, hi;
	__asm__ __volatile__("rdtsc"
					 : "=a"(lo), "=d"(hi));
	return ((std::uint64_t)hi << 32) | lo;
}
#endif
