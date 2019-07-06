

#include <phd/out_ptr/inout_ptr.hpp>

#include <ficapi/ficapi.hpp>

#include <catch2/catch.hpp>


TEST_CASE("inout_ptr/raw", "inout_ptr type works with smart pointers and C-style output APIs") {
	SECTION("void*") {
		void* p(nullptr);
		ficapi_re_create(boost::out_ptr::inout_ptr(p), ficapi_type::ficapi_type_int);
		int* rawp = static_cast<int*>(p);
		REQUIRE(rawp != nullptr);
		REQUIRE(*rawp == ficapi_get_dynamic_data());
		ficapi_delete(p, ficapi_type::ficapi_type_int);
	}
	SECTION("int*") {
		int* p(nullptr);
		ficapi_int_re_create(boost::out_ptr::inout_ptr(p));
		int* rawp = p;
		REQUIRE(rawp != nullptr);
		REQUIRE(*rawp == ficapi_get_dynamic_data());
		ficapi_int_delete(p);
	}
	SECTION("ficapi::opaque*") {
		ficapi::opaque* p(nullptr);
		ficapi_handle_re_create(boost::out_ptr::inout_ptr(p));
		ficapi::opaque_handle rawp = p;
		REQUIRE(rawp != nullptr);
		REQUIRE(ficapi_handle_get_data(rawp) == ficapi_get_dynamic_data());
		ficapi_handle_delete(p);
	}
	SECTION("ficapi::opaque*, void inout_ptr") {
		ficapi::opaque* p(nullptr);
		ficapi_re_create(boost::out_ptr::inout_ptr<void*>(p), ficapi_type::ficapi_type_opaque);
		ficapi::opaque_handle rawp = p;
		REQUIRE(rawp != nullptr);
		REQUIRE(ficapi_handle_get_data(rawp) == ficapi_get_dynamic_data());
		ficapi_delete(p, ficapi_type::ficapi_type_opaque);
	}
}
