#pragma once

#ifndef PHD_TEXT_DECODE_HPP
#define PHD_TEXT_DECODE_HPP

#include <phd/text/encode_result.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/state.hpp>
#include <phd/text/code_point.hpp>

#include <phd/meta/is_character.hpp>
#include <phd/meta/remove_cv_ref.hpp>

#include <phd/text/c_string_view.hpp>

#include <range/v3/core.hpp>
#include <range/v3/empty.hpp>
#include <range/v3/span.hpp>
#include <range/v3/view/unbounded.hpp>

namespace phd {

	template <typename __Input, typename __Output, typename __Encoding, typename __State, typename __ErrorHandler>
	constexpr void text_decode_into(__Input&& __input, __Output&& __output, __Encoding&& __encoding, __State& __state, __ErrorHandler&& __error_handler) {
		using __uInput		   = typename meta::remove_cv_ref<__Input>::type;
		using __uOutput	   = typename meta::remove_cv_ref<__Output>::type;
		using __InputValueType = ranges::value_type_t<__uInput>;
		//using __OutputValueType = ranges::value_type_t<__uOutput>;
		using __WorkingInput  = std::conditional_t<std::is_array_v<__uInput>, std::conditional_t<meta::is_character_v<__InputValueType>, basic_c_string_view<__InputValueType>, ranges::span<__InputValueType>>, __uInput>;
		using __WorkingOutput = __uOutput; /*std::conditional_t<std::is_array_v<__uOutput>, ranges::span<__OutputValueType>, __uOutput>;*/

		__WorkingInput __working_input(std::forward<__Input>(__input));
		__WorkingOutput __working_output(std::forward<__Output>(__output));

		for (;;) {
			auto __result = __encoding.decode(__working_input, __working_output, __state, __error_handler);
			if (__result.error_code != encoding_errc::ok) {
				break;
			}
			if (ranges::empty(__result.input)) {
				break;
			}
			__working_input  = std::move(__result.input);
			__working_output = std::move(__result.output);
		}
	}

	template <typename __Input, typename __Encoding, typename __State, typename __ErrorHandler>
	auto text_decode(__Input&& __input, __Encoding&& __encoding, __State& __state, __ErrorHandler&& __error_handler) {
		using __uEncoding  = typename meta::remove_cv_ref<__Encoding>::type;
		using __code_point = encoding_code_point_t<__uEncoding>;

		std::basic_string<__code_point> __output{};
		if constexpr (ranges::SizedRange<__Input>::value) {
			// NOTE: any ol' estimate will do
			__output.reserve(ranges::size(__input) / 2);
		}
		text_decode_into(std::forward<__Input>(__input), ranges::unbounded_view(ranges::back_inserter(__output)), std::forward<__Encoding>(__encoding), __state, std::forward<__ErrorHandler>(__error_handler));
		return __output;
	}

	template <typename __Input, typename __Encoding, typename __ErrorHandler>
	auto text_decode(__Input&& __input, __Encoding&& __encoding, __ErrorHandler&& __error_handler) {
		using __uEncoding = typename meta::remove_cv_ref<__Encoding>::type;
		using __state	= encoding_state_t<__uEncoding>;

		__state __s{};
		return text_decode(std::forward<__Input>(__input), std::forward<__Encoding>(__encoding), __s, std::forward<__ErrorHandler>(__error_handler));
	}

	template <typename __Input, typename __Encoding>
	auto text_decode(__Input&& __input, __Encoding&& __encoding) {
		return text_decode(std::forward<__Input>(__input), std::forward<__Encoding>(__encoding), default_text_error_handler{});
	}

} // namespace phd

#endif // PHD_TEXT_DECODE_HPP
