#pragma once

#ifndef PHD_TEXT_TESTS_BASIC_UNICODE_STRINGS_HPP
#define PHD_TEXT_TESTS_BASIC_UNICODE_STRINGS_HPP

#include <string>
#include <memory>

namespace phd { namespace text { namespace tests {

	inline constexpr const char32_t u32_unicode_sequence_truth[] = U"z♥䕦🍷私はガラスを食べられます。それは私を傷つけません。𐌼𐌰𐌲 𐌲𐌻𐌴𐍃 𐌹̈𐍄𐌰𐌽, 𐌽𐌹 𐌼𐌹𐍃 𐍅𐌿 𐌽𐌳𐌰𐌽 𐌱𐍂𐌹𐌲𐌲𐌹𐌸.யாமறிந்த மொழிகளிலே தமிழ்மொழி போல் இனிதாவது எங்கும் காணோம்,На берегу пустынных волн € أنا قادر على أكل الزجاج و هذا لا يؤلمني.";
	inline constexpr const char16_t u16_unicode_sequence_truth[] = u"z♥䕦🍷私はガラスを食べられます。それは私を傷つけません。𐌼𐌰𐌲 𐌲𐌻𐌴𐍃 𐌹̈𐍄𐌰𐌽, 𐌽𐌹 𐌼𐌹𐍃 𐍅𐌿 𐌽𐌳𐌰𐌽 𐌱𐍂𐌹𐌲𐌲𐌹𐌸.யாமறிந்த மொழிகளிலே தமிழ்மொழி போல் இனிதாவது எங்கும் காணோம்,На берегу пустынных волн € أنا قادر على أكل الزجاج و هذا لا يؤلمني.";
	inline constexpr const char8_t u8_unicode_sequence_truth[]   = u8"z♥䕦🍷私はガラスを食べられます。それは私を傷つけません。𐌼𐌰𐌲 𐌲𐌻𐌴𐍃 𐌹̈𐍄𐌰𐌽, 𐌽𐌹 𐌼𐌹𐍃 𐍅𐌿 𐌽𐌳𐌰𐌽 𐌱𐍂𐌹𐌲𐌲𐌹𐌸.யாமறிந்த மொழிகளிலே தமிழ்மொழி போல் இனிதாவது எங்கும் காணோம்,На берегу пустынных волн € أنا قادر على أكل الزجاج و هذا لا يؤلمني.";
	inline constexpr const wchar_t w_unicode_sequence_truth[]    = L"z♥䕦🍷私はガラスを食べられます。それは私を傷つけません。𐌼𐌰𐌲 𐌲𐌻𐌴𐍃 𐌹̈𐍄𐌰𐌽, 𐌽𐌹 𐌼𐌹𐍃 𐍅𐌿 𐌽𐌳𐌰𐌽 𐌱𐍂𐌹𐌲𐌲𐌹𐌸.யாமறிந்த மொழிகளிலே தமிழ்மொழி போல் இனிதாவது எங்கும் காணோம்,На берегу пустынных волн € أنا قادر على أكل الزجاج و هذا لا يؤلمني.";

	inline constexpr const char32_t u32_ansi_sequence_truth[] = U"zá:iï";
	inline constexpr const char16_t u16_ansi_sequence_truth[] = u"zá:iï";
	inline constexpr const char8_t u8_ansi_sequence_truth[]   = u8"";
	inline constexpr const wchar_t w_ansi_sequence_truth[]    = L"zá:iï";
	inline constexpr const char ansi_sequence_truth[]		   = "zá:iï";

	inline constexpr const char32_t u32_unicode_single_truth[] = U"♥";
	inline constexpr const char16_t u16_unicode_single_truth[] = u"♥";
	inline constexpr const char8_t u8_unicode_single_truth[]   = u8"♥";
	inline constexpr const wchar_t w_unicode_single_truth[]    = L"♥";

	inline constexpr const char32_t u32_ansi_single_truth[] = U"á";
	inline constexpr const char16_t u16_ansi_single_truth[] = u"á";
	inline constexpr const char8_t u8_ansi_single_truth[]   = u8"á";
	inline constexpr const wchar_t w_ansi_single_truth[]    = L"á";
	inline constexpr const char ansi_single_truth[]		 = "á";

	inline constexpr const char32_t u32_unicode_invalid_input[] = { 0xFFFFFFFF, 0 };
	inline constexpr const char16_t u16_unicode_invalid_input[] = { 0xD800, 0 };
	inline constexpr const char8_t u8_unicode_invalid_input[]   = { 0xC0, 0 };

	inline constexpr const char32_t u32_unicode_replacement_truth[] = U"\uFFFD";
	inline constexpr const char16_t u16_unicode_replacement_truth[] = u"\uFFFD";
	inline constexpr const char8_t u8_unicode_replacement_truth[]   = u8"\uFFFD";

}}} // namespace phd::text::tests

#endif // PHD_TEXT_TESTS_BASIC_UNICODE_STRINGS_HPP
