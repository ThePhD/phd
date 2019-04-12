#pragma once

#ifndef PHD_TEXT_EXECUTION_HPP
#define PHD_TEXT_EXECUTION_HPP

#include <phd/text/encode_result.hpp>
#include <phd/text/encoding_error.hpp>
#include <phd/text/unicode_code_point.hpp>
#include <phd/text/utf16.hpp>
#include <phd/text/detail/windows.hpp>

#include <cuchar>
#include <cwchar>

#include <range/v3/core.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/span.hpp>

#include <phd/meta/remove_cv_ref.hpp>

namespace phd {

	struct execution {
		struct __state {
			std::mbstate_t narrow_state;

			__state() = default;
		};

		using state			 = __state;
		using code_unit		 = char;
		using code_point		 = unicode_code_point;
		using is_decode_injective = std::false_type;
		using is_encode_injective = std::false_type;

		static inline constexpr code_unit replacement_code_unit = '?';

		template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
		static constexpr auto encode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
			using __uInputRange  = typename meta::template remove_cv_ref<__InputRange>::type;
			using __uOutputRange = typename meta::template remove_cv_ref<__OutputRange>::type;
			using __result_t	= encode_result<__uInputRange, __uOutputRange, state>;
#ifdef _WIN32
			using __u16e			  = __text_detail::__utf16_with<void, wchar_t, false>;
			using __intermediate_state = typename __u16e::state;
#endif

			auto __init   = ranges::cbegin(__input);
			auto __inlast = ranges::cend(__input);

			if (__init == __inlast) {
				// an exhausted sequence is fine
				return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
			}

			auto __outit   = ranges::begin(__output);
			auto __outlast = ranges::end(__output);

			code_point codepoint = ranges::dereference(__init);
			__init			 = ranges::next(__init);
#ifdef WIN32
			__u16e __u16enc{};
			__intermediate_state __intermediate_s{};
			wchar_t __wide_intermediary[8];
			auto __result = __u16enc.encode(std::forward<__InputRange>(__input), ranges::span(__wide_intermediary, static_cast<std::size_t>(8)), __intermediate_s, __text_detail::__pass_through_text_error_handler{});
			if (__result.error_code != encoding_errc::ok) {
				return __error_handler(execution{}, __result_t(std::move(__result.input), std::forward<__OutputRange>(__output), __s, __result.error_code));
			}
			constexpr const std::size_t __state_count_max = 12;
			code_unit __intermediary_output[__state_count_max]{};
			phd::windows::BOOL __used_default_char = false;
			auto __wide_read_buffer			    = ranges::span(__wide_intermediary, __result.output.data());
			int __res						    = phd::windows::WideCharToMultiByte(phd::windows::determine_code_page(), 0, __wide_read_buffer.data(), static_cast<int>(__wide_read_buffer.size()), __intermediary_output, __state_count_max, &replacement_code_unit, std::addressof(__used_default_char));
			if (__res == 0) {
				return __error_handler(execution{}, __result_t(std::move(__result.input), std::forward<__OutputRange>(__output), __s, phd::windows::GetLastError() == ERROR_INSUFFICIENT_BUFFER ? encoding_errc::insufficient_output_space : encoding_errc::invalid_sequence));
			}
			for (auto __intermediary_it = __intermediary_output; __res-- > 0;) {
				if (__outit == __outlast) {
					return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
				}
				ranges::dereference(__outit) = ranges::dereference(__intermediary_it);
				__outit				    = ranges::next(__outit);
			}
			return __result_t(std::move(__result.input), __uOutputRange(__outit, __outlast), __s, __result.error_code);
#else
			code_unit __intermediary_output[MB_LEN_MAX]{};
			std::size_t __res = std::c32rtomb(__intermediary_output, codepoint, std::addressof(__s.narrow_state));
			if (__res == static_cast<std::size_t>(-1)) {
				return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
			}
			for (auto __intermediary_it = __intermediary_output; __res-- > 0;) {
				if (__outit == __outlast) {
					return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
				}
				ranges::dereference(__outit) = ranges::dereference(__intermediary_it);
				__outit				    = ranges::next(__outit);
			}
			return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
#endif // Windows is hell
		}

		template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
		static constexpr auto decode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
			using __uInputRange  = typename meta::template remove_cv_ref<__InputRange>::type;
			using __uOutputRange = typename meta::template remove_cv_ref<__OutputRange>::type;
			using __result_t	= decode_result<__uInputRange, __uOutputRange, state>;
#ifdef _WIN32
			using __u16e			  = __text_detail::__utf16_with<void, wchar_t, false>;
			using __intermediate_state = typename __u16e::state;
#endif

			auto __init   = ranges::cbegin(__input);
			auto __inlast = ranges::cend(__input);

			if (__init == __inlast) {
				// an exhausted sequence is fine
				return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
			}

			auto __outit   = ranges::begin(__output);
			auto __outlast = ranges::end(__output);

			if (__outit == __outlast) {
				return __error_handler(execution{}, __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::insufficient_output_space));
			}

			constexpr const std::size_t __state_count_max	= 12;
			code_unit __intermediary_input[__state_count_max] = { ranges::dereference(__init), {} };
			__init									= ranges::next(__init);
			std::size_t __state_count					= 1;
			std::size_t __state_offset					= 0;
			for (; __state_count < __state_count_max; ++__state_count) {
				char32_t __intermediary_output{};
#ifdef _WIN32
				constexpr const int __wide_intermediary_size = 4;
				wchar_t __wide_intermediary[__wide_intermediary_size]{};
				int __res = phd::windows::MultiByteToWideChar(phd::windows::determine_code_page(), 0,
					__intermediary_input, static_cast<int>(__state_count),
					__wide_intermediary, __wide_intermediary_size);
				if (__res == 0) {
					if (phd::windows::GetLastError() == ERROR_NO_UNICODE_TRANSLATION) {
						// loopback; we might just not have enough code units
						if (__init == __inlast) {
							return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
						}
						__intermediary_input[__state_count] = ranges::dereference(__init);
						__init						 = ranges::next(__init);
						continue;
					}
					return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::invalid_sequence));
				}

				__u16e __u16enc{};
				__intermediate_state __intermediate_s{};
				auto __result = __u16enc.decode(wc_string_view(__wide_intermediary, static_cast<std::size_t>(__res)), std::forward<__OutputRange>(__output), __intermediate_s, __text_detail::__pass_through_text_error_handler{});
				if (__result.error_code != encoding_errc::ok) {
					return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), std::move(__result.output), __s, __result.error_code));
				}
				return __result_t(__uInputRange(__init, __inlast), std::move(__result.output), __s, __result.error_code);
#else
				std::size_t __res = std::mbrtoc32(std::addressof(__intermediary_output), std::addressof(__intermediary_input[__state_offset]), __state_count, std::addressof(__s.narrow_state));

				switch (__res) {
				case static_cast<std::size_t>(-2):
					// cycle around and continue
					__init = ranges::next(__init);
					if (__init == __inlast) {
						return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
					}
					__intermediary_input[__state_count] = ranges::dereference(__init);
					break;
				case static_cast<std::size_t>(-3):
					ranges::dereference(__outit) = __intermediary_output;
					__state_offset			    = __state_count;
					__outit				    = ranges::next(__outit);
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
					__outit				    = ranges::next(__outit);
					__init				    = ranges::next(__init);
					return __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::ok);
				}
#endif
			}
			// if it was invalid, we would have caught it before
			// this is for incomplete sequences only
			return __error_handler(execution{}, __result_t(__uInputRange(__init, __inlast), __uOutputRange(__outit, __outlast), __s, encoding_errc::incomplete_sequence));
		}
	};

} // namespace phd

#endif // PHD_TEXT_EXECUTION_HPP
