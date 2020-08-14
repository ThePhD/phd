// Copyright � 2018-2020 ThePhD

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

static void int_by_ref_failure(benchmark::State& state) {
	std::size_t expected_value;
	{
		tl::optional<size_t&> maybe_value = tl::nullopt;
		expected_value = bench_inplace_int(maybe_value);
	}
	size_t x = 0;
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<size_t&> maybe_value = tl::nullopt;
		state.ResumeTiming();
		x += bench_inplace_int(maybe_value);
	}
	if (!ensure(state, x, state.iterations() * expected_value)) {
		return;
	}
}

static void int_3x_by_ref_failure(benchmark::State& state) {
	std::size_t expected_value;
	{
		tl::optional<size_t&> maybe_value = tl::nullopt;
		expected_value = bench_inplace_int_3x(maybe_value);
	}
	size_t x = 0;
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<size_t&> maybe_value = tl::nullopt;
		state.ResumeTiming();
		x += bench_inplace_int_3x(maybe_value);
	}
	if (!ensure(state, x, state.iterations() * expected_value)) {
		return;
	}
}

static void vector_by_ref_failure(benchmark::State& state) {
	std::size_t expected_value;
	{
		tl::optional<std::vector<size_t>&> maybe_value = tl::nullopt;
		expected_value = bench_inplace_vector(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<std::vector<size_t>&> maybe_value = tl::nullopt;
		state.ResumeTiming();
		size_t x = bench_inplace_vector(maybe_value);
		state.PauseTiming();
		ensure(state, x, expected_value);
		state.ResumeTiming();
	}
}

static void vector_3x_by_ref_failure(benchmark::State& state) {
	std::size_t expected_value;
	{
		tl::optional<std::vector<size_t>&> maybe_value = tl::nullopt;
		expected_value = bench_inplace_vector(maybe_value);
	}
	for (auto _ : state) {
		state.PauseTiming();
		tl::optional<std::vector<size_t>&> maybe_value = tl::nullopt;
		state.ResumeTiming();
		size_t x = bench_inplace_vector(maybe_value);
		state.PauseTiming();
		ensure(state, x, expected_value);
		state.ResumeTiming();
	}
}

BENCHMARK(int_by_ref_failure)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(int_3x_by_ref_failure)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(vector_by_ref_failure)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(vector_3x_by_ref_failure)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
