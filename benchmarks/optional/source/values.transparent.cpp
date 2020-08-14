// Copyright � 2018-2020 ThePhD

#include "optional.hpp"
#include "data.hpp"
#include "stats.hpp"
#include "bench.hpp"
#include "ensure.hpp"

#include <benchmark/benchmark.h>

#include <vector>
#include <numeric>

static void int_by_value_transparent_success(benchmark::State& state) {
	size_t expected_value = 0;
	{
		// set up initial expected value
		tl::optional<size_t> maybe_value = int_value;
		expected_value = bench_transparent_pure_int(maybe_value);
	}
	size_t x = 0;
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<size_t> maybe_value = int_value;
		state.ResumeTiming();
		x += bench_transparent_pure_int(maybe_value);
	}
	return;
}

static void int_3x_by_value_transparent_success(benchmark::State& state) {
	size_t expected_value = 0;
	{
		// set up initial expected value
		tl::optional<size_t> maybe_value = int_value;
		expected_value = bench_transparent_pure_int_3x(maybe_value);
	}
	size_t x = 0;
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<size_t> maybe_value = int_value;
		state.ResumeTiming();
		x += bench_transparent_pure_int_3x(maybe_value);
	}
	if (!ensure(state, x, state.iterations() * expected_value)) {
		return;
	}
}

static void vector_by_value_transparent_success(benchmark::State& state) {
	std::vector<size_t> initial_value = create_random_vector();
	size_t expected_value = 0;
	{
		// set up initial expected value
		tl::optional<std::vector<size_t>> maybe_value = initial_value;
		expected_value = bench_transparent_pure_vector(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<std::vector<size_t>> maybe_value = initial_value;
		state.ResumeTiming();
		size_t x = bench_transparent_pure_vector(maybe_value);
		state.PauseTiming();
		ensure(state, x, expected_value);
		state.ResumeTiming();
	}
}

static void vector_3x_by_value_transparent_success(benchmark::State& state) {
	std::vector<size_t> initial_value = create_random_vector();
	size_t expected_value = 0;
	{
		// set up initial expected value
		tl::optional<std::vector<size_t>> maybe_value = initial_value;
		expected_value = bench_transparent_pure_vector_3x(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<std::vector<size_t>> maybe_value = initial_value;
		state.ResumeTiming();
		size_t x = bench_transparent_pure_vector_3x(maybe_value);
		state.PauseTiming();
		ensure(state, x, expected_value);
		state.ResumeTiming();
	}
}

BENCHMARK(int_by_value_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(int_3x_by_value_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(vector_by_value_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(vector_3x_by_value_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
