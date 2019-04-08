#pragma once

#ifndef PHD_TEXT_IS_CHARACTER_REPLACEABLE_HPP
#define PHD_TEXT_IS_CHARACTER_REPLACEABLE_HPP

#include <phd/text/text_forward.hpp>
#include <phd/meta/is_detected.hpp>

#include <type_traits>

namespace phd {

	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename T>
		using __is_code_point_replaceable_test = decltype(T::replacement_code_point);
	}
	} // namespace __abi_v0::__text_detail

	template <typename T>
	struct is_code_point_replaceable : is_detected<__text_detail::__is_code_point_replaceable_test, T> {};

	template <typename T>
	constexpr inline bool is_code_point_replaceable_v = is_code_point_replaceable<T>::value;

} // namespace phd

#endif // PHD_TEXT_IS_CHARACTER_REPLACEABLE_HPP
