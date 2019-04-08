#pragma once

#ifndef PHD_TEXT_EXECUTION_HPP
#define PHD_TEXT_EXECUTION_HPP

#include <phd/text/encoding_result.hpp>
#include <phd/text/encoding_error.hpp>
#include <phd/text/unicode_code_point.hpp>

#include <cuchar>
#include <cwchar>

namespace phd {

	struct execution {
		struct state {
			std::mbstate_t narrow_state;
		};

		using code_unit = char;
		using code_point = unicode_code_point;
	};

} // namespace phd

#endif // PHD_TEXT_EXECUTION_HPP
