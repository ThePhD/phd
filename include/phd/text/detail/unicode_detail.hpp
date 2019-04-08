#pragma once

#ifndef PHD_TEXT_DETAIL_UNICODE_DETAIL_HPP
#define PHD_TEXT_DETAIL_UNICODE_DETAIL_HPP

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
	inline constexpr auto __last_1byte_value = 0x7Fu;
	inline constexpr auto __last_2byte_value = 0x7FFu;
	inline constexpr auto __last_3byte_value = 0xFFFFu;

	inline constexpr auto __start_2byte_mask = 0x80u;
	inline constexpr auto __start_3byte_mask = 0xE0u;
	inline constexpr auto __start_4byte_mask = 0xF0u;

	inline constexpr auto __continuation_mask = 0xC0u;
	inline constexpr auto __continuation_signature = 0x80u;

	inline constexpr bool __is_invalid(unsigned char __b) {
		return __b == 0xC0 || __b == 0xC1 || __b > 0xF4;
	}

	inline constexpr bool __is_continuation(unsigned char __value) {
		return (__value & __continuation_mask) == __continuation_signature;
	}

	inline constexpr bool __is_overlong(char32_t __value, std::size_t __bytes) {
		return __value <= __last_1byte_value || (__value <= __last_2byte_value && __bytes > 2)
			|| (__value <= __last_3byte_value && __bytes > 3);
	}

	inline constexpr int __sequence_length(unsigned char __value) {
		return (__value & __start_2byte_mask) == 0 ? 1
										   : (__value & __start_3byte_mask) != __start_3byte_mask ? 2
																					   : (__value & __start_4byte_mask) != __start_4byte_mask ? 3
																																   : 4;
	}

	inline constexpr char32_t __decode(unsigned char __value0, unsigned char __value1) {
		return ((__value0 & 0x1F) << 6) | (__value1 & 0x3F);
	}
	inline constexpr char32_t __decode(unsigned char __value0, unsigned char __value1, unsigned char __value2) {
		return ((__value0 & 0x0F) << 12) | ((__value1 & 0x3F) << 6) | (__value2 & 0x3F);
	}
	inline constexpr char32_t __decode(unsigned char __value0, unsigned char __value1, unsigned char __value2, unsigned char __value3) {
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
};
}
} // namespace phd::__abi_v0::__unicode_detail

#endif // PHD_TEXT_DETAIL_UNICODE_DETAIL_HPP
