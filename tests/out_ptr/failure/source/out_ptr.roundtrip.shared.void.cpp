

#include <phd/out_ptr/out_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("out_ptr/failure/roundtrip/shared", "out_ptr should not allow round-tripping from void** to Pointer* and then void** if specific Pointer template arugment is used") {
	SECTION("shared_ptr<void>") {
		std::shared_ptr<void> p(nullptr);
		ficapi_create(boost::out_ptr::out_ptr<ficapi::opaque_handle>(p, ficapi::deleter<ficapi_type::ficapi_type_opaque>()), ficapi_type::ficapi_type_opaque);
		ficapi::opaque_handle rawp = static_cast<ficapi::opaque_handle>(p.get());
		REQUIRE(rawp != nullptr);
		REQUIRE(ficapi_handle_get_data(rawp) == ficapi_get_dynamic_data());
	}
}
