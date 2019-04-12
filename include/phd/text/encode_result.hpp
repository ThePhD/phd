#pragma once

#ifndef PHD_TEXT_ENCODING_RESULT_HPP
#define PHD_TEXT_ENCODING_RESULT_HPP

#include <phd/text/char8_t.hpp>
#include <phd/text/unicode_code_point.hpp>
#include <phd/text/encoding_error.hpp>

#include <cstddef>
#include <array>
#include <utility>
#include <system_error>

namespace phd {

	template <typename __Input, typename __Output, typename __State>
	struct encode_result {
		__Input input;
		__Output output;
		__State& state;
		encoding_errc error_code;
		bool handled_error;

		template <typename __InRange, typename __OutRange, typename __EncodingState>
		constexpr encode_result(__InRange&& __input, __OutRange&& __output, __EncodingState&& __state, encoding_errc __error_code = encoding_errc::ok)
		: encode_result(std::forward<__InRange>(__input), std::forward<__OutRange>(__output), std::forward<__EncodingState>(__state), __error_code, __error_code != encoding_errc::ok) {
		}

		template <typename __InRange, typename __OutRange, typename __EncodingState>
		constexpr encode_result(__InRange&& __input, __OutRange&& __output, __EncodingState&& __state, encoding_errc __error_code, bool __handled_error)
		: input(std::forward<__InRange>(__input)), output(std::forward<__OutRange>(__output)), state(std::forward<__EncodingState>(__state)), error_code(__error_code), handled_error(__handled_error) {
		}

		// TODO: when the standard catches up
		/* constexpr */ std::error_condition error() const {
			return make_error_condition(error_code);
		}
	};

	template <typename __Input, typename __Output, typename __State>
	struct decode_result {
		__Input input;
		__Output output;
		__State& state;
		encoding_errc error_code;
		bool handled_error;

		template <typename __InRange, typename __OutRange, typename __EncodingState>
		constexpr decode_result(__InRange&& __input, __OutRange&& __output, __EncodingState&& __state, encoding_errc __error_code = encoding_errc::ok)
		: decode_result(std::forward<__InRange>(__input), std::forward<__OutRange>(__output), std::forward<__EncodingState>(__state), __error_code, __error_code != encoding_errc::ok) {
		}

		template <typename __InRange, typename __OutRange, typename __EncodingState>
		constexpr decode_result(__InRange&& __input, __OutRange&& __output, __EncodingState&& __state, encoding_errc __error_code, bool __handled_error)
		: input(std::forward<__InRange>(__input)), output(std::forward<__OutRange>(__output)), state(std::forward<__EncodingState>(__state)), error_code(__error_code), handled_error(__handled_error) {
		}

		// TODO: when the standard catches up
		/* constexpr */ std::error_condition error() const {
			return make_error_condition(error_code);
		}
	};
} // namespace phd

#endif // PHD_TEXT_ENCODING_RESULT_HPP
