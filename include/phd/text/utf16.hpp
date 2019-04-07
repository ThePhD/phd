#pragma once

#ifndef PHD_TEXT_UTF16_HPP
#define PHD_TEXT_UTF16_HPP

#include <phd/text/text_forward.hpp>

namespace phd {

	inline namespace __abi_v0 {
	namespace __detail {
		template <typename __CodeUnit = char16_t>
		struct __utf16_with {
			using code_unit = __CodeUnit;
			using decoded_code_unit = unicode_code_point;

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr encoding_result<__InputRange, __OutputRange> encode(__InputRange&& input, __OutputRange&& output, __ErrorHandler&& error_handler) {
				static_assert(std::is_assignable_v<const code_unit&, ranges::reference_t<__InputRange>>, "the input must have a reference type which can be bound to a const code_unit&");
				static_assert(std::is_assignable_v<ranges::reference_t<__OutputRange>, decoded_code_unit>, "the input must have a reference type which can be bound to a const code_unit&");
				return encoding_result<std::remove_cvref_t<__InputRange>, std::remove_cvref_t<__OutputRange>>(input, output);
			}

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr encoding_result<__InputRange, __OutputRange> decode(__InputRange&& __input, __OutputRange&& __output, __ErrorHandler&& __error_handler) {
				return encoding_result<std::remove_cvref_t<__InputRange>, std::remove_cvref_t<__OutputRange>>(__input, __output);
			}
		};
	}
	} // namespace __abi_v0::__detail

	struct utf16 : __detail::__utf16_with<char16_t> {};

} // namespace phd

#endif // PHD_TEXT_UTF16_HPP
