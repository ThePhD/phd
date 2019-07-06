#include <phd/text/text_view.hpp>

#include <catch2/catch.hpp>

TEST_CASE("text/text_view/basic", "basic usages of text_view do not explode") {
	SECTION("char") {
		phd::text_view t;
	}
	SECTION("wchar_t") {
		phd::wtext_view t;
	}
	SECTION("char8_t") {
		phd::u8text_view t;
	}
	SECTION("char16_t") {
		phd::u16text_view t;
	}
	SECTION("char32_t") {
		phd::u32text_view t;
	}
}
