#include <benchmarks/statistics.hpp>

#include <benchmark/benchmark.h>

#include <phd/out_ptr/friendly_unique_ptr.hpp>
#include <phd/out_ptr/inout_ptr.hpp>
#include <phd/out_ptr/friendly_inout_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <memory>
#include <cstdlib>

static void c_code_reset_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	ficapi_opaque_handle p = NULL;
	for (auto _ : state) {
		ficapi_handle_no_alloc_re_create(&p);
		x += ficapi_handle_get_data(p);
	}
	if (p != NULL) {
		ficapi_handle_no_alloc_delete(p);
	}
}
BENCHMARK(c_code_reset_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void manual_reset_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
	for (auto _ : state) {
		ficapi_opaque_handle temp_p = p.release();
		ficapi_handle_no_alloc_re_create(&temp_p);
		p.reset(temp_p);
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(manual_reset_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void simple_reset_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
	for (auto _ : state) {
		ficapi_handle_no_alloc_re_create(phd::simple_inout_ptr(p));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(simple_reset_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

static void clever_reset_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
	for (auto _ : state) {
		ficapi_handle_no_alloc_re_create(phd::clever_inout_ptr(p));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(clever_reset_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);


#if defined(PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR) && PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR != 0

static void friendly_reset_inout_ptr(benchmark::State& state) {
	int64_t x = 0;
	std::friendly_unique_ptr<ficapi::opaque, ficapi::handle_no_alloc_deleter> p(nullptr);
	for (auto _ : state) {
		ficapi_handle_no_alloc_re_create(phd::friendly_inout_ptr(p));
		x += ficapi_handle_get_data(p.get());
	}
	int64_t expected = int64_t(state.iterations()) * ficapi_get_data();
	if (x != expected) {
		state.SkipWithError("Unexpected result");
		return;
	}
}
BENCHMARK(friendly_reset_inout_ptr)

	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

#endif // OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR
