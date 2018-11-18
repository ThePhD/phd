#pragma once

#ifndef PHD_TEXT_TEXT_HPP
#define PHD_TEXT_TEXT_HPP

#include <phd/text/basic_text.hpp>
#include <phd/text/encoding.hpp>
#include <phd/text/basic_c_string_view.hpp>
#include <phd/text/c_string_view.hpp>
#include <phd/text/basic_string_literal.hpp>
#include <phd/text/string_literal.hpp>
#include <phd/text/char8_t.hpp>

#include <phd/text/text_view.hpp>


namespace phd {

	using text = basic_text_view<execution>;
	using wtext = basic_text_view<wide_execution>;
	using u8text = basic_text_view<utf8>;
	using u16text = basic_text_view<utf16>;
	using u32text = basic_text_view<utf32>;

} // namespace phd

#endif // PHD_TEXT_TEXT_HPP
