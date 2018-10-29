// Copyright � 2018 ThePhD

#include "ensure.hpp"
#include "data.hpp"
#include "stats.hpp"
#include "optional.hpp"
#include "bench.hpp"

#include <benchmark/benchmark.h>
#include <barrier/barrier.hpp>

static void noop(benchmark::State& state) {
	for (auto _ : state) {
	}
}

BENCHMARK(noop)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
