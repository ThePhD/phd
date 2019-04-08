#pragma once

#ifndef PHD_TEXT_ERROR_HANDLER_HPP
#define PHD_TEXT_ERROR_HANDLER_HPP

#include <phd/text/encoding_error.hpp>
#include <phd/text/encoding_result.hpp>
#include <phd/text/is_unicode_code_point.hpp>
#include <phd/text/is_code_point_replaceable.hpp>
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

	struct assume_valid_error_handler {
		template <typename __Encoding, typename Result, typename __State>
		auto operator()(const __Encoding& enc, Result __result) const {
			return __result;
		}
	};

	struct replacement_error_handler {
		template <typename __Encoding, typename __State, typename __InputRange, typename __OutputRange>
		constexpr auto operator()(const __Encoding& enc, encoding_result<__InputRange, __OutputRange, __State> __result) const {
			auto __outit = ranges::begin(__result.output);
			auto __outsentinel = ranges::end(__result.output);
			if (__outit == __outsentinel) {
				// BAIL
				return __result;
			}

			using __input_code_point = encoding_code_point_t<__Encoding>;

			__input_code_point __wut[2];
			if constexpr (is_code_point_replaceable<__Encoding>) {
				__wut[0] = static_cast<__input_code_point>(__Encoding::replacement_code_point);
			}
			else {
				__wut[0] = static_cast<__input_code_point>(is_unicode_code_point_v<__input_code_point> ? U'\uFFFD' : U'?');
			}
			__wut[1] = static_cast<__input_code_point>(0);
			const basic_c_string_view<__input_code_point> __wut_range(__wut, 1);

			__State __fresh_state{};
			auto __encres = enc.encode(__wut_range, __result.output, __fresh_state, assume_valid_error_handler{});
			__result.output = std::move(__encres.output);

			return __result;
		}

		template <typename __Encoding, typename __State, typename __InputRange, typename __OutputRange>
		constexpr auto operator()(const __Encoding&, decoding_result<__InputRange, __OutputRange, __State> __result) const {
			using __output_code_point = encoding_code_point_t<__Encoding>;
			//(void)enc; // UNUSED

			auto __outit = ranges::begin(__result.output);
			auto __outsentinel = ranges::end(__result.output);
			if (__result.error_code == encoding_errc::insufficient_output_space || __outit == __outsentinel) {
				// BAIL
				return __result;
			}

			if constexpr (is_code_point_replaceable_v<__output_code_point>) {
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

			__result.output = __OutputRange(__outit, __outsentinel);

			return __result;
		}
	};

	using default_error_handler = replacement_error_handler;
}
} // namespace phd::__abi_v0


#endif // PHD_TEXT_ERROR_HANDLER_HPP
