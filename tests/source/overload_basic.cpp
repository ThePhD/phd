#include <catch2/catch.hpp>

#include <phd/overload/overload.hpp>

#include <tests/classes.hpp>
#include <tests/functions.hpp>

#include <variant>
#include <memory>
#include <functional>

TEST_CASE("phd/overload/free functions", "overload can handle non-template free functions") {
	auto ovr = phd::overload(i_to_i, d_to_i, s_to_i);

	REQUIRE(ovr(2) == 0);
	REQUIRE(ovr(3.5) == 1);
	REQUIRE(ovr("bark") == 2);
}

TEST_CASE("phd/overload/lambdas", "overload can handle non-template lambdas") {
	SECTION("const") {
		auto ovr = phd::overload(
			[](int) { return 0; },
			[](double) { return 1; },
			[](std::string) { return 2; });

		REQUIRE(ovr(2) == 0);
		REQUIRE(ovr(3.5) == 1);
		REQUIRE(ovr("bark") == 2);
	}
	SECTION("mutable") {
		auto ovr = phd::overload(
			[](int) mutable { return 0; },
			[](double) mutable { return 1; },
			[](std::string) mutable { return 2; });

		REQUIRE(ovr(2) == 0);
		REQUIRE(ovr(3.5) == 1);
		REQUIRE(ovr("bark") == 2);
	}
	SECTION("mixed") {
		auto ovr = phd::overload(
			[](int) { return 0; },
			[](double) mutable { return 1; },
			[](std::string) { return 2; });

		REQUIRE(const_cast<const decltype(ovr)&>(ovr)(2) == 0);
		REQUIRE(ovr(3.5) == 1);
		REQUIRE(ovr("bark") == 2);
	}
}

TEST_CASE("phd/overload/member functions", "overload can handle member functions") {
	SECTION("value self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d_to_i,
			&my_class::s_to_i);
		my_class v_obj{};

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, 3.5) == 1);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("pointer self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d_to_i,
			&my_class::s_to_i);
		std::unique_ptr<my_class> ptr_obj = std::make_unique<my_class>();
		my_class* v_obj = ptr_obj.get();

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, 3.5) == 1);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("value self/mixed") {
		auto ovr = phd::overload(
			&my_class_mixed::i_to_i,
			&my_class_mixed::d_to_i,
			&my_class_mixed::s_to_i);
		my_class_mixed v_obj{};

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(const_cast<const my_class_mixed&>(v_obj), 3.5) == 1);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
}

TEST_CASE("phd/overload/member variables", "overload can handle member object pointers") {
	SECTION("value self") {
		auto ovr = phd::overload(
			&my_class::i,
			&my_class::d_to_i,
			&my_class::s_to_i);
		my_class v_obj{};

		REQUIRE(ovr(v_obj) == 0);
		REQUIRE(ovr(v_obj, 3.5) == 1);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("pointer self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d,
			&my_class::s_to_i);
		std::unique_ptr<my_class> ptr_obj = std::make_unique<my_class>();
		my_class* v_obj = ptr_obj.get();

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj) == 1.0);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("value self/2 types") {
		auto ovr = phd::overload(
			&my_class_mixed::i_to_i,
			&my_class_mixed::d,
			&my_class::s);
		my_class_mixed v_obj0{};
		my_class v_obj1{};

		REQUIRE(ovr(v_obj0, 2) == 0);
		REQUIRE(ovr(const_cast<const my_class_mixed&>(v_obj0)) == 1.0);
		REQUIRE(ovr(v_obj1) == "2");
	}
}

TEST_CASE("phd/overload/virtual member functions", "overload can handle virtual member functions") {
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
		my_derived_class* v_obj = ptr_obj.get();

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, 3.5) == 1);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
}

TEST_CASE("phd/overload/reference_wrapped callables", "overload can handle reference wrappers") {
	SECTION("member_functions") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d_to_i,
			&my_class::s_to_i);
		my_class v_obj_value{};
		std::reference_wrapper<my_class> v_obj(v_obj_value);
		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, 3.5) == 1);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("member_variables") {
		auto ovr = phd::overload(
			&my_class::i,
			&my_class::d_to_i,
			&my_class_mixed::s);
		my_class v_obj0_value{};
		my_class_mixed v_obj1_value{};
		std::reference_wrapper<my_class> v_obj0(v_obj0_value);
		std::reference_wrapper<my_class_mixed> v_obj1(v_obj1_value);
		REQUIRE(ovr(v_obj0) == 0);
		REQUIRE(ovr(v_obj0, 3.5) == 1.0);
		REQUIRE(ovr(v_obj1) == "2");
	}
}

TEST_CASE("phd/overload/final callables", "overload can handle final objects") {
	SECTION("plain") {
		auto ovr = phd::overload(
			my_final_callable_i(),
			my_final_callable_d(),
			my_final_callable_s());

		REQUIRE(ovr(2) == 0);
		REQUIRE(ovr(3.5) == 1);
		REQUIRE(ovr("bark") == 2);
	}
	SECTION("reference_wrapper") {
		my_final_callable_i i;
		//my_final_callable_d d;
		my_final_callable_s s;
		auto ovr = phd::overload(
			std::ref(i),
			//std::ref(d),
			std::ref(s));

		REQUIRE(ovr(2) == 0);
		//REQUIRE(ovr(3.5) == 1);
		REQUIRE(ovr("bark") == 2);
	}
}
