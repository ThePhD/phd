#pragma once

#ifndef PHD_TEXT_DECODE_HPP
#define PHD_TEXT_DECODE_HPP

#include <phd/text/encoding_result.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/state.hpp>

#include <range/v3/core.hpp>
#include <range/v3/empty.hpp>

namespace phd {

	template <typename __Input, typename __Output, typename __Encoding, typename __State, typename __ErrorHandler>
	constexpr void text_decode_into(__Input&& __input, __Output&& __output, __Encoding&& __encoding, __State& __state, __ErrorHandler&& __error_handler) {
		auto __working_input = std::forward<__Input>(__input);
		auto __working_output = std::forward<__Output>(__output);
		for (;;) {
			auto result = __encoding.decode(__working_input, __working_output, __state, __error_handler);
			if (result.error_code != encoding_errc::ok) {
				break;
			}
			if (ranges::empty(__working_input)) {
				break;
			}
			__working_input = std::move(result.input);
			__working_output = std::move(result.output);
		}
	}

	template <typename __Input, typename __Output, typename __Encoding, typename __State, typename __ErrorHandler>
	auto text_decode(__Input&& __input, __Encoding&& __encoding, __State& __state, __ErrorHandler&& __error_handler) {
		using __uEncoding = typename meta::remove_cv_ref<__Encoding>::type;
		using __code_point = encoding_code_point_t<__uEncoding>;

		std::basic_string<__code_point> __output{};
		if constexpr (ranges::SizedRange<__Input>::value) {
			// NOTE: any ol' estimate will do
			__output.reserve(ranges::size(__input) / 2);
		}
		text_decode_into(std::forward<__Input>(__input), ranges::back_inserter(__output), std::forward<__Encoding>(__encoding), __state, std::forward<__ErrorHandler>(__error_handler));
		return __output;
	}

	template <typename __Input, typename __Encoding, typename __ErrorHandler>
	auto text_decode(__Input&& __input, __Encoding&& __encoding, __ErrorHandler&& __error_handler) {
		using __uEncoding = typename meta::remove_cv_ref<__Encoding>::type;
		using __state = encoding_state_t<__uEncoding>;

		__state __s{};
		return text_decode(std::forward<__Input>(__input), std::forward<__Encoding>(__encoding), __s, std::forward<__ErrorHandler>(__error_handler));
	}

	template <typename __Input, typename __Encoding>
	auto text_decode(__Input&& __input, __Encoding&& __encoding) {
		return text_decode(std::forward<__Input>(__input), std::forward<__Encoding>(__encoding), default_text_error_handler{});
	}

} // namespace phd

#endif // PHD_TEXT_DECODE_HPP
