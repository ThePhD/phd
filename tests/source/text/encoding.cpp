#include <phd/text/encoding.hpp>

#include <catch2/catch.hpp>

#include <range/v3/span.hpp>

TEST_CASE("text/encoding/core", "basic usages of encoding do not explode") {
	SECTION("char") {
	}
	SECTION("wchar_t") {
	}
	SECTION("char8_t") {
		phd::utf8 u8enc;

		const char8_t input[] = u8"♥";
		phd::unicode_code_point output[2]{};
		phd::utf8::state s{};
		auto dres = u8enc.decode(phd::u8c_string_view(input), ranges::span(output, 1), s, phd::default_error_handler{});
		REQUIRE(dres.error_code == phd::encoding_errc::ok);
		REQUIRE_FALSE(dres.error());
		REQUIRE(phd::u32c_string_view(output) == U"♥");
	}
	SECTION("char16_t") {
	}
	SECTION("char32_t") {
	}
}
