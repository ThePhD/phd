// Copyright � 2018-2020 ThePhD

#include <benchmark/benchmark.h>

int main(int argc, char** argv) {
	::benchmark::Initialize(&argc, argv);
	if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
		return 1;
	}
	::benchmark::RunSpecifiedBenchmarks();

	return 0;
}