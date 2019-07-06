#include <phd/text/decode.hpp>
#include <phd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <range/v3/core.hpp>
#include <range/v3/span.hpp>


#include <phd/text/tests/basic_unicode_strings.hpp>

TEST_CASE("text/encode/core", "basic usages of encode function do not explode") {
	SECTION("char") {
		std::string result0 = phd::text_encode(phd::text::tests::u32_ansi_sequence_truth, phd::execution{});
		REQUIRE(result0 == phd::text::tests::ansi_sequence_truth);
	}
	SECTION("wchar_t") {
		std::wstring result0 = phd::text_encode(phd::text::tests::u32_ansi_sequence_truth, phd::wide_execution{});
		REQUIRE(result0 == phd::text::tests::w_ansi_sequence_truth);

		std::wstring result1 = phd::text_encode(phd::text::tests::u32_unicode_sequence_truth, phd::wide_execution{});
		REQUIRE(result1 == phd::text::tests::w_unicode_sequence_truth);
	}
	SECTION("char8_t") {
		std::basic_string<char8_t> result0 = phd::text_encode(phd::text::tests::u32_ansi_sequence_truth, phd::utf8{});
		REQUIRE(result0 == phd::text::tests::u8_ansi_sequence_truth);

		std::basic_string<char8_t> result1 = phd::text_encode(phd::text::tests::u32_unicode_sequence_truth, phd::utf8{});
		REQUIRE(result1 == phd::text::tests::u8_unicode_sequence_truth);
	}
	SECTION("char16_t") {
		std::u16string result0 = phd::text_encode(phd::text::tests::u32_ansi_sequence_truth, phd::utf16{});
		REQUIRE(result0 == phd::text::tests::u16_ansi_sequence_truth);

		std::u16string result1 = phd::text_encode(phd::text::tests::u32_unicode_sequence_truth, phd::utf16{});
		REQUIRE(result1 == phd::text::tests::u16_unicode_sequence_truth);
	}
	SECTION("char32_t") {
		std::u32string result0 = phd::text_encode(phd::text::tests::u32_ansi_sequence_truth, phd::utf32{});
		REQUIRE(result0 == phd::text::tests::u32_ansi_sequence_truth);

		std::u32string result1 = phd::text_encode(phd::text::tests::u32_unicode_sequence_truth, phd::utf32{});
		REQUIRE(result1 == phd::text::tests::u32_unicode_sequence_truth);
	}
}
