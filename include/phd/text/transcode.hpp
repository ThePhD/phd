#pragma once

#ifndef PHD_TEXT_TRANSCODE_HPP
#define PHD_TEXT_TRANSCODE_HPP

#include <phd/text/encoding_result.hpp>
#include <phd/text/error_handler.hpp>
#include <phd/text/state.hpp>
#include <phd/text/code_point.hpp>

#include <range/v3/begin_end.hpp>
#include <range/v3/view/bounded.hpp>

namespace phd {

	template <typename __Input, typename __Output, typename __EncodingFrom, typename __EncodingTo, typename __StateFrom, typename __StateTo, typename __ErrorHandler>
	constexpr void transcode_into(__Input&& __input, __Output&& __output, __EncodingFrom&& __encoding_from, __EncodingTo&& __encoding_to, __StateFrom& __state_from, __StateTo& __state_to, __ErrorHandler&& __error_handler) {
		using __uEncodingFrom = typename std::remove_cvref<__EncodingFrom>::type;
		using __intermediate_code_point = encoding_code_point_t<__uEncodingFrom>;
		__intermediate_code_point __intermediary_storage[8];
		auto __working_input = std::forward<__Input>(__input);
		auto __working_output = std::forward<__Output>(__output);
		auto for (;;) {
			auto __decode_result = __encoding.decode(__working_input, ranges::span(__intermediary_storage), __s, __error_handler);
			if (__decode_result.error_code != encoding_errc::ok) {
				break;
			}
			if (ranges::empty(__decode_result.__working_input)) {
				break;
			}
			auto __intermediary_storage_used = ranges::view::bounded(ranges::cbegin(__intermediary_storage), ranges::cbegin(__decode_result.output));
			auto __encode_result = __encoding.encode(__intermediary_storage_used, __working_output, __s, __error_handler);
			if (__encode_result.error_code != encoding_errc::ok) {
				break;
			}
			__working_input = std::move(__decode_result.input);
			__working_output = std::move(__encode_result.output);
		}
	}

} // namespace phd

#endif // PHD_TEXT_TRANSCODE_HPP
