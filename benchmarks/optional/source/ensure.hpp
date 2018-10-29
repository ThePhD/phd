// Copyright � 2018 ThePhD

#ifndef PHD_OPTIONAL_BENCHMARKS_ENSURE_HPP
#define PHD_OPTIONAL_BENCHMARKS_ENSURE_HPP

#include <benchmark/benchmark.h>

template <typename Left, typename Right>
bool ensure(benchmark::State& state, Left&& l, Right&& r) {
	if (l != r) {
		state.SkipWithError("unexpected result");
		return false;
	}
	return true;
}

#endif // PHD_OPTIONAL_BENCHMARKS_ENSURE_HPP
