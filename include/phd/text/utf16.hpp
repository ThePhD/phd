#pragma once

#ifndef PHD_TEXT_UTF16_HPP
#define PHD_TEXT_UTF16_HPP

#include <phd/text/text_forward.hpp>

namespace phd {

	inline namespace __abi_v0 {
	namespace __detail {
		template <typename __CodeUnit = char16_t, bool surrogates_allowed = false>
		struct __utf16_with {
			using state = __detail::__empty_state;
			using code_unit = __CodeUnit;
			using code_point = unicode_code_point;
		};
	}
	} // namespace __abi_v0::__detail

	struct utf16 : __detail::__utf16_with<char16_t> {};

} // namespace phd

#endif // PHD_TEXT_UTF16_HPP
