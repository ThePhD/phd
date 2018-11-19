#pragma once

#ifndef PHD_TEXT_BASIC_STRING_LITERAL_HPP
#define PHD_TEXT_BASIC_STRING_LITERAL_HPP

#include <phd/text/char8_t.hpp>
#include <cstddef>

#ifndef __cpp_impl_string_literal

namespace phd {

	namespace detail {
		template <class InputIt, class OutputIt>
		constexpr OutputIt constexpr_copy(InputIt first, InputIt last, OutputIt out_first) {
			while (first != last) {
				*out_first++ = *first++;
			}
			return out_first;
		}
	} // namespace detail

	template <class CharType, std::size_t N>
	class basic_string_literal {
	private:
		using storage_type = CharType[N];

	public:
		using value_type = CharType;
		using reference = const CharType&;
		using const_reference = const CharType&;
		using size_type = size_t;

		using iterator = const CharType*;
		using const_iterator = const CharType*;

		constexpr basic_string_literal(const storage_type& a)
		: arr{} {
			detail::constexpr_copy(a + 0, a + N, arr + 0);
		}

		constexpr basic_string_literal() noexcept {
		}

		constexpr operator const storage_type&() const {
			return arr;
		}

		constexpr const CharType* data() const noexcept {
			return arr;
		}
		constexpr const CharType* c_str() const noexcept {
			return arr;
		}

		constexpr size_type size() const noexcept {
			return N;
		}

		constexpr iterator begin() const noexcept {
			return arr;
		}
		constexpr iterator end() const noexcept {
			return begin() + size();
		}
		constexpr const_iterator cbegin() const noexcept {
			return arr;
		}
		constexpr const_iterator cend() const noexcept {
			return cbegin() + size();
		}

	private:
		storage_type arr; // exposition-only
	};

	template <class CharType, size_t N>
	constexpr const CharType* begin(const basic_string_literal<CharType, N>& lit) noexcept {
		return lit.begin();
	}
	template <class CharType, size_t N>
	constexpr const CharType* end(const basic_string_literal<CharType, N>& lit) noexcept {
		return lit.end();
	}
	template <class CharType, size_t N>
	constexpr const CharType* cbegin(const basic_string_literal<CharType, N>& lit) noexcept {
		return lit.cbegin();
	}
	template <class CharType, size_t N>
	constexpr const CharType* cend(const basic_string_literal<CharType, N>& lit) noexcept {
		return lit.cend();
	}

} // namespace phd

#endif // __cpp_impl_string_literal

#endif // PHD_TEXT_BASIC_STRING_LITERAL_HPP
