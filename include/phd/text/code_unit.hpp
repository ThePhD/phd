#pragma once

#ifndef PHD_TEXT_CODE_UNIT_HPP
#define PHD_TEXT_CODE_UNIT_HPP

namespace phd {

	template <typename __T>
	struct encoding_code_unit {
		using type = typename __T::code_unit;
	};

	template <typename __T>
	using encoding_code_unit_t = typename encoding_code_unit<__T>::type;

} // namespace phd

#endif // PHD_TEXT_CODE_UNIT_HPP
