#pragma once

#ifndef PHD_TEXT_UTF32_HPP
#define PHD_TEXT_UTF32_HPP

#include <phd/text/text_forward.hpp>

namespace phd {
	inline namespace __abi_v0 {
	namespace __detail {
		template <typename __CodeUnit = char32_t>
		struct __utf32_with {
			using state = __detail::__empty_state;
			using code_unit = __CodeUnit;
			using code_point = unicode_code_point;
		};
	}
	} // namespace __abi_v0::__detail

	struct utf32 : __detail::__utf32_with<char32_t> {};

} // namespace phd


#endif // PHD_TEXT_UTF32_HPP
