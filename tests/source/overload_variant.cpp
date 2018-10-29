#include <catch2/catch.hpp>

#include <phd/overload/overload.hpp>

#include <tests/classes.hpp>
#include <tests/functions.hpp>

#include <variant>
#include <memory>

TEST_CASE("phd/overload/free functions", "overload can handle non-template free functions with a variant") {
	auto ovr = phd::overload(i_to_i, d_to_i, s_to_i);
	std::variant<int, double, std::string> v0(2);
	std::variant<int, double, std::string> v1(3.5);
	std::variant<int, double, std::string> v2("bark");

	int v0_select = std::visit(ovr, v0);
	REQUIRE(v0_select == 0);

	int v1_select = std::visit(ovr, v1);
	REQUIRE(v1_select == 1);

	int v2_select = std::visit(ovr, v2);
	REQUIRE(v2_select == 2);
}

TEST_CASE("phd/overload/lambdas", "overload can handle non-template lambdas with a variant") {
	SECTION("const") {
		auto ovr = phd::overload(
			[](int) { return 0; },
			[](double) { return 1; },
			[](std::string) { return 2; });
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v2);
		REQUIRE(v2_select == 2);
	}
	SECTION("mutable") {
		auto ovr = phd::overload(
			[](int) mutable { return 0; },
			[](double) mutable { return 1; },
			[](std::string) mutable { return 2; });
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v2);
		REQUIRE(v2_select == 2);
	}
	SECTION("mixed") {
		// Cannot control conversions here
		/*
		auto ovr = phd::overload(
			[](int) { return 0; },
			[](double) mutable { return 1; },
			[](std::string) { return 2; });
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v2);
		REQUIRE(v2_select == 2);
		*/
	}
}

TEST_CASE("phd/overload/member functions", "overload can handle member functions with a variant") {
	SECTION("value self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d_to_i,
			&my_class::s_to_i);
		std::variant<my_class> v_obj(my_class{});
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v_obj, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v_obj, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v_obj, v2);
		REQUIRE(v2_select == 2);
	}
	SECTION("pointer self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d_to_i,
			&my_class::s_to_i);
		std::unique_ptr<my_class> ptr_obj = std::make_unique<my_class>();
		std::variant<my_class*> v_obj(ptr_obj.get());
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v_obj, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v_obj, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v_obj, v2);
		REQUIRE(v2_select == 2);
	}
	SECTION("value self/mixed") {
		// cannot convert function call
		/*
		auto ovr = phd::overload(
			&my_class_mixed::i_to_i,
			&my_class_mixed::d_to_i,
			&my_class_mixed::s_to_i);
		std::variant<my_class_mixed> v_obj(my_class_mixed{});
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v_obj, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v_obj, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v_obj, v2);
		REQUIRE(v2_select == 2);
		*/
	}
}

TEST_CASE("phd/overload/virtual member functions", "overload can handle virtual member functions with a variant") {
	SECTION("value self") {
		auto ovr = phd::overload(
			&my_derived_class::i_to_i,
			&my_derived_class::d_to_i,
			&my_derived_class::s_to_i);
		std::variant<my_derived_class> v_obj(my_derived_class{});
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v_obj, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v_obj, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v_obj, v2);
		REQUIRE(v2_select == 2);
	}
	SECTION("pointer self") {
		auto ovr = phd::overload(
			&my_derived_class::i_to_i,
			&my_derived_class::d_to_i,
			&my_derived_class::s_to_i);
		std::unique_ptr<my_derived_class> ptr_obj = std::make_unique<my_derived_class>();
		std::variant<my_derived_class*> v_obj(ptr_obj.get());
		std::variant<int, double, std::string> v0(2);
		std::variant<int, double, std::string> v1(3.5);
		std::variant<int, double, std::string> v2("bark");

		int v0_select = std::visit(ovr, v_obj, v0);
		REQUIRE(v0_select == 0);

		int v1_select = std::visit(ovr, v_obj, v1);
		REQUIRE(v1_select == 1);

		int v2_select = std::visit(ovr, v_obj, v2);
		REQUIRE(v2_select == 2);
	}
}
