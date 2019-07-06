

#include <phd/out_ptr/inout_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("inout_ptr/failure/roundtrip/void", "inout_ptr type should not roundtrip void** -> Pointer* -> void**") {
	SECTION("unique_ptr<void>, ficapi::opaque_handle inout_ptr") {
		std::unique_ptr<void, ficapi::deleter<ficapi_type::ficapi_type_opaque>> p(nullptr);
		ficapi_re_create(boost::out_ptr::inout_ptr<ficapi::opaque_handle>(p), ficapi::type::ficapi_type_opaque);
		ficapi::opaque_handle rawp = static_cast<ficapi::opaque_handle>(p.get());
		REQUIRE(rawp != nullptr);
		REQUIRE(ficapi_handle_get_data(rawp) == ficapi_get_dynamic_data());
	}
}