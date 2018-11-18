#pragma once

#ifndef PHD_TEXT_TEXT_FORWARD_HPP
#define PHD_TEXT_TEXT_FORWARD_HPP

namespace phd {

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
