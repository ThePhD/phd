#pragma once

#ifndef PHD_TEXT_LATIN1_HPP
#define PHD_TEXT_LATIN1_HPP

#include <phd/text/char8_t.hpp>

#include <cstddef>

namespace phd {

	struct utf8 {
		using code_unit = char8_t;
		using encoding_code_point = code_point;
		using decoding_code_point = code_point;

		static std::pair<std::array<code_unit, 8>, std::size_t> encode_one(encoding_code_point u){

		};

		template <typename BIt, typename EIt>
		static std::pair<decoding_code_point, It> decode_one(BIt first, EIt last){

		};
	};
} // namespace phd

#endif // PHD_TEXT_LATIN1_HPP
