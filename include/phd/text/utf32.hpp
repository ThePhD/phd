#pragma once

#ifndef PHD_TEXT_UTF32_HPP
#define PHD_TEXT_UTF32_HPP

#include <phd/text/text_forward.hpp>
#include <phd/text/empty_state.hpp>
#include <phd/text/code_unit.hpp>
#include <phd/text/code_point.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/encode_result.hpp>
#include <phd/text/is_ignorable_error_handler.hpp>

#include <range/v3/core.hpp>
#include <range/v3/begin_end.hpp>

namespace phd {
	inline namespace __abi_v0 {
	namespace __text_detail {
		template <typename __Derived = void, typename __CodeUnit = char32_t, bool __validate = true>
		struct __utf32_with {
		private:
			using __self_t = typename std::conditional<std::is_void_v<__Derived>, __utf32_with, __Derived>::type;

		public:
			using state			 = __text_detail::__empty_state;
			using code_unit		 = __CodeUnit;
			using code_point		 = unicode_code_point;
			using is_decode_injective = std::true_type;
			using is_encode_injective = std::true_type;

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr auto decode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
				using __uInputRange				 = typename meta::template remove_cv_ref<__InputRange>::type;
				using __uOutputRange			 = typename meta::template remove_cv_ref<__OutputRange>::type;
				using __uErrorHandler			 = typename meta::template remove_cv_ref<__ErrorHandler>::type;
				using __result_t				 = decode_result<__uInputRange, __uOutputRange, state>;
				constexpr bool __call_error_handler = !is_ignorable_error_handler_v<__uErrorHandler>;

				auto __init   = ranges::cbegin(__input);
				auto __inlast = ranges::cend(__input);
				if (__init == __inlast) {
					// an exhausted sequence is fine
					return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
				}

				auto __outit   = ranges::begin(__output);
				auto __outlast = ranges::end(__output);

				if constexpr (__call_error_handler) {
					if (__outit == __outlast) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
					}
				}
				else {
					(void)__outlast;
				}

				code_unit __unit = ranges::dereference(__init);
				__init		  = ranges::next(__init);

				if constexpr (__validate && __call_error_handler) {
					if (__unit > __unicode_detail::__last_code_point || __unicode_detail::__is_surrogate(__unit)) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_output));
					}
				}

				ranges::dereference(__outit) = __unit;
				__outit				    = ranges::next(__outit);

				return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
			}

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr auto encode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
				using __uInputRange				 = typename meta::template remove_cv_ref<__InputRange>::type;
				using __uOutputRange			 = typename meta::template remove_cv_ref<__OutputRange>::type;
				using __uErrorHandler			 = typename meta::template remove_cv_ref<__ErrorHandler>::type;
				using __result_t				 = encode_result<__uInputRange, __uOutputRange, state>;
				constexpr bool __call_error_handler = !is_ignorable_error_handler_v<__uErrorHandler>;

				auto __init   = ranges::cbegin(__input);
				auto __inlast = ranges::cend(__input);
				if (__init == __inlast) {
					// an exhausted sequence is fine
					return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
				}

				auto __outit   = ranges::begin(__output);
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
				__init			   = ranges::next(__init);

				if constexpr (__validate && __call_error_handler) {
					if (__codepoint > __unicode_detail::__last_code_point || __unicode_detail::__is_surrogate(__codepoint)) {
						return __error_handler(__self_t{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_output));
					}
				}

				ranges::dereference(__outit) = __codepoint;
				__outit				    = ranges::next(__outit);

				return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
			}
		};
	}
	} // namespace __abi_v0::__text_detail

	struct utf32 : __text_detail::__utf32_with<utf32, char32_t> {};

} // namespace phd


#endif // PHD_TEXT_UTF32_HPP
