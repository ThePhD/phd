#pragma once

#ifndef PHD_TEXT_UTF16_HPP
#define PHD_TEXT_UTF16_HPP

#include <phd/text/text_forward.hpp>
#include <phd/text/empty_state.hpp>
#include <phd/text/code_unit.hpp>
#include <phd/text/code_point.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/is_ignorable_error_handler.hpp>

#include <range/v3/core.hpp>
#include <range/v3/begin_end.hpp>

namespace phd {

	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename __Derived = void, typename __CodeUnit = char16_t, bool surrogates_allowed = false>
		struct __utf16_with {
		private:
			using __self_t = typename std::conditional<std::is_void_v<__Derived>, __utf16_with, __Derived>::type;

		public:
			using state = __text_detail::__empty_state;
			using code_unit = __CodeUnit;
			using code_point = unicode_code_point;

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr auto decode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
				using __uInputRange = typename meta::template remove_cv_ref<__InputRange>::type;
				using __uOutputRange = typename meta::template remove_cv_ref<__OutputRange>::type;
				using __uErrorHandler = typename meta::template remove_cv_ref<__ErrorHandler>::type;
				using __result_t = encoding_result<__uInputRange, __uOutputRange, state>;
				constexpr bool __call_error_handler = !is_ignorable_error_handler_v<__uErrorHandler>;

				auto __init = ranges::cbegin(__input);
				auto __inlast = ranges::cend(__input);
				if constexpr (__call_error_handler) {
					if (__init == __inlast) {
						// an exhausted sequence is fine
						return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
					}
				}
				else {
					(void)__inlast;
				}

				auto __outit = ranges::begin(__output);
				auto __outlast = ranges::end(__output);

				if constexpr (__call_error_handler) {
					if (__outit == __outlast) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
					}
				}
				else {
					(void)__outlast;
				}

				char16_t __lead = static_cast<char16_t>(ranges::dereference(__init));
				__init = ranges::next(__init);

				if (!__unicode_detail::__is_surrogate(__lead)) {
					ranges::dereference(__outit) = static_cast<code_point>(__lead);
					__outit = ranges::next(__outit);
					return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
				}
				if constexpr (__call_error_handler) {
					if (!__unicode_detail::__is_lead_surrogate(__lead)) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_leading_sequence));
					}
				}
				if constexpr (__call_error_handler) {
					if (__init == __inlast) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
					}
				}

				auto __trail = ranges::dereference(__init);
				__init = ranges::next(__init);
				if constexpr (__call_error_handler) {
					if (!__unicode_detail::__is_trail_surrogate(__trail)) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_trailing_sequence));
					}
				}
				ranges::dereference(__outit) = static_cast<code_point>(__unicode_detail::__combine_surrogates(static_cast<char16_t>(__lead), static_cast<char16_t>(__trail)));
				__outit = ranges::next(__outit);

				return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
			}

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr auto encode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
				using __uInputRange = typename meta::template remove_cv_ref<__InputRange>::type;
				using __uOutputRange = typename meta::template remove_cv_ref<__OutputRange>::type;
				using __uErrorHandler = typename meta::template remove_cv_ref<__ErrorHandler>::type;
				using __result_t = encoding_result<__uInputRange, __uOutputRange, state>;
				constexpr bool __call_error_handler = !is_ignorable_error_handler_v<__uErrorHandler>;

				auto __init = ranges::cbegin(__input);
				auto __inlast = ranges::cend(__input);
				if (__init == __inlast) {
					// an exhausted sequence is fine
					return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
				}

				auto __outit = ranges::begin(__output);
				auto __outlast = ranges::end(__output);

				if constexpr (__call_error_handler) {
					if (__outit == __outlast) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
					}
				}
				else {
					(void)__outlast;
				}

				code_point __codepoint = ranges::dereference(__init);
				__init = ranges::next(__init);

				if constexpr (__call_error_handler) {
					if (__codepoint > __unicode_detail::__last_code_point) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_output));
					}
				}

				if (__codepoint <= __unicode_detail::__last_bmp_value) {
					ranges::dereference(__outit) = static_cast<char16_t>(__codepoint);
					__outit = ranges::next(__outit);
				}
				else {
					auto normal = __codepoint - __unicode_detail::__normalizing_value;
					auto lead = __unicode_detail::__first_lead_surrogate + ((normal & __unicode_detail::__lead_surrogate_bitmask) >> __unicode_detail::__lead_shifted_bits);
					auto trail = __unicode_detail::__first_trail_surrogate + (normal & __unicode_detail::__trail_surrogate_bitmask);

					ranges::dereference(__outit) = static_cast<char16_t>(lead);
					__outit = ranges::next(__outit);

					if constexpr (__call_error_handler) {
						if (__outit == __outlast) {
							return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
						}
					}
					ranges::dereference(__outit) = static_cast<char16_t>(trail);
					__outit = ranges::next(__outit);
				}

				return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
			}
		};
	}
	} // namespace __abi_v0::__text_detail

	struct utf16 : __text_detail::__utf16_with<utf16, char16_t> {};

} // namespace phd

#endif // PHD_TEXT_UTF16_HPP
