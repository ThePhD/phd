#pragma once

#ifndef PHD_TEXT_IS_FULL_RANGE_REPRESENTABLE_HPP
#define PHD_TEXT_IS_FULL_RANGE_REPRESENTABLE_HPP

#include <phd/text/text_forward.hpp>
#include <phd/meta/is_detected.hpp>

#include <type_traits>

namespace phd {

	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename __T>
		using __is_encode_injective_test = decltype(T::is_encode_injective);

		template <typename __T>
		using __is_decode_injective_test = decltype(T::is_decode_injective);

		template <typename, typename = void>
		struct __is_full_range_representable_sfinae : std::false_type {};

		template <typename __T>
		struct __is_full_range_representable_sfinae<__T, std::enable_if_t<is_detected_v<__is_decode_injective_test, __T> && is_detected_v<__is_encode_injective_test, __T>>> : std::integral_constant<bool, __T::is_encode_injective::value && __T::is_decode_injective::value> {};
	}
	} // namespace __abi_v0::__text_detail

	template <typename __T>
	struct is_full_range_representable : __text_detail::__is_full_range_representable_sfinae<__T> {};

	template <typename __T>
	constexpr inline bool is_full_range_representable_v = is_full_range_representable<__T>::value;

} // namespace phd

#endif // PHD_TEXT_IS_FULL_RANGE_REPRESENTABLE_HPP
