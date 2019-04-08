#pragma once

#ifndef PHD_TEXT_EXECUTION_HPP
#define PHD_TEXT_EXECUTION_HPP

#include <phd/text/encoding_result.hpp>
#include <phd/text/encoding_error.hpp>
#include <phd/text/unicode_code_point.hpp>

#include <cuchar>
#include <cwchar>

namespace phd {

	struct execution {
		struct state {
			std::mbstate_t narrow_state;

			state() = default;
		};

		using code_unit = char;
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
				return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
			}

			code_unit __intermediary_output[MB_LEN_MAX]{};
			code_point codepoint = ranges::dereference(__init);
			std::size_t __res = std::c32rtomb(__intermediary_output, codepoint, std::addressof(__s.narrow_state));
			if (__res == static_cast<std::size_t>(-1)) {
				return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
			}
			for (auto __intermediary_it = __intermediary_output; __res-- > 0;) {
				ranges::dereference(__outit) = ranges::dereference(__intermediary_it);
				__outit = ranges::next(__outit);
			}
			return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
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
				return __error_handler(execution{}, __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::insufficient_output_space));
			}

			static constexpr const std::size_t __state_count_max = 12;
			code_unit __intermediary_input[__state_count_max] = { ranges::dereference(__init), {} };
			std::size_t __state_count = 1;
			std::size_t __state_offset = 0;
			for (; __state_count < __state_count_max; ++__state_count) {
				char32_t __intermediary_output;
				std::size_t __res = std::mbrtoc32(std::addressof(__intermediary_output), std::addressof(__intermediary_input[__state_offset]), __state_count, std::addressof(__s.narrow_state));
				switch (__res) {
				case static_cast<std::size_t>(-2):
					// cycle around and continue
					__init = ranges::next(__init);
					__intermediary_input[__state_count] = ranges::dereference(__init);
					break;
				case static_cast<std::size_t>(-3):
					ranges::dereference(__outit) = __intermediary_output;
					__state_offset = __state_count;
					__outit = ranges::next(__outit);
					return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
				case static_cast<std::size_t>(-1):
					// OH GOD PANIC AAAAAAH
					// seriously we're out of spec here:
					// everything has gone to shit
					// even the narrow_state is unspecified ;;
					return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
				case static_cast<std::size_t>(0):
					__init = ranges::next(__init);
					return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
				default:
					// 0 means null character; ok
					ranges::dereference(__outit) = __intermediary_output;
					__outit = ranges::next(__outit);
					__init = ranges::next(__init);
					return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
				}
			}
			// if it was invalid, we would have caught it before
			// this is for incomplete sequences only
			return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
		}
	};

} // namespace phd

#endif // PHD_TEXT_EXECUTION_HPP
