// Copyright � 2018-2020 ThePhD

#ifndef PHD_OUTPUT_RANGE_BENCHMARKS_ENSURE_HPP
#define PHD_OUTPUT_RANGE_BENCHMARKS_ENSURE_HPP

#include <benchmark/benchmark.h>

template <typename Left, typename Right>
bool ensure(benchmark::State& state, Left&& l, Right&& r) {
	if (l != r) {
		state.SkipWithError("unexpected result");
		return false;
	}
	return true;
}

#endif // PHD_OUTPUT_RANGE_BENCHMARKS_ENSURE_HPP
