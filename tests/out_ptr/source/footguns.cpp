

#include <phd/out_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>

#include <phd/out_ptr/tests/c_api.hpp>

#if 0
TEST_CASE("out_ptr/footgun", "footguns") {
	SECTION("if") {
		using del = std::default_delete<int>;
		std::shared_ptr<int> meow(nullptr);
		bool value = false;
		if (ficapi_int_create_fail(phd::out_ptr::out_ptr(meow, del()), 0) == 0 && (value = meow == nullptr) && meow) {
			REQUIRE(false); // this may not happen
		}
		REQUIRE(meow != nullptr);
		REQUIRE_FALSE(value);
	}
	SECTION("if-initializer") {
		using del = std::default_delete<int>;
		std::shared_ptr<int> meow(nullptr);
		bool value = false;
		if (auto err = ficapi_int_create_fail(phd::out_ptr::out_ptr(meow, del()), 0); err == 0 && (value = meow == nullptr) && meow) {
			REQUIRE(false); // this may not happen
		}
		REQUIRE(meow != nullptr);
		REQUIRE_FALSE(value);
	}
}
#endif // weird footgun cases
