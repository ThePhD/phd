#pragma once

#ifndef PHD_TEXT_ASCII_HPP
#define PHD_TEXT_ASCII_HPP

#include <phd/text/empty_state.hpp>
#include <phd/text/code_unit.hpp>
#include <phd/text/code_point.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/encode_result.hpp>
#include <phd/text/is_ignorable_error_handler.hpp>

#include <range/v3/core.hpp>
#include <range/v3/begin_end.hpp>

namespace phd {

	struct ascii {
	public:
		using state										  = __text_detail::__empty_state;
		using code_unit									  = char;
		using code_point									  = unicode_code_point;
		using is_decode_injective							  = std::true_type;
		using is_encode_injective							  = std::false_type;
		inline static constexpr const code_unit replacement_code_unit = 0x3F; // ?

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
					return __error_handler(ascii{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
				}
			}
			else {
				(void)__outlast;
			}

			code_unit __unit = ranges::dereference(__init);
			__init		  = ranges::next(__init);

			if constexpr (__call_error_handler) {
				if (static_cast<signed char>(__unit) < static_cast<signed char>(0)) {
					return __error_handler(ascii{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_output));
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
					return __error_handler(ascii{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
				}
			}
			else {
				(void)__outlast;
			}

			code_point __codepoint = ranges::dereference(__init);
			__init			   = ranges::next(__init);

			if constexpr (__call_error_handler) {
				if (__codepoint > __unicode_detail::__last_ascii_value) {
					return __error_handler(ascii{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_output));
				}
			}

			ranges::dereference(__outit) = __codepoint;
			__outit				    = ranges::next(__outit);

			return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
		}
	};
} // namespace phd

#endif // PHD_TEXT_ASCII_HPP
