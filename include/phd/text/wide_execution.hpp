#pragma once

#ifndef PHD_TEXT_WIDE_EXECUTION_HPP
#define PHD_TEXT_WIDE_EXECUTION_HPP

#include <phd/text/unicode_code_point.hpp>
#include <phd/text/execution.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/c_string_view.hpp>

#include <cuchar>
#include <cwchar>
#ifdef _MSC_VER
#include <wchar.h>
#endif
#include <utility>

namespace phd {

	struct wide_execution {
		struct state {
			std::mbstate_t wide_state;
			execution::state narrow_state;

			state() = default;
		};

		using code_unit = wchar_t;
		using code_point = unicode_code_point;

		template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
		static constexpr auto encode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
			using __uInputRange = typename meta::template remove_cv_ref<__InputRange>::type;
			using __uOutputRange = typename meta::template remove_cv_ref<__OutputRange>::type;
			using __result_t = encoding_result<__uInputRange, __uOutputRange, state>;

			auto __init = ranges::cbegin(__input);
			auto __inlast = ranges::cend(__input);

			if (__init == __inlast) {
				// an exhausted sequence is fine
				return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
			}

			auto __outit = ranges::cbegin(__output);
			auto __outlast = ranges::cend(__output);

			if (__outit == __outlast) {
				return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
			}

			static constexpr const std::size_t __state_max = 32;
			char __pray_for_state[__state_max + 1];
			char* __pray_start = &__pray_for_state[0];

			execution __exec{};
			auto __result = __exec.encode(std::forward<__InputRange>(__input), ranges::span(__pray_start, __state_max), __s.narrow_state, __text_detail::__pass_through_text_error_handler{});
			if (__result.error_code != encoding_errc::ok) {
				return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, __result.error_code));
			}

			auto __current_input = ranges::span(__pray_start, std::distance(__pray_start, __result.output.data()));
			code_unit __unit;
			const code_point codepoint = ranges::dereference(__init);
			std::size_t __res = std::mbrtowc(&__unit, __current_input.data(), __current_input.size(), std::addressof(__s.wide_state));
			if (__res == static_cast<std::size_t>(-1)) {
				// error: cry about it
				return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
			}
			else if (static_cast<std::size_t>(-2)) {
				// incomplete sequence
				return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
			}

			for (auto __prayit = __pray_for_state; __res-- > 0;) {
				ranges::dereference(__outit) = ranges::dereference(__prayit);
				__outit = ranges::next(__outit);
				if (__outit == __outlast) {
					return __error_handler(wide_execution{}, __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
				}
			}

			return __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, __result.error_code);
		}

		template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
		static constexpr auto decode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
			using __uInputRange = typename meta::template remove_cv_ref<__InputRange>::type;
			using __uOutputRange = typename meta::template remove_cv_ref<__OutputRange>::type;
			using __result_t = encoding_result<__uInputRange, __uOutputRange, state>;

			auto __init = ranges::cbegin(__input);
			auto __inlast = ranges::cend(__input);

			if (__init == __inlast) {
				// an exhausted sequence is fine
				return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
			}

			auto __outit = ranges::cbegin(__output);
			auto __outlast = ranges::cend(__output);

			if (__outit == __outlast) {
				return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
			}

			/*NOT A CONSTANT: MB_CUR_MAX * 4*/
			static constexpr const std::size_t __state_max = 32;
			char __pray_for_state[__state_max + 1]{};
			std::size_t __state_count = 0;
			for (; __init != __inlast && __state_count < __state_max;) {
				code_unit __unit = ranges::dereference(__init);
#ifdef _MSC_VER
				std::size_t __res;
				errno_t __err = wcrtomb_s(std::addressof(__res), __pray_for_state, __state_max, __unit, std::addressof(__s.wide_state));
				if (__err != 0) {
					// error: cry about it
					return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
				}
#else
				std::size_t __res = std::wcrtomb(__pray_for_state, __unit, std::addressof(__s.wide_state));
#endif
				if (__res == static_cast<std::size_t>(-1)) {
					// error: cry about it
					return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
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
					__init = ranges::next(__init);
					continue;
				}
				__state_count += __res;
				for (auto __prayit = __pray_for_state; __res-- > 0;) {
					if (__outit == __outlast) {
						return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
					}
					ranges::dereference(__outit) = ranges::dereference(__prayit);
					__outit = ranges::next(__outit);
				}
				break;
			}

			execution __exec{};
			auto __result = __exec.decode(c_string_view(__pray_for_state, __state_count), std::forward<__OutputRange>(__output), __s.narrow_state, __text_detail::__pass_through_text_error_handler{});
			if (__result.error_code != encoding_errc::ok) {
				return __error_handler(wide_execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, __result.error_code));
			}
			return __result_t(__uInputRange(__init, __inlast), std::move(__result.output), __s, __result.error_code);
		}
	};

} // namespace phd

#endif // PHD_TEXT_WIDE_EXECUTION_HPP
