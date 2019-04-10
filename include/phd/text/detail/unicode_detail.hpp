#pragma once

#ifndef PHD_TEXT_DETAIL_UNICODE_DETAIL_HPP
#define PHD_TEXT_DETAIL_UNICODE_DETAIL_HPP

#include <phd/text/char8_t.hpp>

#include <cstddef>

namespace phd {
inline namespace __abi_v0 {
namespace __unicode_detail {
	// codepoint related
	inline constexpr char32_t __last_code_point = 0x10FFFF;

	inline constexpr char32_t __first_lead_surrogate = 0xD800;
	inline constexpr char32_t __last_lead_surrogate = 0xDBFF;

	inline constexpr char32_t __first_trail_surrogate = 0xDC00;
	inline constexpr char32_t __last_trail_surrogate = 0xDFFF;

	inline constexpr char32_t __first_surrogate = __first_lead_surrogate;
	inline constexpr char32_t __last_surrogate = __last_trail_surrogate;

	inline constexpr bool __is_lead_surrogate(char32_t __value) {
		return __value >= __first_lead_surrogate && __value <= __last_lead_surrogate;
	}
	inline constexpr bool __is_trail_surrogate(char32_t __value) {
		return __value >= __first_trail_surrogate && __value <= __last_trail_surrogate;
	}
	inline constexpr bool __is_surrogate(char32_t __value) {
		return __value >= __first_surrogate && __value <= __last_surrogate;
	}

	// utf8 related
	inline constexpr char32_t __last_1byte_value = 0x7F;
	inline constexpr char32_t __last_2byte_value = 0x7FF;
	inline constexpr char32_t __last_3byte_value = 0xFFFF;
	inline constexpr char32_t __last_4byte_value = 0x1FFFFF;
	inline constexpr char32_t __last_5byte_value = 0x3FFFFFF;
	inline constexpr char32_t __last_6byte_value = 0x7FFFFFFF;

	inline constexpr char8_t __start_1byte_mask = 0x80u;
	inline constexpr char8_t __start_1byte_continuation = 0x00u;
	inline constexpr char8_t __start_1byte_shift = 7u;
	inline constexpr char8_t __start_2byte_mask = 0xC0u;
	inline constexpr char8_t __start_2byte_continuation = __start_2byte_mask;
	inline constexpr char8_t __start_2byte_shift = 5u;
	inline constexpr char8_t __start_3byte_mask = 0xE0u;
	inline constexpr char8_t __start_3byte_continuation = __start_3byte_mask;
	inline constexpr char8_t __start_3byte_shift = 4u;
	inline constexpr char8_t __start_4byte_mask = 0xF0u;
	inline constexpr char8_t __start_4byte_continuation = __start_4byte_mask;
	inline constexpr char8_t __start_4byte_shift = 3u;
	inline constexpr char8_t __start_5byte_mask = 0xF8u;
	inline constexpr char8_t __start_5byte_continuation = __start_5byte_mask;
	inline constexpr char8_t __start_5byte_shift = 2u;
	inline constexpr char8_t __start_6byte_mask = 0xFCu;
	inline constexpr char8_t __start_6byte_continuation = __start_6byte_mask;
	inline constexpr char8_t __start_6byte_shift = 1u;

	inline constexpr char8_t __continuation_mask = 0xC0u;
	inline constexpr char8_t __continuation_signature = 0x80u;
	inline constexpr char8_t __continuation_mask_value = 0x3Fu;
	inline constexpr char8_t __single_mask_value = 0x7Fu;

	inline constexpr bool __is_invalid(char8_t __b) {
		return __b == 0xC0 || __b == 0xC1 || __b > 0xF4;
	}

	inline constexpr bool __is_continuation(char8_t __value) {
		return (__value & __continuation_mask) == __continuation_signature;
	}

	inline constexpr bool __is_overlong(char32_t __value, std::size_t __bytes) {
		return __value <= __last_1byte_value || (__value <= __last_2byte_value && __bytes > 2)
			|| (__value <= __last_3byte_value && __bytes > 3);
	}

	inline constexpr bool __is_overlong_extended(char32_t __value, std::size_t __bytes) {
		return __value <= __last_1byte_value || (__value <= __last_2byte_value && __bytes > 2)
			|| (__value <= __last_3byte_value && __bytes > 3)
			|| (__value <= __last_4byte_value && __bytes > 4)
			|| (__value <= __last_5byte_value && __bytes > 5);
	}

	template <bool __overlong_allowed = false>
	inline constexpr int __decode_length(char32_t __value) {
		if (__value <= __unicode_detail::__last_1byte_value) {
			return 1;
		}
		if (__value <= __unicode_detail::__last_2byte_value) {
			return 2;
		}
		if (__value <= __unicode_detail::__last_3byte_value) {
			return 3;
		}
		if (__value <= __unicode_detail::__last_4byte_value) {
			return 4;
		}
		if constexpr (__overlong_allowed) {
			if (__value <= __unicode_detail::__last_5byte_value) {
				return 5;
			}
			if (__value <= __unicode_detail::__last_6byte_value) {
				return 6;
			}
		}
		return 8;
	}

	inline constexpr int __sequence_length(char8_t __value) {
		return (__value & __start_1byte_mask) == __start_1byte_continuation ? 1
															   : (__value & __start_3byte_mask) != __start_3byte_continuation ? 2
																												 : (__value & __start_4byte_mask) != __start_4byte_continuation ? 3
																																								    : 4;
	}

	inline constexpr int __sequence_length_extended(char8_t __value) {
		return (__value & __start_1byte_mask) == __start_1byte_continuation ? 1
															   : (__value & __start_3byte_mask) != __start_3byte_continuation ? 2
																												 : (__value & __start_4byte_mask) != __start_4byte_continuation ? 3
																																								    : (__value & __start_5byte_mask) != __start_5byte_continuation ? 4
																																																					  : (__value & __start_6byte_mask) != __start_6byte_continuation ? 5
																																																																		: 6;
	}

	inline constexpr char32_t __decode(char8_t __value0, char8_t __value1) {
		return ((__value0 & 0x1F) << 6) | (__value1 & 0x3F);
	}
	inline constexpr char32_t __decode(char8_t __value0, char8_t __value1, char8_t __value2) {
		return ((__value0 & 0x0F) << 12) | ((__value1 & 0x3F) << 6) | (__value2 & 0x3F);
	}
	inline constexpr char32_t __decode(char8_t __value0, char8_t __value1, char8_t __value2, char8_t __value3) {
		return ((__value0 & 0x07) << 18) | ((__value1 & 0x3F) << 12) | ((__value2 & 0x3F) << 6) | (__value3 & 0x3F);
	}

	// utf16 related
	inline constexpr char32_t __last_bmp_value = 0xFFFF;
	inline constexpr char32_t __normalizing_value = 0x10000;
	inline constexpr int __lead_surrogate_bitmask = 0xFFC00;
	inline constexpr int __trail_surrogate_bitmask = 0x3FF;
	inline constexpr int __lead_shifted_bits = 10;
	inline constexpr char32_t __replacement = 0xFFFD;
	inline constexpr char32_t __ascii_replacement = 0x003F;

	inline char32_t __combine_surrogates(char16_t __lead, char16_t __trail) {
		auto __hibits = __lead - __first_lead_surrogate;
		auto __lobits = __trail - __first_trail_surrogate;
		return __normalizing_value + ((__hibits << __lead_shifted_bits) | __lobits);
	}
}
}
} // namespace phd::__abi_v0::__unicode_detail

#endif // PHD_TEXT_DETAIL_UNICODE_DETAIL_HPP
