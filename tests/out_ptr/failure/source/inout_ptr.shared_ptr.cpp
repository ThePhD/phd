

#include <phd/out_ptr/inout_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("inout_ptr/failure/shared not allowed", "inout_ptr type will static assert that a std::shared_ptr is not allowed") {
	SECTION("shared with inout_ptr") {
		std::shared_ptr<void> p(nullptr, ficapi::deleter<>());
		ficapi_re_create(boost::out_ptr::inout_ptr(p, ficapi::deleter<>()), ficapi_type::ficapi_type_int);
		int* rawp = static_cast<int*>(p.get());
		REQUIRE(rawp != nullptr);
		REQUIRE(*rawp == ficapi_get_dynamic_data());
	}
}
