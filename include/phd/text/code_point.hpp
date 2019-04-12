#pragma once

#ifndef PHD_TEXT_CODE_POINT_HPP
#define PHD_TEXT_CODE_POINT_HPP

#include <phd/text/unicode_code_point.hpp>

namespace phd {

	template <typename __T>
	struct encoding_code_point {
		using type = typename __T::code_point;
	};

	template <typename __T>
	using encoding_code_point_t = typename encoding_code_point<__T>::type;

} // namespace phd

#endif // PHD_TEXT_CODE_POINT_HPP
