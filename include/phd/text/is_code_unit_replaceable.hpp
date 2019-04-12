#pragma once

#ifndef PHD_TEXT_IS_CODE_UNIT_REPLACEABLE_HPP
#define PHD_TEXT_IS_CODE_UNIT_REPLACEABLE_HPP

#include <phd/text/text_forward.hpp>
#include <phd/meta/is_detected.hpp>

#include <type_traits>

namespace phd {

	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename __T>
		using __is_code_unit_replaceable_test = decltype(__T::replacement_code_unit);
	}
	} // namespace __abi_v0::__text_detail

	template <typename __T>
	struct is_code_unit_replaceable : is_detected<__text_detail::__is_code_unit_replaceable_test, __T> {};

	template <typename __T>
	constexpr inline bool is_code_unit_replaceable_v = is_code_unit_replaceable<__T>::value;

} // namespace phd

#endif // PHD_TEXT_IS_CODE_UNIT_REPLACEABLE_HPP
