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
		using code_point = unicode_code_point;
	};
} // namespace phd

#endif // PHD_TEXT_ASCII_HPP
