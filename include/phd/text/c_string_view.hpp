#pragma once

#ifndef PHD_TEXT_C_STRING_VIEW_HPP
#define PHD_TEXT_C_STRING_VIEW_HPP

#include <phd/text/basic_c_string_view.hpp>
#include <phd/text/char8_t.hpp>

namespace phd {

	using c_string_view = basic_c_string_view<char>;
	using wc_string_view = basic_c_string_view<wchar_t>;
	using u8c_string_view = basic_c_string_view<char8_t>;
	using u16c_string_view = basic_c_string_view<char16_t>;
	using u32c_string_view = basic_c_string_view<char32_t>;

	inline namespace literals {
	inline namespace string_view_literals {
		// suffix for basic_c_string_view literals
		constexpr inline c_string_view operator"" _csv(const char* str_, size_t len_) noexcept {
			return c_string_view(str_, len_);
		}
#if defined(__cpp_char_8t)
		constexpr inline u8c_string_view operator"" _csv(const char8_t* str_, size_t len_) noexcept {
			return u8c_string_view(str_, len_);
		}
#else
		constexpr inline u8c_string_view operator"" _cu8sv(const char* str_, size_t len_) noexcept {
			return u8c_string_view(reinterpret_cast<const char8_t*>(str_), len_);
		}
#endif // char8_t hacks
		constexpr inline u16c_string_view operator"" _csv(const char16_t* str_, size_t len_) noexcept {
			return u16c_string_view(str_, len_);
		}
		constexpr inline u32c_string_view operator"" _csv(const char32_t* str_, size_t len_) noexcept {
			return u32c_string_view(str_, len_);
		}
		constexpr inline wc_string_view operator"" _csv(const wchar_t* str_, size_t len_) noexcept {
			return wc_string_view(str_, len_);
		}
	}} // namespace literals::string_view_literals

} // namespace phd

#endif // PHD_TEXT_C_STRING_VIEW_HPP
