

#include <phd/out_ptr/out_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("out_ptr/failure/roundtrip/unique", "out_ptr should not allow round-tripping from void** to Pointer* and then void** if specific Pointer template arugment is used") {
	SECTION("unique_ptr<void>, ficapi::opaque_handle out_ptr") {
		std::unique_ptr<void, ficapi::deleter<ficapi_type::ficapi_type_opaque>> p(nullptr);
		ficapi_create(boost::out_ptr::out_ptr<ficapi::opaque_handle>(p), ficapi::type::ficapi_type_opaque);
		ficapi::opaque_handle rawp = static_cast<ficapi::opaque_handle>(p.get());
		REQUIRE(rawp != nullptr);
		REQUIRE(ficapi_handle_get_data(rawp) == ficapi_get_dynamic_data());
	}
}
