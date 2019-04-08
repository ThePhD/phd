#pragma once

#ifndef PHD_TEXT_ENCODING_ERROR_HPP
#define PHD_TEXT_ENCODING_ERROR_HPP

#include <phd/text/char8_t.hpp>
#include <phd/text/unicode_code_point.hpp>
#include <phd/text/encoding_error.hpp>

#include <cstddef>
#include <system_error>
#include <type_traits>
#include <string>
#include <exception>

namespace phd {
inline namespace __abi_v0 {

	enum class encoding_errc : int {
		// just fine
		ok = 0x00,
		// input contains ill-formed sequences
		invalid_sequence = 0x01,
		// input contains incomplete sequences
		incomplete_sequence = 0x02,
		// input contains overlong encoding sequence (e.g. for utf8)
		overlong_sequence = 0x03,
		// output cannot receive all the completed code units
		insufficient_output_space = 0x04,
		// sequence can be encoded but resuulting code point is invalid (e.g., encodes a lone surrogate)
		invalid_output = 0x05,
		// leading code unit is wrong
		invalid_leading_sequence = 0x06,
		// leading code units were correct, trailing code units were wrong
		invalid_trailing_sequence = 0x07
	};

	class encoding_error_category : public std::error_category {
	public:
#ifdef _MSC_VER
		encoding_error_category() noexcept
		: std::error_category() {
		}
#else
		constexpr encoding_error_category() noexcept
		: std::error_category() {
		}
#endif

		virtual const char* name() const noexcept override {
			return "encoding_error_category (__abi_v0)";
		}

		virtual std::string message(int condition) const override {
			using namespace std::string_literals;

			switch (static_cast<encoding_errc>(condition)) {
			case encoding_errc::ok:
				return "ok"s;
			case encoding_errc::invalid_sequence:
				return "input contains invalid sequence"s;
			case encoding_errc::incomplete_sequence:
				return "input contains incomplete sequence of code points"s;
			case encoding_errc::overlong_sequence:
				return "input contains overlong sequence for single code point"s;
			case encoding_errc::insufficient_output_space:
				return "output does not contain enough space for the operation"s;
			case encoding_errc::invalid_output:
				return "sequence can be encoded but it produces an invalid output"s;
			case encoding_errc::invalid_leading_sequence:
				return "sequence starts with an invalid code unit"s;
			case encoding_errc::invalid_trailing_sequence:
				return "sequence follows valid leading code units with illegal code units"s;
			default:
				//std::unreachable();
				std::terminate();
			}
		}
	};

#ifdef _MSC_VER
#else
	namespace __detail {
		constexpr const encoding_error_category __encoding_error_category = encoding_error_category{};
	}
#endif

#ifdef _MSC_VER
	inline const std::error_category& encoding_category() {
		static const encoding_error_category __val = encoding_error_category{};
		return __val;
	}
#else
	inline constexpr const std::error_category& encoding_category() {
		return __detail::__encoding_error_category;
	}
#endif

	inline std::error_condition make_error_condition(encoding_errc e) noexcept {
		return std::error_condition(static_cast<int>(e), encoding_error_category());
	}
}
} // namespace phd::__abi_v0

namespace std {
	template <>
	struct is_error_condition_enum<::phd::encoding_errc> : std::true_type {};
} // namespace std

#endif // PHD_TEXT_ENCODING_ERROR_HPP
