

#include <phd/out_ptr/inout_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("inout_ptr/failure/shared no deleter", "inout_ptr type will static assert the lack of a deleter (and the fact that it is a std::shared_ptr)") {
	SECTION("shared without deleter") {
		std::shared_ptr<void> p(nullptr, ficapi::deleter<>());
		ficapi_re_create(phd::out_ptr::inout_ptr(p), ficapi_type::ficapi_type_int);
		int* rawp = static_cast<int*>(p.get());
		REQUIRE(rawp != nullptr);
		REQUIRE(*rawp == ficapi_get_dynamic_data());
	}
}
