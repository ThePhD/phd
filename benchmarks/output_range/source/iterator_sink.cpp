// Copyright � 2018-2020 ThePhD

#include "optional.hpp"
#include "data.hpp"
#include "stats.hpp"
#include "ensure.hpp"
#include "sink_copy.hpp"

#include <barrier/barrier.hpp>
#include <benchmark/benchmark.h>

#include <vector>
#include <numeric>

static void iterator_sink_copy_transparent(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		transparent::write sink{destination_data.data()};
		iterator_sink_copy(source_data.cbegin(), source_data.cend(), sink);
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

static void iterator_sink_copy_barrier(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		barrier::write sink{destination_data.data()};
		iterator_sink_copy(source_data.cbegin(), source_data.cend(), sink);
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

static void iterator_sink_copy_inline(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		auto sink = [d = destination_data.begin()](std::size_t i) mutable {
			*d++ = i;
		};
		iterator_sink_copy(source_data.cbegin(), source_data.cend(), sink);
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

BENCHMARK(iterator_sink_copy_transparent)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(iterator_sink_copy_barrier)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(iterator_sink_copy_inline)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
