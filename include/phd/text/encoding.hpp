#pragma once

#ifndef PHD_TEXT_ENCODING_HPP
#define PHD_TEXT_ENCODING_HPP

#include <phd/text/execution.hpp>
#include <phd/text/wide_execution.hpp>
#include <phd/text/ascii.hpp>
#include <phd/text/utf8.hpp>
#include <phd/text/utf16.hpp>
#include <phd/text/utf32.hpp>

#include <phd/text/text_traits.hpp>
#include <phd/text/error_handler.hpp>

namespace phd {

	template <>
	struct is_bitwise_transcoding_compatible<ascii, utf8> : std::true_type {};

	/* TODO: when latin1 and other ASCII-extending compatible
	encodings are implemented
	template <>
	struct is_bitwise_transcoding_compatible<ascii, latin-X> : std::true_type {};
	*/

} // namespace phd

#endif // PHD_TEXT_ENCODING_HPP
