#pragma once

#ifndef PHD_TEXT_WIDE_EXECUTION_HPP
#define PHD_TEXT_WIDE_EXECUTION_HPP

#include <phd/text/unicode_code_point.hpp>

#include <cuchar>
#include <cwchar>

namespace phd {

	struct wide_execution {
		struct state {
			std::mbstate_t wide_state;
			std::mbstate_t narrow_state;

			state() = default;
		};

		using code_unit = wchar_t;
		using code_point = unicode_code_point;
	};

} // namespace phd

#endif // PHD_TEXT_WIDE_EXECUTION_HPP
