#include <phd/text/text.hpp>

#include <catch2/catch.hpp>

TEST_CASE("text/text/basic", "basic usages of text do not explode") {
	SECTION("char") {
		phd::text t;
	}
	SECTION("wchar_t") {
		phd::wtext t;
	}
	SECTION("char8_t") {
		phd::u8text t;
	}
	SECTION("char16_t") {
		phd::u16text t;
	}
	SECTION("char32_t") {
		phd::u32text t;
	}
}
