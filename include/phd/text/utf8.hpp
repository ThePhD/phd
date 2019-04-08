#pragma once

#ifndef PHD_TEXT_UTF8_HPP
#define PHD_TEXT_UTF8_HPP

#include <phd/text/char8_t.hpp>
#include <phd/text/unicode_code_point.hpp>
#include <phd/text/detail/unicode_detail.hpp>
#include <phd/text/encoding_result.hpp>
#include <phd/text/empty_state.hpp>

#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>

#include <phd/meta/remove_cv_ref.hpp>

#include <array>

namespace phd {
	inline namespace __abi_v0 {
	namespace __detail {
		template <typename __CodeUnit = char8_t>
		struct __utf8_with {
			using state = __detail::__empty_state;
			using code_unit = __CodeUnit;
			using code_point = unicode_code_point;

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr auto encode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& error_handler) {
				using __uInputRange = meta::remove_cv_ref_t<__InputRange>;
				using __uOutputRange = meta::remove_cv_ref_t<__OutputRange>;
				using __result_t = encoding_result<__uInputRange, __uOutputRange, state>;

				static_assert(std::is_assignable_v<const code_unit&, ranges::reference_t<__InputRange>>, "the input must have a reference type which can be bound to a const code_unit&");
				static_assert(std::is_assignable_v<ranges::reference_t<__uOutputRange>, code_point>, "the output must have a reference type which can be assigned into a code_point");

				return __result_t(__input, __output, __s);
			}

			template <typename __InputRange, typename __OutputRange, typename __ErrorHandler>
			static constexpr auto decode(__InputRange&& __input, __OutputRange&& __output, state& __s, __ErrorHandler&& __error_handler) {
				using __uInputRange = typename meta::remove_cv_ref<__InputRange>::type;
				using __uOutputRange = typename meta::remove_cv_ref<__OutputRange>::type;
				using __result_t = decoding_result<__uInputRange, __uOutputRange, state>;

				auto __init = ranges::begin(__input);
				auto __inlast = ranges::end(__input);

				if (__init == __inlast) {
					// the empty sequence is an OK sequence
					return __result_t(std::forward<__InputRange>(__input), std::forward<__OutputRange>(__output), __s, encoding_errc::ok);
				}

				auto __outit = ranges::begin(__output);
				auto __outlast = ranges::end(__output);

				code_unit b0 = ranges::dereference(__init);
				std::size_t length = __unicode_detail::__sequence_length(static_cast<unsigned char>(b0));

				if (length == 1) {
					__init = ranges::next(__init);
					if (__outit == __outlast) {
						return __error_handler(__utf8_with{}, __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
					}
					else {
						ranges::dereference(__outit) = static_cast<code_point>(b0);
						return __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s);
					}
				}
				const bool __is_invalid_cu = __unicode_detail::__is_invalid(static_cast<unsigned char>(b0));
				if (__is_invalid_cu || __unicode_detail::__is_continuation(static_cast<unsigned char>(b0))) {
					return __error_handler(__utf8_with{}, __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s, __is_invalid_cu ? encoding_errc::invalid_sequence : encoding_errc::invalid_leading_sequence));
				}

				__init = ranges::next(__init);
				std::array<unsigned char, 4> b;
				b[0] = b0;
				for (std::size_t i = 1; i < length; ++i) {
					b[i] = ranges::dereference(__init);
					if (!__unicode_detail::__is_continuation(b[i])) {
						return __error_handler(__utf8_with{}, __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s, encoding_errc::invalid_trailing_sequence));
					}
					__init = ranges::next(__init);
				}

				code_point __decoded;
				switch (length) {
				case 2:
					__decoded = __unicode_detail::__decode(b[0], b[1]);
					break;
				case 3:
					__decoded = __unicode_detail::__decode(b[0], b[1], b[2]);
					break;
				default:
					__decoded = __unicode_detail::__decode(b[0], b[1], b[2], b[3]);
					break;
				}

				if (__unicode_detail::__is_overlong(__decoded, length)) {
					return __error_handler(__utf8_with{}, __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s, encoding_errc::overlong_sequence));
				}
				if (__unicode_detail::__is_surrogate(__decoded) || __decoded > __unicode_detail::__last_code_point) {
					return __error_handler(__utf8_with{}, __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s, encoding_errc::invalid_output));
				}

				if (__outit == __outlast) {
					return __error_handler(__utf8_with{}, __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s, encoding_errc::insufficient_output_space));
				}
				// then everything is fine
				ranges::dereference(__outit) = __decoded;
				__outit = ranges::next(__outit);
				return __result_t(__InputRange(__init, __inlast), __OutputRange(__outit, __outlast), __s);
			}
		};
	}
	} // namespace __abi_v0::__detail

	struct utf8 : __detail::__utf8_with<char8_t> {};

} // namespace phd

#endif // PHD_TEXT_UTF8_HPP
