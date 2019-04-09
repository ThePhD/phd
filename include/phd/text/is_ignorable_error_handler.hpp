#pragma once

#ifndef PHD_TEXT_IS_TRANSPARENT_ERROR_HANDLER_HPP
#define PHD_TEXT_IS_TRANSPARENT_ERROR_HANDLER_HPP

#include <phd/text/text_forward.hpp>
#include <phd/meta/is_detected.hpp>

#include <type_traits>

namespace phd {

	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename T>
		using __is_ignorable_error_handler_test = decltype(T::assume_valid);

		template <typename, typename = void>
		struct __is_ignorable_error_handler_sfinae : std::false_type {};

		template <typename T>
		struct __is_ignorable_error_handler_sfinae<T, std::enable_if_t<is_detected_v<__is_ignorable_error_handler_test, T>>> : std::integral_constant<bool, T::assume_valid::value> {};
	}
	} // namespace __abi_v0::__text_detail

	template <typename T>
	struct is_ignorable_error_handler : __text_detail::__is_ignorable_error_handler_sfinae<T> {};

	template <typename T>
	constexpr inline bool is_ignorable_error_handler_v = is_ignorable_error_handler<T>::value;

} // namespace phd

#endif // PHD_TEXT_IS_TRANSPARENT_ERROR_HANDLER_HPP
