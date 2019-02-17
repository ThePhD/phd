#pragma once

#ifndef PHD_TEXT_UTF8_HPP
#define PHD_TEXT_UTF8_HPP

#include <phd/text/char8_t.hpp>
#include <phd/text/unicode_code_point.hpp>

#include <cstddef>
#include <array>

namespace phd {

	struct utf8 {
		using code_unit = char8_t;
		using encoding_code_point = unicode_code_point;
		using decoding_code_point = unicode_code_point;

		static std::pair<std::array<code_unit, 8>, std::size_t> encode_one(encoding_code_point u){

		};

		template <typename It, typename Sentinel>
		static std::pair<decoding_code_point, It> decode_one(It first, Sentinel last){

		};
	};
} // namespace phd

#endif // PHD_TEXT_UTF8_HPP
