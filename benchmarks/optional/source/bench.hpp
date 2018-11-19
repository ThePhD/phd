// Copyright � 2018 ThePhD

#ifndef PHD_OPTIONAL_BENCHMARKS_BENCH_HPP
#define PHD_OPTIONAL_BENCHMARKS_BENCH_HPP

#include "optional.hpp"
#include "data.hpp"
#include <barrier/barrier.hpp>

#include <numeric>

#if defined(__clang__) || defined(__GNUC__)
#define PHD_OPT_BENCH_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define PHD_OPT_BENCH_INLINE __forceinline
#else
#define PHD_OPT_BENCH_INLINE
#endif // forced inline

template <typename T, typename F, F transform>
inline PHD_OPT_BENCH_INLINE size_t bench_transform(tl::optional<T>& maybe_value) {
	size_t x = 0;
	tl::optional<T> post_maybe_value = maybe_value.map(transform);
	if (post_maybe_value) {
		std::add_lvalue_reference_t<T> acc = post_maybe_value.value();
		if constexpr (std::is_arithmetic<std::remove_reference_t<T>>::value) {
			x += post_maybe_value.value();
		}
		else {
			typedef typename std::remove_reference_t<T>::value_type value_type;
			x += std::accumulate(acc.begin(), acc.end(), value_type(0));
		}
	}
	else {
		if constexpr (std::is_arithmetic<std::remove_reference_t<T>>::value) {
			x += int_fail_value;
		}
		else {
			x += vector_fail_value;
		}
	}
	return x;
}

template <typename T, typename F, F transform>
inline PHD_OPT_BENCH_INLINE size_t bench_transform_3x(tl::optional<T>& maybe_value) {
	size_t x = 0;
	tl::optional<T> post_maybe_value = maybe_value.map(transform)
									.map(transform)
									.map(transform);
	if (post_maybe_value) {
		std::add_lvalue_reference_t<T> acc = post_maybe_value.value();
		if constexpr (std::is_arithmetic<std::remove_reference_t<T>>::value) {
			x += post_maybe_value.value();
		}
		else {
			typedef typename std::remove_reference_t<T>::value_type value_type;
			x += std::accumulate(acc.begin(), acc.end(), value_type(0));
		}
	}
	else {
		if constexpr (std::is_arithmetic<std::remove_reference_t<T>>::value) {
			x += int_fail_value;
		}
		else {
			x += vector_fail_value;
		}
	}
	return x;
}

inline PHD_OPT_BENCH_INLINE size_t bench_pure_int(tl::optional<size_t>& maybe_value) {
	return bench_transform<size_t, decltype(&barrier::pure_transform_int), &barrier::pure_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_inplace_int(tl::optional<size_t&>& maybe_value) {
	return bench_transform<size_t&, decltype(&barrier::inplace_transform_int), &barrier::inplace_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_pure_int_3x(tl::optional<size_t>& maybe_value) {
	return bench_transform_3x<size_t, decltype(&barrier::pure_transform_int), &barrier::pure_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_inplace_int_3x(tl::optional<size_t&>& maybe_value) {
	return bench_transform_3x<size_t&, decltype(&barrier::inplace_transform_int), &barrier::inplace_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_pure_vector(tl::optional<std::vector<size_t>>& maybe_value) {
	return bench_transform<std::vector<size_t>, decltype(&barrier::pure_transform_vector), &barrier::pure_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_inplace_vector(tl::optional<std::vector<size_t>&>& maybe_value) {
	return bench_transform<std::vector<size_t>&, decltype(&barrier::inplace_transform_vector), &barrier::inplace_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_pure_vector_3x(tl::optional<std::vector<size_t>>& maybe_value) {
	return bench_transform_3x<std::vector<size_t>, decltype(&barrier::pure_transform_vector), &barrier::pure_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_inplace_vector_3x(tl::optional<std::vector<size_t>&>& maybe_value) {
	return bench_transform_3x<std::vector<size_t>&, decltype(&barrier::inplace_transform_vector), &barrier::inplace_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_pure_int(tl::optional<size_t>& maybe_value) {
	return bench_transform<size_t, decltype(&transparent::pure_transform_int), &transparent::pure_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_inplace_int(tl::optional<size_t&>& maybe_value) {
	return bench_transform<size_t&, decltype(&transparent::inplace_transform_int), &transparent::inplace_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_pure_int_3x(tl::optional<size_t>& maybe_value) {
	return bench_transform_3x<size_t, decltype(&transparent::pure_transform_int), &transparent::pure_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_inplace_int_3x(tl::optional<size_t&>& maybe_value) {
	return bench_transform_3x<size_t&, decltype(&transparent::inplace_transform_int), &transparent::inplace_transform_int>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_pure_vector(tl::optional<std::vector<size_t>>& maybe_value) {
	return bench_transform<std::vector<size_t>, decltype(&transparent::pure_transform_vector), &transparent::pure_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_inplace_vector(tl::optional<std::vector<size_t>&>& maybe_value) {
	return bench_transform<std::vector<size_t>&, decltype(&transparent::inplace_transform_vector), &transparent::inplace_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_pure_vector_3x(tl::optional<std::vector<size_t>>& maybe_value) {
	return bench_transform_3x<std::vector<size_t>, decltype(&transparent::pure_transform_vector), &transparent::pure_transform_vector>(maybe_value);
}

inline PHD_OPT_BENCH_INLINE size_t bench_transparent_inplace_vector_3x(tl::optional<std::vector<size_t>&>& maybe_value) {
	return bench_transform_3x<std::vector<size_t>&, decltype(&transparent::inplace_transform_vector), &transparent::inplace_transform_vector>(maybe_value);
}

#endif // PHD_OPTIONAL_BENCHMARKS_BENCH_HPP
