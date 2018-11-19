#pragma once

#ifndef PHD_TEXT_STRING_LITERAL_HPP
#define PHD_TEXT_STRING_LITERAL_HPP

#include <phd/text/basic_string_literal.hpp>

#ifndef __cpp_impl_string_literal

namespace phd {

	template <size_t N>
	class string_literal : public basic_string_literal<char, N> {
	private:
		using base_t = basic_string_literal<char, N>;

	public:
		using base_t::base_t;
	};

	template <size_t N>
	class wstring_literal : public basic_string_literal<wchar_t, N> {
	private:
		using base_t = basic_string_literal<wchar_t, N>;

	public:
		using base_t::base_t;
	};

	template <size_t N>
	class u8string_literal : public basic_string_literal<char8_t, N> {
	private:
		using base_t = basic_string_literal<char8_t, N>;

	public:
		using base_t::base_t;
	};

	template <size_t N>
	class u16string_literal : public basic_string_literal<char16_t, N> {
	private:
		using base_t = basic_string_literal<char16_t, N>;

	public:
		using base_t::base_t;
	};

	template <size_t N>
	class u32string_literal : public basic_string_literal<char32_t, N> {
	private:
		using base_t = basic_string_literal<char32_t, N>;

	public:
		using base_t::base_t;
	};

	template <size_t N>
	string_literal(const char (&)[N])->string_literal<N>;

	template <size_t N>
	wstring_literal(const wchar_t (&)[N])->wstring_literal<N>;

	template <size_t N>
	u8string_literal(const char8_t (&)[N])->u8string_literal<N>;

	template <size_t N>
	u16string_literal(const char16_t (&)[N])->u16string_literal<N>;

	template <size_t N>
	u32string_literal(const char32_t (&)[N])->u32string_literal<N>;

} // namespace phd

#endif //__cpp_impl_string_literal

#endif // PHD_TEXT_STRING_LITERAL_HPP
