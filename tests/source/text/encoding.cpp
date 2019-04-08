#include <phd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <range/v3/span.hpp>

#include <iostream>

TEST_CASE("text/encoding/core", "basic usages of encoding do not explode") {
	constexpr const char32_t u32_unicode_truth[] = U"♥";
	constexpr const char16_t u16_unicode_truth[] = u"♥";
	constexpr const char8_t u8_unicode_truth[] = u8"♥";
	constexpr const wchar_t w_unicode_truth[] = L"♥";
	constexpr const char unicode_truth[] = "\u2665";

	constexpr const char32_t u32_ansi_truth[] = U"á";
	constexpr const char16_t u16_ansi_truth[] = u"á";
	constexpr const char8_t u8_ansi_truth[] = u8"á";
	constexpr const wchar_t w_ansi_truth[] = L"á";
	constexpr const char ansi_truth[] = "á";

	SECTION("decode") {
		SECTION("char") {
			phd::execution enc;

			const auto& input = ansi_truth;
			phd::unicode_code_point output[2]{};
			phd::execution::state s{};
			auto dres = enc.decode(phd::c_string_view(input), ranges::span(output, 1), s, phd::default_text_error_handler{});
			REQUIRE(dres.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(dres.error());
			REQUIRE(phd::u32c_string_view(output) == u32_ansi_truth);
		}
		SECTION("wchar_t") {
			phd::wide_execution enc;

			const auto& input = w_ansi_truth;
			phd::unicode_code_point output[2]{};
			phd::wide_execution::state s{};
			auto dres = enc.decode(phd::wc_string_view(input), ranges::span(output, 1), s, phd::default_text_error_handler{});
			REQUIRE(dres.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(dres.error());
			REQUIRE(phd::u32c_string_view(output) == u32_ansi_truth);
		}
		SECTION("char8_t") {
			phd::utf8 enc;

			const auto& input = u8_unicode_truth;
			phd::unicode_code_point output[2]{};
			phd::utf8::state s{};
			auto dres = enc.decode(phd::u8c_string_view(input), ranges::span(output, 1), s, phd::default_text_error_handler{});
			REQUIRE(dres.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(dres.error());
			REQUIRE(phd::u32c_string_view(output) == u32_unicode_truth);
		}
		SECTION("char16_t") {
		}
		SECTION("char32_t") {
		}
	}
	SECTION("encode") {
		SECTION("char") {
			phd::execution enc;

			char output[8]{};
			phd::execution::state s{};
			auto dres = enc.encode(phd::u32c_string_view(u32_ansi_truth), ranges::span(output, 7), s, phd::default_text_error_handler{});
			REQUIRE(dres.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(dres.error());
			REQUIRE(phd::c_string_view(output) == ansi_truth);
		}
		SECTION("wchar_t") {
			phd::wide_execution enc;

			wchar_t output[8]{};
			phd::wide_execution::state s{};
			auto dres = enc.encode(phd::u32c_string_view(u32_ansi_truth), ranges::span(output, 7), s, phd::default_text_error_handler{});
			REQUIRE(dres.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(dres.error());
			REQUIRE(phd::wc_string_view(output) == w_ansi_truth);
		}
		SECTION("char8_t") {
			phd::utf8 enc;

			char8_t output[4]{};
			phd::utf8::state s{};
			auto dres = enc.encode(phd::u32c_string_view(u32_unicode_truth), ranges::span(output, 3), s, phd::default_text_error_handler{});
			REQUIRE(dres.error_code == phd::encoding_errc::ok);
			REQUIRE_FALSE(dres.error());
			REQUIRE(phd::u8c_string_view(output) == u8_unicode_truth);
		}
		SECTION("char16_t") {
		}
		SECTION("char32_t") {
		}
	}
	SECTION("transcode") {
	}
}
