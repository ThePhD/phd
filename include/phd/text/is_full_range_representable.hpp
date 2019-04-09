#pragma once

#ifndef PHD_TEXT_IS_FULL_RANGE_REPRESENTABLE_HPP
#define PHD_TEXT_IS_FULL_RANGE_REPRESENTABLE_HPP

#include <phd/text/text_forward.hpp>
#include <phd/meta/is_detected.hpp>

#include <type_traits>

namespace phd {

	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename T>
		using __is_encode_lossless_test = decltype(T::is_encode_lossless);

		template <typename T>
		using __is_decode_lossless_test = decltype(T::is_decode_lossless);

		template <typename, typename = void>
		struct __is_full_range_representable_sfinae : std::false_type {};

		template <typename T>
		struct __is_full_range_representable_sfinae<T, std::enable_if_t<is_detected_v<__is_decode_lossless_test, T> && is_detected_v<__is_encode_lossless_test, T>>> : std::integral_constant<bool, T::is_encode_lossless::value && T::is_decode_lossless::value> {};
	}
	} // namespace __abi_v0::__text_detail

	template <typename T>
	struct is_full_range_representable : __text_detail::__is_full_range_representable_sfinae<T> {};

	template <typename T>
	constexpr inline bool is_full_range_representable_v = is_full_range_representable<T>::value;

} // namespace phd

#endif // PHD_TEXT_IS_FULL_RANGE_REPRESENTABLE_HPP
