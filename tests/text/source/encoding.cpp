#include <phd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <range/v3/core.hpp>
#include <range/v3/span.hpp>

#include <phd/text/tests/basic_unicode_strings.hpp>

TEST_CASE("text/encoding/core", "basic usages of encoding do not explode") {
	constexpr const std::size_t encode_output_max = 16;
	constexpr const std::size_t decode_output_max = 2;

	SECTION("decode") {
		SECTION("char") {
			phd::execution enc;

			phd::unicode_code_point output0[decode_output_max]{};
			phd::execution::state s{};
			auto result0 = enc.decode(phd::c_string_view(phd::text::tests::ansi_single_truth), ranges::span(output0, 1), s, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_ansi_single_truth);
		}
		SECTION("wchar_t") {
			phd::wide_execution enc;

			phd::unicode_code_point output0[decode_output_max]{};
			phd::wide_execution::state s0{};
			auto result0 = enc.decode(phd::wc_string_view(phd::text::tests::w_ansi_single_truth), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_ansi_single_truth);

			phd::unicode_code_point output1[decode_output_max]{};
			phd::wide_execution::state s1{};
			auto result1 = enc.decode(phd::wc_string_view(phd::text::tests::w_unicode_single_truth), ranges::span(output1, 1), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u32c_string_view(output1) == phd::text::tests::u32_unicode_single_truth);
		}
		SECTION("char8_t") {
			phd::utf8 enc;

			phd::unicode_code_point output0[decode_output_max]{};
			phd::utf8::state s0{};
			auto result0 = enc.decode(phd::u8c_string_view(phd::text::tests::u8_ansi_single_truth), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_ansi_single_truth);

			phd::unicode_code_point output1[decode_output_max]{};
			phd::utf8::state s1{};
			auto result1 = enc.decode(phd::u8c_string_view(phd::text::tests::u8_unicode_single_truth), ranges::span(output1, 1), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u32c_string_view(output1) == phd::text::tests::u32_unicode_single_truth);
		}
		SECTION("char16_t") {
			phd::utf16 enc;

			phd::unicode_code_point output0[decode_output_max]{};
			phd::utf16::state s0{};
			auto result0 = enc.decode(phd::u16c_string_view(phd::text::tests::u16_ansi_single_truth), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_ansi_single_truth);

			phd::unicode_code_point output1[decode_output_max]{};
			phd::utf16::state s1{};
			auto result1 = enc.decode(phd::u16c_string_view(phd::text::tests::u16_unicode_single_truth), ranges::span(output1, 1), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u32c_string_view(output1) == phd::text::tests::u32_unicode_single_truth);
		}
		SECTION("char32_t") {
			phd::utf32 enc;

			phd::unicode_code_point output0[decode_output_max]{};
			phd::utf32::state s0{};
			auto result0 = enc.decode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_single_truth);

			phd::unicode_code_point output1[decode_output_max]{};
			phd::utf32::state s1{};
			auto result1 = enc.decode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output1, 1), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_single_truth);
		}
	}
	SECTION("encode") {
		SECTION("char") {
			phd::execution enc;

			char output0[encode_output_max]{};
			phd::execution::state s0{};
			auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_ansi_single_truth), ranges::span(output0, 7), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::c_string_view(output0) == phd::text::tests::ansi_single_truth);
		}
		SECTION("wchar_t") {
			phd::wide_execution enc;

			wchar_t output0[encode_output_max]{};
			phd::wide_execution::state s0{};
			auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_ansi_single_truth), ranges::span(output0, 7), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::wc_string_view(output0) == phd::text::tests::w_ansi_single_truth);

			wchar_t output1[encode_output_max]{};
			phd::wide_execution::state s1{};
			auto result1 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output0, 7), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::wc_string_view(output0) == phd::text::tests::w_unicode_single_truth);
		}
		SECTION("char8_t") {
			phd::utf8 enc;

			char8_t output0[encode_output_max]{};
			phd::utf8::state s0{};
			auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_ansi_single_truth), ranges::span(output0, encode_output_max), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u8c_string_view(output0) == phd::text::tests::u8_ansi_single_truth);

			char8_t output1[encode_output_max]{};
			phd::utf8::state s1{};
			auto result1 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output1, encode_output_max), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u8c_string_view(output1) == phd::text::tests::u8_unicode_single_truth);
		}
		SECTION("char16_t") {
			phd::utf16 enc;

			char16_t output0[encode_output_max]{};
			phd::utf16::state s0{};
			auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output0, encode_output_max), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u16c_string_view(output0) == phd::text::tests::u16_unicode_single_truth);

			char16_t output1[encode_output_max]{};
			phd::utf16::state s1{};
			auto result1 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output1, encode_output_max), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u16c_string_view(output1) == phd::text::tests::u16_unicode_single_truth);
		}
		SECTION("char32_t") {
			phd::utf32 enc;

			char32_t output0[encode_output_max]{};
			phd::utf32::state s0{};
			auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output0, encode_output_max), s0, phd::default_text_error_handler{});
			REQUIRE(result0.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result0.error());
			REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_single_truth);

			char32_t output1[encode_output_max]{};
			phd::utf32::state s1{};
			auto result1 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_single_truth), ranges::span(output1, encode_output_max), s1, phd::default_text_error_handler{});
			REQUIRE(result1.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(result1.error());
			REQUIRE(phd::u32c_string_view(output1) == phd::text::tests::u32_unicode_single_truth);
		}
	}
}
