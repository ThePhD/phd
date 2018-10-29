// Copyright � 2018 ThePhD

#include "ensure.hpp"
#include "data.hpp"
#include "stats.hpp"
#include "optional.hpp"
#include "bench.hpp"

#include <benchmark/benchmark.h>
#include <barrier/barrier.hpp>

#include <algorithm>
#include <numeric>
#include <random>

static void int_by_ref_transparent_success(benchmark::State& state) {
	size_t x = 0;
	size_t expected_value = 0;
	{
		size_t ref_value = int_value;
		tl::optional<size_t&> maybe_value = ref_value;
		expected_value = bench_transparent_inplace_int(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		size_t ref_value = int_value;
		tl::optional<size_t&> maybe_value = ref_value;
		state.ResumeTiming();
		x += bench_transparent_inplace_int(maybe_value);
	}
	if (!ensure(state, x, state.iterations() * expected_value)) {
		return;
	}
}

static void int_3x_by_ref_transparent_success(benchmark::State& state) {
	size_t x = 0;
	size_t expected_value = 0;
	{
		size_t ref_value = int_value;
		tl::optional<size_t&> maybe_value = ref_value;
		expected_value = bench_transparent_inplace_int_3x(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		size_t ref_value = int_value;
		tl::optional<size_t&> maybe_value = ref_value;
		state.ResumeTiming();
		x += bench_transparent_inplace_int_3x(maybe_value);
	}
	if (!ensure(state, x, state.iterations() * expected_value)) {
		return;
	}
}

static void vector_by_ref_transparent_success(benchmark::State& state) {
	std::vector<size_t> initial_value = create_random_vector();

	size_t expected_value = 0;
	{
		// set up initial expected value
		std::vector<size_t> value = initial_value;
		tl::optional<std::vector<size_t>&> maybe_value = value;
		expected_value = bench_transparent_inplace_vector(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		std::vector<size_t> value = initial_value;
		tl::optional<std::vector<size_t>&> maybe_value = value;
		state.ResumeTiming();
		size_t x = bench_transparent_inplace_vector(maybe_value);
		state.PauseTiming();
		ensure(state, x, expected_value);
		state.ResumeTiming();
	}
}

static void vector_3x_by_ref_transparent_success(benchmark::State& state) {
	std::vector<size_t> initial_value = create_random_vector();

	size_t expected_value = 0;
	{
		// set up initial expected value
		std::vector<size_t> value = initial_value;
		tl::optional<std::vector<size_t>&> maybe_value = value;
		expected_value = bench_transparent_inplace_vector(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		std::vector<size_t> value = initial_value;
		tl::optional<std::vector<size_t>&> maybe_value = value;
		state.ResumeTiming();
		size_t x = bench_transparent_inplace_vector(maybe_value);
		state.PauseTiming();
		ensure(state, x, expected_value);
		state.ResumeTiming();
	}
}

BENCHMARK(int_by_ref_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(int_3x_by_ref_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(vector_by_ref_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(vector_3x_by_ref_transparent_success)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
