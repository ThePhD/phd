

#include <phd/out_ptr/out_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("out_ptr/fail/shared no deleter", "out_ptr type will static assert the lack of a deleter") {
	SECTION("shared without deleter") {
		std::shared_ptr<void> p(nullptr);
		ficapi_create(boost::out_ptr::out_ptr(p), ficapi_type::ficapi_type_int);
		int* rawp = static_cast<int*>(p.get());
		REQUIRE(rawp != nullptr);
		REQUIRE(*rawp == ficapi_get_dynamic_data());
	}
}
