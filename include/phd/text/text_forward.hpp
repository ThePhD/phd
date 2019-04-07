#pragma once

#ifndef PHD_TEXT_TEXT_FORWARD_HPP
#define PHD_TEXT_TEXT_FORWARD_HPP

#include <phd/text/char8_t.hpp>

namespace phd {

	inline namespace __abi_v0 {
	namespace __detail {
		template <typename __CharT>
		struct __utf8_with;

		template <typename __CharT>
		struct __utf16_with;

		template <typename __CharT>
		struct __utf32_with;
	}
	} // namespace __abi_v0::__detail

	struct execution;
	struct wide_execution;
	struct utf8;
	struct utf16;
	struct utf32;

	struct nfc;
	struct nfd;
	struct nfkc;
	struct nfkd;

	template <typename CharType, typename Traits>
	class basic_c_string_view;

	template <typename Encoding, typename NormalizationForm, typename View>
	class basic_text_view;

	template <typename Encoding, typename NormalizationForm, typename Container>
	class basic_text;

} // namespace phd

#endif // PHD_TEXT_TEXT_FORWARD_HPP
