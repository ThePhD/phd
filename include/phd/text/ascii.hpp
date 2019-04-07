#pragma once

#ifndef PHD_TEXT_ASCII_HPP
#define PHD_TEXT_ASCII_HPP

#include <phd/text/char8_t.hpp>
#include <phd/text/unicode_code_point.hpp>
#include <phd/text/code_point.hpp>

#include <cstddef>
#include <utility>
#include <array>

namespace phd {

	struct ascii {
		using code_unit = char;
		using encoding_code_point = code_point;
		using decoding_code_point = code_point;

		static std::pair<std::array<code_unit, 8>, std::size_t> encode_one(encoding_code_point u){

		};

		template <typename Range>
		static std::pair<decoding_code_point, Range> decode_one(Range r){

		};
	};
} // namespace phd

#endif // PHD_TEXT_ASCII_HPP
