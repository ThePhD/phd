#pragma once

#ifndef PHD_TEXT_CODE_UNIT_HPP
#define PHD_TEXT_CODE_UNIT_HPP

namespace phd {

	template <typename T>
	struct code_unit_of {
		using type = typename T::code_unit;
	};

	template <typename T>
	using code_unit_of_t = typename code_unit_of<T>::type;

} // namespace phd

#endif // PHD_TEXT_CODE_UNIT_HPP
