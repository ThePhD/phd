#include <phd/text/transcode.hpp>
#include <phd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <range/v3/core.hpp>
#include <range/v3/span.hpp>


TEST_CASE("text/transcode/roundtrip", "transcode can roundtrip") {
	constexpr const char32_t u32_unicode_truth[] = U"z♥䕦🍷";
	constexpr const char16_t u16_unicode_truth[] = u"z♥䕦🍷";
	constexpr const char8_t u8_unicode_truth[] = u8"z♥䕦🍷";
	constexpr const wchar_t w_unicode_truth[] = L"z♥䕦🍷";

	constexpr const char32_t u32_ansi_truth[] = U"zá:iï";
	constexpr const char16_t u16_ansi_truth[] = u"zá:iï";
	constexpr const char8_t u8_ansi_truth[] = u8"zá:iï";
	constexpr const wchar_t w_ansi_truth[] = L"zá:iï";
#ifdef _WIN32
	constexpr const char ansi_truth[] = "z\xE1:i\xEF";
#else
	constexpr const char ansi_truth[] = "zá:iï";
#endif

	SECTION("char") {
		std::string result = phd::text_transcode(ansi_truth, phd::execution{}, phd::execution{});
		REQUIRE(result == ansi_truth);
	}
	SECTION("wchar_t") {
		std::wstring result0 = phd::text_transcode(w_ansi_truth, phd::wide_execution{}, phd::wide_execution{});
		REQUIRE(result0 == w_ansi_truth);

		std::wstring result1 = phd::text_transcode(w_unicode_truth, phd::wide_execution{}, phd::wide_execution{});
		REQUIRE(result1 == w_unicode_truth);
	}
	SECTION("char8_t") {
		std::basic_string<char8_t> result0 = phd::text_transcode(u8_ansi_truth, phd::utf8{}, phd::utf8{});
		REQUIRE(result0 == u8_ansi_truth);

		std::basic_string<char8_t> result1 = phd::text_transcode(u8_unicode_truth, phd::utf8{}, phd::utf8{});
		REQUIRE(result1 == u8_unicode_truth);
	}
	SECTION("char16_t") {
		std::u16string result0 = phd::text_transcode(u16_ansi_truth, phd::utf16{}, phd::utf16{});
		REQUIRE(result0 == u16_ansi_truth);

		std::u16string result1 = phd::text_transcode(u16_unicode_truth, phd::utf16{}, phd::utf16{});
		REQUIRE(result1 == u16_unicode_truth);
	}
	SECTION("char32_t") {
		std::u32string result0 = phd::text_transcode(u32_ansi_truth, phd::utf32{}, phd::utf32{});
		REQUIRE(result0 == u32_ansi_truth);

		std::u32string result1 = phd::text_transcode(u32_unicode_truth, phd::utf32{}, phd::utf32{});
		REQUIRE(result1 == u32_unicode_truth);
	}
}
