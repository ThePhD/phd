#include <phd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <range/v3/core.hpp>
#include <range/v3/span.hpp>

#include <phd/text/tests/basic_unicode_strings.hpp>

#include <iostream>

TEST_CASE("text/encoding/errors", "invalid characters are handled") {
	constexpr const std::size_t encode_output_max = 16;
	constexpr const std::size_t decode_output_max = 2;

	SECTION("replacement characters") {
		SECTION("decode") {
			SECTION("char8_t") {
				phd::utf8 enc;

				phd::unicode_code_point output0[decode_output_max]{};
				phd::utf8::state s0{};
				auto result0 = enc.decode(phd::u8c_string_view(phd::text::tests::u8_unicode_invalid_input), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
				REQUIRE(result0.error_code == phd::encoding_errc::ok);
				REQUIRE_FALSE(result0.error());
				REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_replacement_truth);
			}
			SECTION("char16_t") {
				phd::utf16 enc;

				phd::unicode_code_point output0[decode_output_max]{};
				phd::utf16::state s0{};
				auto result0 = enc.decode(phd::u16c_string_view(phd::text::tests::u16_unicode_invalid_input), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
				REQUIRE(result0.error_code == phd::encoding_errc::ok);
				REQUIRE_FALSE(result0.error());
				REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_replacement_truth);
			}
			SECTION("char32_t") {
				phd::utf32 enc;

				phd::unicode_code_point output0[decode_output_max]{};
				phd::utf32::state s0{};
				auto result0 = enc.decode(phd::u32c_string_view(phd::text::tests::u32_unicode_invalid_input), ranges::span(output0, 1), s0, phd::default_text_error_handler{});
				REQUIRE(result0.error_code == phd::encoding_errc::ok);
				REQUIRE_FALSE(result0.error());
				REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_replacement_truth);
			}
		}
		SECTION("encode") {
			SECTION("char8_t") {
				phd::utf8 enc;

				char8_t output0[encode_output_max]{};
				phd::utf8::state s0{};
				auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_invalid_input), ranges::span(output0, encode_output_max), s0, phd::default_text_error_handler{});
				REQUIRE(result0.error_code == phd::encoding_errc::ok);
				REQUIRE_FALSE(result0.error());
				REQUIRE(phd::u8c_string_view(output0) == phd::text::tests::u8_unicode_replacement_truth);
			}
			SECTION("char16_t") {
				phd::utf16 enc;

				char16_t output0[encode_output_max]{};
				phd::utf16::state s0{};
				auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_invalid_input), ranges::span(output0, encode_output_max), s0, phd::default_text_error_handler{});
				REQUIRE(result0.error_code == phd::encoding_errc::ok);
				REQUIRE_FALSE(result0.error());
				REQUIRE(phd::u16c_string_view(output0) == phd::text::tests::u16_unicode_replacement_truth);
			}
			SECTION("char32_t") {
				phd::utf32 enc;

				char32_t output0[encode_output_max]{};
				phd::utf32::state s0{};
				auto result0 = enc.encode(phd::u32c_string_view(phd::text::tests::u32_unicode_invalid_input), ranges::span(output0, encode_output_max), s0, phd::default_text_error_handler{});
				REQUIRE(result0.error_code == phd::encoding_errc::ok);
				REQUIRE_FALSE(result0.error());
				REQUIRE(phd::u32c_string_view(output0) == phd::text::tests::u32_unicode_replacement_truth);
			}
		}
	}
}
