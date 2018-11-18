#pragma once

#ifndef PHD_TEXT_TEXT_VIEW_HPP
#define PHD_TEXT_TEXT_VIEW_HPP

#include <phd/text/basic_text_view.hpp>
#include <phd/text/encoding.hpp>
#include <phd/text/basic_c_string_view.hpp>
#include <phd/text/char8_t.hpp>

namespace phd {

	using text_view = basic_text_view<execution>;
	using wtext_view = basic_text_view<wide_execution>;
	using u8text_view = basic_text_view<utf8>;
	using u16text_view = basic_text_view<utf16>;
	using u32text_view = basic_text_view<utf32>;

} // namespace phd

#endif // PHD_TEXT_TEXT_VIEW_HPP
