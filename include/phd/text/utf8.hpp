#pragma once

#ifndef PHD_TEXT_UTF8_HPP
#define PHD_TEXT_UTF8_HPP

#include <phd/text/char8_t.hpp>

namespace phd {

	struct utf8 {
		using code_unit = char8_t;
	};

} // namespace phd

#endif // PHD_TEXT_UTF8_HPP
