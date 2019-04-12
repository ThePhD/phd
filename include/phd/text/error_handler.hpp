#pragma once

#ifndef PHD_TEXT_ERROR_HANDLER_HPP
#define PHD_TEXT_ERROR_HANDLER_HPP

#include <phd/text/encoding_error.hpp>
#include <phd/text/encode_result.hpp>
#include <phd/text/is_unicode_code_point.hpp>
#include <phd/text/is_code_point_replaceable.hpp>
#include <phd/text/is_code_unit_replaceable.hpp>
#include <phd/text/code_point.hpp>
#include <phd/text/c_string_view.hpp>
#include <phd/text/detail/unicode_detail.hpp>

#include <range/v3/begin_end.hpp>
#include <range/v3/range_traits.hpp>

#include <cstddef>
#include <system_error>
#include <type_traits>
#include <string_view>
#include <exception>
#include <climits>

namespace phd {
inline namespace __abi_v0 {

	namespace __text_detail {
		template <bool __assume_validity = false>
		struct __pass_through_text_error_handler_with {
			using assume_valid = std::integral_constant<bool, __assume_validity>;

			template <typename __Encoding, typename Result>
			constexpr auto operator()(const __Encoding&, Result __result) const {
				return __result;
			}
		};

		using __pass_through_text_error_handler = __pass_through_text_error_handler_with<false>;
	}; // namespace __text_detail

	struct assume_valid_text_error_handler : __text_detail::__pass_through_text_error_handler_with<true> {};

	struct replacement_text_error_handler {
		template <typename __Encoding, typename __InputRange, typename __OutputRange, typename __State>
		constexpr auto operator()(const __Encoding& enc, encode_result<__InputRange, __OutputRange, __State> __result) const {
			using __uOutputRange = typename meta::remove_cv_ref<__OutputRange>::type;

			auto __outit   = ranges::begin(__result.output);
			auto __outlast = ranges::end(__result.output);
			if (__result.error_code == encoding_errc::insufficient_output_space || __outit == __outlast) {
				// BAIL
				return __result;
			}

			if constexpr (is_code_unit_replaceable_v<__Encoding>) {
				(*__outit)	 = __Encoding::replacement_code_unit;
				__outit		 = ranges::next(__outit);
				__result.output = __uOutputRange(std::move(__outit), std::move(__outlast));
			}
			else {
				using __input_code_point = encoding_code_point_t<__Encoding>;
				__input_code_point __wut[2];
				if constexpr (is_code_point_replaceable_v<__Encoding>) {
					__wut[0] = static_cast<__input_code_point>(__Encoding::replacement_code_point);
				}
				else {
					__wut[0] = static_cast<__input_code_point>(is_unicode_code_point_v<__input_code_point> ? __unicode_detail::__replacement : __unicode_detail::__ascii_replacement);
				}
				__wut[1] = static_cast<__input_code_point>(0);
				const basic_c_string_view<__input_code_point> __wut_range(__wut, 1);

				__State __fresh_state{};
				auto __encresult = enc.encode(__wut_range, std::move(__result.output), __fresh_state, assume_valid_text_error_handler{});
				__result.output  = std::move(__encresult.output);
			}

			__result.error_code = encoding_errc::ok;

			return __result;
		}

		template <typename __Encoding, typename __InputRange, typename __OutputRange, typename __State>
		constexpr auto operator()(const __Encoding&, decode_result<__InputRange, __OutputRange, __State> __result) const {
			using __output_code_point = encoding_code_point_t<__Encoding>;

			auto __outit   = ranges::begin(__result.output);
			auto __outlast = ranges::end(__result.output);
			if (__result.error_code == encoding_errc::insufficient_output_space || __outit == __outlast) {
				// BAIL
				return __result;
			}

			if constexpr (is_code_point_replaceable_v<__Encoding>) {
				(*__outit) = __Encoding::replacement_code_point;
			}
			else {
				if constexpr (is_unicode_code_point_v<__output_code_point>) {
					(*__outit) = static_cast<__output_code_point>(__unicode_detail::__replacement);
				}
				else {
					(*__outit) = static_cast<__output_code_point>(__unicode_detail::__ascii_replacement);
				}
			}
			__outit = ranges::next(__outit);

			__result.output	= __OutputRange(__outit, __outlast);
			__result.error_code = encoding_errc::ok;

			return __result;
		}
	};

	using default_text_error_handler = replacement_text_error_handler;
}
} // namespace phd::__abi_v0


#endif // PHD_TEXT_ERROR_HANDLER_HPP
