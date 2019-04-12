#pragma once

#ifndef PHD_TEXT_WIDE_EXECUTION_HPP
#define PHD_TEXT_WIDE_EXECUTION_HPP

#include <phd/text/unicode_code_point.hpp>
#include <phd/text/execution.hpp>
#include <phd/text/encode_result.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/c_string_view.hpp>
#include <phd/text/empty_state.hpp>
#include <phd/text/detail/windows.hpp>

#include <range/v3/core.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/span.hpp>

#include <phd/meta/remove_cv_ref.hpp>

#include <cuchar>
#include <cwchar>
#ifdef _MSC_VER
#include <wchar.h>
#endif
#include <utility>
#include <iterator>

namespace phd {

	struct wide_execution {
#ifdef _WIN32
		using __state = __text_detail::__empty_state;
#else
		struct __state {
			std::mbstate_t wide_state;
			execution::state narrow_state;

			__state() = default;
		};
#endif // Windows

		// UTF32 on Linux; is fully injective
		// UTF16 on Windows; is fully injective
		using state			 = __state;
		using code_unit		 = wchar_t;
		using code_point		 = unicode_code_point;
		using is_decode_injective = std::true_type;
		using is_encode_injective = std::true_type;

		template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
		static constexpr auto encode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
			using __uInputRange				 = typename meta::template remove_cv_ref<__InputRange>::type;
			using __uOutputRange			 = typename meta::template remove_cv_ref<__OutputRange>::type;
			using __uErrorHandler			 = typename meta::remove_cv_ref<__ErrorHandler>::type;
			using __result_t				 = encode_result<__uInputRange, __uOutputRange, state>;
			constexpr bool __call_error_handler = !is_ignorable_error_handler_v<__uErrorHandler>;
#ifdef _WIN32
			using __u16e			  = __text_detail::__utf16_with<void, wchar_t, false>;
			using __intermediate_state = typename __u16e::state;

			__u16e __u16enc{};
			__intermediate_state __intermediate_s{};
			auto __result = __u16enc.encode(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __intermediate_s, __text_detail::__pass_through_text_error_handler_with<!__call_error_handler>{});
			if (__result.error_code != encoding_errc::ok) {
				return __error_handler(wide_execution{}, __result_t(std::move(__result.input), std::move(__result.output), __s, __result.error_code));
			}
			return __result_t(std::move(__result.input), std::move(__result.output), __s, __result.error_code);
#else
			auto __init   = ranges::cbegin(__input);
			auto __inlast = ranges::cend(__input);

			if (__init == __inlast) {
				// an exhausted sequence is fine
				return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
			}

			auto __outit   = ranges::begin(__output);
			auto __outlast = ranges::end(__output);

			constexpr const std::size_t __state_max = 32;
			char __pray_for_state[__state_max + 1];
			char* __pray_start = &__pray_for_state[0];

			execution __exec{};
			auto __result = __exec.encode(std::forward<__InputRange>(__input), ranges::span(__pray_start, __state_max), __s.narrow_state, __text_detail::__pass_through_text_error_handler_with<!__call_error_handler>{});
			if constexpr (__call_error_handler) {
				if (__result.error_code != encoding_errc::ok) {
					return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, __result.error_code));
				}
			}

			auto __current_input = ranges::span(__pray_start, std::distance(__pray_start, __result.output.data()));
			code_unit __unit;
			std::size_t __res = std::mbrtowc(&__unit, __current_input.data(), __current_input.size(), std::addressof(__s.wide_state));
			if (__res == static_cast<std::size_t>(-1)) {
				// error: cry about it
				if constexpr (__call_error_handler) {
					return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
				}
			}
			else if (static_cast<std::size_t>(-2)) {
				// incomplete sequence
				if constexpr (__call_error_handler) {
					return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
				}
			}
			ranges::dereference(__outit) = __unit;
			__outit				    = ranges::next(__outit);

			return __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, __result.error_code);
#endif // Windows shit
		}

		template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
		static constexpr auto decode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
			using __uInputRange				 = typename meta::template remove_cv_ref<__InputRange>::type;
			using __uOutputRange			 = typename meta::template remove_cv_ref<__OutputRange>::type;
			using __uErrorHandler			 = typename meta::remove_cv_ref<__ErrorHandler>::type;
			using __result_t				 = decode_result<__uInputRange, __uOutputRange, state>;
			constexpr bool __call_error_handler = !is_ignorable_error_handler_v<__uErrorHandler>;
#ifdef _WIN32
			using __u16e			  = __text_detail::__utf16_with<void, wchar_t, false>;
			using __intermediate_state = typename __u16e::state;

			__u16e __u16enc{};
			__intermediate_state __intermediate_s{};
			auto __result = __u16enc.decode(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __intermediate_s, __text_detail::__pass_through_text_error_handler_with<!__call_error_handler>{});
			if constexpr (__call_error_handler) {
				if (__result.error_code != encoding_errc::ok) {
					return __error_handler(wide_execution{}, __result_t(std::move(__result.input), std::move(__result.output), __s, __result.error_code));
				}
			}
			return __result_t(std::move(__result.input), std::move(__result.output), __s, __result.error_code, __result.handled_error);
#else

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
					return __error_handler(wide_execution{}, __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::insufficient_output_space));
				}
			}

			constexpr const std::size_t __state_max = 32;
			char __pray_for_state[__state_max + 1]{};
			std::size_t __state_count = 0;
			for (; __state_count < __state_max;) {
				code_unit __unit = ranges::dereference(__init);
				__init		  = ranges::next(__init);
#ifdef _MSC_VER
				std::size_t __res;
				errno_t __err = wcrtomb_s(std::addressof(__res), __pray_for_state, __state_max, __unit, std::addressof(__s.wide_state));
				if constexpr (__call_error_handler) {
					if (__err != 0) {
						// error: cry about it
						return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
					}
				}
				else {
					(void)__err;
				}
#else
				std::size_t __res = std::wcrtomb(__pray_for_state, __unit, std::addressof(__s.wide_state));
#endif

				if (__res == static_cast<std::size_t>(-1)) {
					if constexpr (__call_error_handler) {
						// error: cry about it
						return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
					}
				}
				else if (__res == 0 && std::mbsinit(std::addressof(__s.wide_state)) == 0) {
					// mixed conversion potential?!
					// technically, not standard behavior, but I don't really care?
					// Mr. Steve Downey points out I'm slightly right
					// about my assumption here: C has an open DR for this
					// (DR 488, http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2059.htm#dr_488)
					// Another DR, DR 499 (http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2059.htm#dr_498)
					// addresses thread safety issues, both should be
					// solved is this is to be anywhere near usable
					if constexpr (__call_error_handler) {
						if (__init == __inlast) {
							return __error_handler(wide_execution{}, __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::incomplete_sequence));
						}
					}
					continue;
				}

				__state_count += __res;
				break;
			}

			execution __exec{};
			auto __result = __exec.decode(c_string_view(__pray_for_state, __state_count), std::forward<__OutputRange>(__output), __s.narrow_state, __text_detail::__pass_through_text_error_handler_with<!__call_error_handler>{});
			if constexpr (__call_error_handler) {
				if (__result.error_code != encoding_errc::ok) {
					return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), std::move(__result.output), __s, __result.error_code));
				}
			}
			return __result_t(__uInputRange(__init, __inlast), std::move(__result.output), __s, __result.error_code);
#endif
		}
	}; // namespace phd

} // namespace phd

#endif // PHD_TEXT_WIDE_EXECUTION_HPP
