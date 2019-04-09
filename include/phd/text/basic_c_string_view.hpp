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

	namespace __string_detail {
#ifdef __cpp_char8_t
#else
		using arr8_t = char8_t[1];
		constexpr inline const arr8_t __u8_shim = {};
#endif

		template <typename C>
		constexpr inline decltype(auto) __empty_string() noexcept {
			static_assert(meta::always_false_v<C>, "unrecognized character type");
			return "";
		}

		template <>
		constexpr inline decltype(auto) __empty_string<char>() noexcept {
			return "";
		}

		template <>
		constexpr inline decltype(auto) __empty_string<wchar_t>() noexcept {
			return L"";
		}

		template <>
		constexpr inline decltype(auto) __empty_string<char8_t>() noexcept {
#ifdef __cpp_char8_t
			return u8"";
#else
			return (__u8_shim);
#endif
		}

		template <>
		constexpr inline decltype(auto) __empty_string<char16_t>() noexcept {
			return u"";
		}

		template <>
		constexpr inline decltype(auto) __empty_string<char32_t>() noexcept {
			return U"";
		}
	} // namespace __string_detail

	template <typename CharType, typename Traits = std::char_traits<CharType>>
	class basic_c_string_view : public std::basic_string_view<CharType, Traits> {
	private:
		using base_t = std::basic_string_view<CharType, Traits>;

		constexpr bool last_element_check() const {
			return ((this->size() > 0) ? *(this->data() + this->size()) == '\0' : true);
		}

	public:
		using const_iterator = typename base_t::const_iterator;
		using const_pointer = typename base_t::const_pointer;
		using const_reverse_iterator = typename base_t::const_reverse_iterator;
		using difference_type = typename base_t::difference_type;
		using iterator = typename base_t::iterator;
		using pointer = typename base_t::pointer;
		using reference = typename base_t::reference;
		using reverse_iterator = typename base_t::reverse_iterator;
		using size_type = typename base_t::size_type;
		using traits_type = typename base_t::traits_type;
		using value_type = typename base_t::value_type;

		constexpr basic_c_string_view()
		: basic_c_string_view(static_cast<const_pointer>(__string_detail::__empty_string<CharType>()), static_cast<size_type>(0)) {
		}

		constexpr basic_c_string_view(const_iterator __arg0_, const_iterator __arg1_)
#if defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL >= 1
		: base_t(__arg0_ == __arg1_ ? __string_detail::__empty_string<CharType>() : std::addressof(*__arg0_), std::distance(__arg0_, __arg1_)) {
#else
		: base_t(std::addressof(*__arg0_), std::distance(__arg0_, __arg1_)) {
#endif
			assert(this->last_element_check() && "non null-terminated c_string_view!");
		}

		template <typename __Arg0, typename __Arg1, typename... __Args, typename = std::enable_if_t<!(std::disjunction_v<std::is_same<meta::remove_cv_ref_t<__Arg0>, iterator>, std::is_same<meta::remove_cv_ref_t<__Arg0>, const_iterator>> && std::disjunction_v<std::is_same<meta::remove_cv_ref_t<__Arg1>, iterator>, std::is_same<meta::remove_cv_ref_t<__Arg1>, const_iterator>>)>>
		constexpr basic_c_string_view(__Arg0&& __arg0_, __Arg1&& __arg1_, __Args&&... __args_)
		: base_t(std::forward<__Arg0>(__arg0_), std::forward<__Arg1>(__arg1_), std::forward<__Args>(__args_)...) {
			assert(this->last_element_check() && "non null-terminated c_string_view!");
		}

		template <typename __Arg, typename = std::enable_if_t<!std::is_same_v<meta::remove_cv_ref_t<__Arg>, basic_c_string_view>>>
		constexpr basic_c_string_view(__Arg&& __arg_)
		: base_t(std::forward<__Arg>(__arg_)) {
			assert(this->last_element_check() && "non null-terminated c_string_view!");
		}

		constexpr basic_c_string_view(basic_c_string_view&&) = default;
		constexpr basic_c_string_view(const basic_c_string_view&) = default;
		constexpr basic_c_string_view& operator=(basic_c_string_view&&) = default;
		constexpr basic_c_string_view& operator=(const basic_c_string_view&) = default;
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
