#pragma once

#ifndef PHD_TEXT_BASIC_C_STRING_VIEW_HPP
#define PHD_TEXT_BASIC_C_STRING_VIEW_HPP

#include <phd/meta/remove_cv_ref.hpp>
#include <phd/meta/always.hpp>
#include <phd/text/text_forward.hpp>
#include <phd/text/char8_t.hpp>

#include <string_view>
#include <type_traits>
#include <cassert>

namespace phd {

	namespace string_detail {
#ifdef __cpp_char8_t
#else
		using arr8_t = char8_t[1];
		constexpr inline const arr8_t u8_shim = {};
#endif

		template <typename C>
		constexpr inline decltype(auto) empty_string() noexcept {
			static_assert(meta::always_false_v<C>, "unrecognized character type");
			return "";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char>() noexcept {
			return "";
		}

		template <>
		constexpr inline decltype(auto) empty_string<wchar_t>() noexcept {
			return L"";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char8_t>() noexcept {
#ifdef __cpp_char8_t
			return u8"";
#else
			return (u8_shim);
#endif
		}

		template <>
		constexpr inline decltype(auto) empty_string<char16_t>() noexcept {
			return u"";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char32_t>() noexcept {
			return U"";
		}
	} // namespace string_detail

	template <typename CharType, typename Traits = std::char_traits<CharType>>
	class basic_c_string_view : public std::basic_string_view<CharType, Traits> {
	private:
		using base_t = std::basic_string_view<CharType, Traits>;

	public:
		constexpr basic_c_string_view()
		: basic_c_string_view(string_detail::empty_string<CharType>(), 0) {
		}

		template <typename Arg0, typename Arg1, typename... Args>
		constexpr basic_c_string_view(Arg0&& arg0_, Arg1&& arg1_, Args&&... args_)
		: base_t(std::forward<Arg0>(arg0_), std::forward<Arg1>(arg1_), std::forward<Args>(args_)...) {
			assert((this->data() ? *(this->data() + this->size()) == '\0' : false) && "non null-terminated c_string_view!");
		}

		template <typename Arg, typename = std::enable_if_t<!std::is_same_v<meta::remove_cv_ref_t<Arg>, basic_c_string_view>>>
		constexpr basic_c_string_view(Arg&& arg_)
		: base_t(std::forward<Arg>(arg_)) {
			assert((this->data() ? *this->end() == '\0' : false) && "non null-terminated c_string_view!");
		}

		constexpr basic_c_string_view(basic_c_string_view&& o) = default;
		constexpr basic_c_string_view(const basic_c_string_view& o) = default;
		constexpr basic_c_string_view& operator=(basic_c_string_view&& o) = default;
		constexpr basic_c_string_view& operator=(const basic_c_string_view& o) = default;
	};

} // namespace phd

namespace std {

	template <typename CharType, typename Traits>
	struct hash<::phd::basic_c_string_view<CharType, Traits>> {

		size_t operator()(const ::phd::basic_c_string_view<CharType, Traits>& c_string_) const {
			hash<basic_string_view<CharType, Traits>> h;
			return h(c_string_);
		}
	};

} // namespace std

#endif // PHD_TEXT_BASIC_C_STRING_VIEW_HPP
