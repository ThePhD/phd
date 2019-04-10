#pragma once

#ifndef PHD_TEXT_TRANSCODE_HPP
#define PHD_TEXT_TRANSCODE_HPP

#include <phd/text/encoding_result.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/state.hpp>
#include <phd/text/code_point.hpp>
#include <phd/text/code_unit.hpp>

#include <phd/meta/is_character.hpp>
#include <phd/meta/remove_cv_ref.hpp>

#include <range/v3/begin_end.hpp>
#include <range/v3/view/bounded.hpp>
#include <range/v3/span.hpp>

namespace phd {

	template <typename __Input, typename __Output, typename __EncodingFrom, typename __EncodingTo, typename __StateFrom, typename __StateTo, typename __ErrorHandlerFrom, typename __ErrorHandlerTo>
	constexpr void text_transcode_into(__Input&& __input, __Output&& __output, __EncodingFrom&& __encoding_from, __EncodingTo&& __encoding_to, __StateFrom& __s_from, __StateTo& __s_to, __ErrorHandlerFrom&& __error_handler_from, __ErrorHandlerTo&& __error_handler_to) {
		using __uInput = typename meta::remove_cv_ref<__Input>::type;
		using __uOutput = typename meta::remove_cv_ref<__Output>::type;
		using __InputValueType = ranges::value_type_t<__uInput>;
		//using __OutputValueType = ranges::value_type_t<__uOutput>;
		using __WorkingInput = std::conditional_t<std::is_array_v<__uInput>, std::conditional_t<meta::is_character_v<__InputValueType>, basic_c_string_view<__InputValueType>, ranges::span<__InputValueType>>, __uInput>;
		using __WorkingOutput = __uOutput; /*std::conditional_t<std::is_array_v<__uOutput>, ranges::span<__OutputValueType>, __uOutput>;*/

		using __uEncodingFrom = typename meta::remove_cv_ref<__EncodingFrom>::type;
		using __intermediate_code_point = encoding_code_point_t<__uEncodingFrom>;

		__WorkingInput __working_input(std::forward<__Input>(__input));
		__WorkingOutput __working_output(std::forward<__Output>(__output));

		__intermediate_code_point __intermediary_storage[8 + 1];
		auto __scratch_space = ranges::span(__intermediary_storage, 8);
		for (;;) {
			auto __decode_result = __encoding_from.decode(__working_input, __scratch_space, __s_from, __error_handler_from);
			if (__decode_result.error_code != encoding_errc::ok) {
				break;
			}
			auto __intermediary_storage_used = ranges::span(__intermediary_storage, __decode_result.output.data());
			auto __encode_result = __encoding_to.encode(__intermediary_storage_used, __working_output, __s_to, __error_handler_to);
			if (__encode_result.error_code != encoding_errc::ok) {
				break;
			}
			if (ranges::empty(__decode_result.input)) {
				break;
			}
			__working_input = std::move(__decode_result.input);
			__working_output = std::move(__encode_result.output);
		}
	}

	template <typename __Input, typename __EncodingFrom, typename __EncodingTo, typename __StateFrom, typename __StateTo>
	constexpr auto text_transcode(__Input&& __input, __EncodingFrom&& __encoding_from, __EncodingTo&& __encoding_to, __StateFrom& __s_from, __StateTo& __s_to) {
		using __uToEncoding = typename meta::remove_cv_ref<__EncodingTo>::type;
		using __code_unit = encoding_code_unit_t<__uToEncoding>;

		std::basic_string<__code_unit> __output{};
		if constexpr (ranges::SizedRange<__Input>::value) {
			// NOTE: any ol' estimate will do
			__output.reserve(ranges::size(__input) / 2);
		}

		text_transcode_into(std::forward<__Input>(__input), ranges::unbounded_view(ranges::back_inserter(__output)), std::forward<__EncodingFrom>(__encoding_from), std::forward<__EncodingTo>(__encoding_to), __s_from, __s_to, phd::default_text_error_handler{}, phd::default_text_error_handler{});
		return __output;
	}

	template <typename __Input, typename __EncodingFrom, typename __EncodingTo>
	constexpr auto text_transcode(__Input&& __input, __EncodingFrom&& __encoding_from, __EncodingTo&& __encoding_to) {
		using __uEncodingFrom = typename meta::remove_cv_ref<__EncodingFrom>::type;
		using __uEncodingTo = typename meta::remove_cv_ref<__EncodingTo>::type;
		using __state_from = encoding_state_t<__uEncodingFrom>;
		using __state_to = encoding_state_t<__uEncodingTo>;

		__state_from __s_from{};
		__state_to __s_to{};

		return text_transcode(std::forward<__Input>(__input), std::forward<__EncodingFrom>(__encoding_from), std::forward<__EncodingTo>(__encoding_to), __s_from, __s_to);
	}

} // namespace phd

#endif // PHD_TEXT_TRANSCODE_HPP
