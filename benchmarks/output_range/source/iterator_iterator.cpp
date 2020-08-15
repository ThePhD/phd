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

namespace {
	struct fn_output_iterator_sentinel {};

	template <typename Fx>
	struct fn_output_iterator {
		std::decay_t<Fx> target_call_;

		struct fn_output_iterator_ref {
			std::reference_wrapper<std::decay_t<Fx>> target_call_ref_;

			fn_output_iterator_ref (std::decay_t<Fx>& f) noexcept : target_call_ref_(f) {
			}

			template <typename Value, std::enable_if_t<!std::is_same_v<std::decay_t<Value>, fn_output_iterator_ref>, int*> = nullptr>
			fn_output_iterator_ref& operator=(Value&& value) noexcept(noexcept((target_call_ref_.get())(std::forward<Value>(value)))) {
				std::decay_t<Fx>& target_call = this->target_call_ref_.get();
				target_call(std::forward<Value>(value));
				return *this;
			}
		};

		template <typename Arg, typename... Args, std::enable_if_t<!std::is_same_v<std::decay_t<Arg>, fn_output_iterator>, int*> = nullptr>
		fn_output_iterator(Arg&& arg, Args&&... args) noexcept : target_call_(std::forward<Arg>(arg), std::forward<Args>(args)...) {}

		fn_output_iterator& operator++() noexcept {
			return *this;
		}

		fn_output_iterator operator++(int) noexcept {
			fn_output_iterator old = *this;
			++(*this);
			return old;
		}

		fn_output_iterator_ref operator*() noexcept {
			return fn_output_iterator_ref(target_call_);
		}
	};

	template <typename Fx>
	bool operator==(const fn_output_iterator<Fx>& left, const fn_output_iterator_sentinel&) {
		return false;
	}

	template <typename Fx>
	bool operator!=(const fn_output_iterator<Fx>& left, const fn_output_iterator_sentinel&) {
		return true;
	}
}

static void iterator_iterator_copy_transparent(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		fn_output_iterator<transparent::write> sink(transparent::write{destination_data.data()});
		std::copy(source_data.cbegin(), source_data.cend(), sink);
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

static void iterator_iterator_copy_barrier(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		fn_output_iterator<barrier::write> sink(barrier::write{destination_data.data()});
		std::copy(source_data.cbegin(), source_data.cend(), sink);
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

static void iterator_iterator_copy_inline(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		auto sink_fn = [d = destination_data.begin()](std::size_t i) mutable {
			*d++ = i;
		};
		fn_output_iterator<decltype(std::ref(sink_fn))> sink(std::ref(sink_fn));
		std::copy(source_data.cbegin(), source_data.cend(), sink);
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

static void iterator_iterator_copy_direct(benchmark::State& state) {
	const std::vector<std::size_t>& source_data = get_constant_random_vector();
	std::vector<std::size_t> destination_data(source_data.size(), static_cast<std::size_t>(0));
	for (auto _ : state) {
		std::copy(source_data.cbegin(), source_data.cend(), destination_data.begin());
		benchmark::DoNotOptimize(*destination_data.begin());
	}
	if (!ensure(state, source_data, destination_data)) {
		return;
	}
}

BENCHMARK(iterator_iterator_copy_transparent)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(iterator_iterator_copy_barrier)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(iterator_iterator_copy_inline)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);

BENCHMARK(iterator_iterator_copy_direct)
	->ComputeStatistics("max", &compute_max)
	->ComputeStatistics("min", &compute_min)
	->ComputeStatistics("variance", &compute_variance)
	->ComputeStatistics("dispersion", &compute_index_of_dispersion);
