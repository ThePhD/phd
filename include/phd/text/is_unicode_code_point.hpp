#pragma once

#ifndef PHD_TEXT_IS_UNICODE_POINT_HPP
#define PHD_TEXT_IS_UNICODE_POINT_HPP

#include <phd/text/text_forward.hpp>

#include <type_traits>

namespace phd {

	template <typename T>
	struct is_unicode_code_point : std::integral_constant<bool, std::is_same_v<T, unicode_code_point> || std::is_same_v<T, unicode_scalar_value>> {};

	template <typename T>
	constexpr inline bool is_unicode_code_point_v = is_unicode_code_point<T>::value;

} // namespace phd

#endif // PHD_TEXT_IS_UNICODE_POINT_HPP
