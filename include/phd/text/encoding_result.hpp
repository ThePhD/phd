#pragma once

#ifndef PHD_TEXT_ENCODING_RESULT_HPP
#define PHD_TEXT_ENCODING_RESULT_HPP

#include <phd/text/char8_t.hpp>
#include <phd/text/unicode_code_point.hpp>
#include <phd/text/encoding_error.hpp>

#include <cstddef>
#include <array>
#include <system_error>

namespace phd {

	template <typename __In, typename __Out>
	struct encoding_result {
		__In input;
		__Out output;
		encoding_errc error_code;

		template <typename __InRange, typename __OutRange>
		constexpr encoding_result(__InRange&& __in, __OutRange&& __out, encoding_errc __error_code = encoding_errc::ok)
		: input(std::forward<__InRange>(__in)), output(std::forward<__OutRange>(__out)), error_code(__error_code) {
		}

		// TODO: when the standard catches up
		/* constexpr */ std::error_condition error() const {
			return make_error_condition(error);
		}
	};
} // namespace phd

#endif // PHD_TEXT_ENCODING_RESULT_HPP
