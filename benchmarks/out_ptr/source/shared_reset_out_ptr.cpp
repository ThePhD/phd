#include <benchmarks/statistics.hpp>

#include <benchmark/benchmark.h>

#include <phd/out_ptr/out_ptr.hpp>
#include <phd/out_ptr/noexcept_out_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <memory>
#include <cstdlib>


static void manual_shared_reset_out_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::shared_ptr<ficapi::opaque> p(nullptr, ficapi::handle_no_alloc_deleter());
	for (auto _ : state) {
		ficapi_opaque_handle temp_p = NULL;
		ficapi_handle_no_alloc_create(&temp_p);
		p.reset(temp_p, ficapi::handle_no_alloc_deleter());
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(manual_shared_reset_out_ptr)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void out_ptr_shared_reset_out_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::shared_ptr<ficapi::opaque> p(nullptr, ficapi::handle_no_alloc_deleter());
	for (auto _ : state) {
		ficapi_handle_no_alloc_create(phd::out_ptr::out_ptr(p, ficapi::handle_no_alloc_deleter()));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(out_ptr_shared_reset_out_ptr)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);


static void noexcept_shared_reset_out_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::shared_ptr<ficapi::opaque> p(nullptr, ficapi::handle_no_alloc_deleter());
	for (auto _ : state) {
		ficapi_handle_no_alloc_create(phd::out_ptr::detail::noexcept_out_ptr(p, ficapi::handle_no_alloc_deleter()));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(out_ptr_shared_reset_out_ptr)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
