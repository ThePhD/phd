#include <phd/overload/overload.hpp>

#include <tests/classes.hpp>
#include <tests/functions.hpp>

#include <catch2/catch.hpp>

#include <memory>
#include <functional>

TEST_CASE("phd/overload/free functions", "overload can handle non-template free functions") {
	auto ovr = phd::overload(i_to_i, s_to_i);

	REQUIRE(ovr(2) == 0);
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

TEST_CASE("phd/overload/function_pointer_convertibles", "overload can handle types which are convertible to a callable") {
	auto ovr = phd::overload(
		my_convertible_class{},
		[](std::string) { return 2; });

	REQUIRE(ovr(2) == 0);
	REQUIRE(ovr("bark") == 2);
}

TEST_CASE("phd/overload/member functions", "overload can handle member functions") {
	SECTION("value self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::s_to_i);
		my_class v_obj{};

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("pointer self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::s_to_i);
		std::unique_ptr<my_class> ptr_obj = std::make_unique<my_class>();
		my_class* v_obj = ptr_obj.get();

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("fancy pointer self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::s_to_i);
		std::unique_ptr<my_class> v_obj = std::make_unique<my_class>();

		REQUIRE(ovr(v_obj, 2) == 0);
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
	SECTION("fancy pointer self") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::d,
			&my_class::s_to_i);
		std::unique_ptr<my_class> v_obj = std::make_unique<my_class>();

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
			&my_derived_class::s_to_i);
		my_derived_class v_obj{};

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("pointer self") {
		auto ovr = phd::overload(
			&my_derived_class::i_to_i,
			&my_derived_class::s_to_i);
		std::unique_ptr<my_derived_class> ptr_obj = std::make_unique<my_derived_class>();
		my_derived_class* v_obj = ptr_obj.get();

		REQUIRE(ovr(v_obj, 2) == 0);
		REQUIRE(ovr(v_obj, "bark") == 2);
	}
	SECTION("fancy pointer self") {
		auto ovr = phd::overload(
			&my_derived_class::i_to_i,
			&my_derived_class::s_to_i);
		std::unique_ptr<my_derived_class> ptr_obj = std::make_unique<my_derived_class>();

		REQUIRE(ovr(ptr_obj, 2) == 0);
		REQUIRE(ovr(ptr_obj, "bark") == 2);
	}
}

TEST_CASE("phd/overload/reference_wrapped callables", "overload can handle reference wrappers") {
	SECTION("reference_wrapper") {
		my_final_callable_i i;
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

TEST_CASE("phd/overload/reference_wrapped objects", "overload can handle reference wrappers as the objects, including non-std::reference_wrapper") {
	SECTION("member_functions") {
		auto ovr = phd::overload(
			&my_class::i_to_i,
			&my_class::s_to_i);
		my_class v_obj_value{};
		std::reference_wrapper<my_class> v_obj(v_obj_value);
		REQUIRE(ovr(v_obj, 2) == 0);
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
		my_reference_wrapper<my_class_mixed> v_obj1(v_obj1_value);

		REQUIRE(v_obj0.get().i == 0);
		REQUIRE(ovr(v_obj0) == 0);
		REQUIRE(ovr(v_obj0, 3.5) == 1.0);
		REQUIRE(ovr(v_obj0, 4) == 1.0);
		// does not set value: d_to_i is better,
		// less-ambiguous match
		REQUIRE(v_obj0.get().i == 0);

		REQUIRE(v_obj1.get_ref().s == "2");
		REQUIRE(ovr(v_obj1) == "2");
		REQUIRE(ovr(v_obj1, "5") == "5");
		REQUIRE(v_obj1.get_ref().s == "5");
	}
}

TEST_CASE("phd/overload/final callables", "overload can handle final objects") {
	SECTION("plain") {
		auto ovr = phd::overload(
			my_final_callable_i(),
			my_final_callable_s());

		REQUIRE(ovr(2) == 0);
		REQUIRE(ovr("bark") == 2);
	}
}

TEST_CASE("phd/overload/ambiguities string", "overload can handle ambiguities in string") {
	SECTION("const const, mutable object") {
		auto f1 = [](std::string_view) { return 0; };
		auto f2 = [](const std::string& s) { return 1; };
		auto ovr = phd::overload(f1, f2);

		struct better_overload_const_const {
			int operator()(std::string_view) const {
				return 0;
			}

			int operator()(const std::string& s) const {
				return 1;
			}
		} bovr;

		std::string_view sv = "";
		std::string s = "";
		const char* ptr = "";
		const auto& lit = "";

		// commented out are not solvable
		// all of these are ambiguous
		// VERDICT: unsolvable
		// requires language fix
		//REQUIRE(bovr("") == 0);
		//REQUIRE(bovr(lit) == 0);
		//REQUIRE(bovr(ptr) == 0);
		REQUIRE(bovr(sv) == 0);
		REQUIRE(bovr(s) == 1);

		//REQUIRE(ovr("") == 0);
		//REQUIRE(ovr(lit) == 0);
		//REQUIRE(ovr(ptr) == 0);
		REQUIRE(ovr(sv) == 0);
		REQUIRE(ovr(s) == 1);

		//REQUIRE(bovr("") == ovr(""));
		//REQUIRE(bovr(lit) == ovr(lit));
		//REQUIRE(bovr(ptr) == ovr(ptr));
		REQUIRE(bovr(sv) == ovr(sv));
		REQUIRE(bovr(s) == ovr(s));
	}
	SECTION("const const, const object") {
		auto f1 = [](std::string_view) { return 0; };
		auto f2 = [](const std::string& s) { return 1; };
		const auto ovr = phd::overload(f1, f2);

		struct better_overload_const_const {
			int operator()(std::string_view) const {
				return 0;
			}

			int operator()(const std::string& s) const {
				return 1;
			}
		} const bovr;

		std::string_view sv = "";
		std::string s = "";
		const char* ptr = "";
		const auto& lit = "";

		// commented out are not solvable
		// all of these are ambiguous
		// VERDICT: unsolvable
		// requires language fix
		//REQUIRE(bovr("") == 0);
		//REQUIRE(bovr(lit) == 0);
		//REQUIRE(bovr(ptr) == 0);
		REQUIRE(bovr(sv) == 0);
		REQUIRE(bovr(s) == 1);

		//REQUIRE(ovr("") == 0);
		//REQUIRE(ovr(lit) == 0);
		//REQUIRE(ovr(ptr) == 0);
		REQUIRE(ovr(sv) == 0);
		REQUIRE(ovr(s) == 1);

		//REQUIRE(bovr("") == ovr(""));
		//REQUIRE(bovr(lit) == ovr(lit));
		//REQUIRE(bovr(ptr) == ovr(ptr));
		REQUIRE(bovr(sv) == ovr(sv));
		REQUIRE(bovr(s) == ovr(s));
	}
	SECTION("const mutable, mutable object") {
		auto f1 = [](std::string_view) { return 0; };
		auto f2 = [](const std::string& s) mutable { return 1; };
		auto ovr = phd::overload(f1, f2);

		struct better_overload_const_mutable {
			int operator()(std::string_view) const {
				return 0;
			}

			int operator()(const std::string& s) {
				return 1;
			}
		} bovr;

		std::string_view sv = "";
		std::string s = "";
		const char* ptr = "";
		const auto& lit = "";

		// no longer ambiguous
		// VERDICT: Caveat Emptor
		REQUIRE(bovr("") == 1);
		REQUIRE(bovr(lit) == 1);
		REQUIRE(bovr(ptr) == 1);
		REQUIRE(bovr(sv) == 0);
		REQUIRE(bovr(s) == 1);

		REQUIRE(ovr("") == 1);
		REQUIRE(ovr(lit) == 1);
		REQUIRE(ovr(ptr) == 1);
		REQUIRE(ovr(sv) == 0);
		REQUIRE(ovr(s) == 1);

		REQUIRE(bovr("") == ovr(""));
		REQUIRE(bovr(lit) == ovr(lit));
		REQUIRE(bovr(ptr) == ovr(ptr));
		REQUIRE(bovr(sv) == ovr(sv));
		REQUIRE(bovr(s) == ovr(s));
	}

	SECTION("const mutable, const object") {
		auto f1 = [](std::string_view) { return 0; };
		auto f2 = [](const std::string& s) mutable { return 1; };
		const auto ovr = phd::overload(f1, f2);

		struct better_overload_const_mutable {
			int operator()(std::string_view) const {
				return 0;
			}

			int operator()(const std::string& s) {
				return 1;
			}
		} const bovr;

		std::string_view sv = "";
		std::string s = "";
		const char* ptr = "";
		const auto& lit = "";

		// no longer ambiguous
		// non-const overload not callable
		// VERDICT: Caveat Emptor
		REQUIRE(bovr("") == 0);
		REQUIRE(bovr(lit) == 0);
		REQUIRE(bovr(ptr) == 0);
		REQUIRE(bovr(sv) == 0);
		//REQUIRE(bovr(s) == 1);

		REQUIRE(ovr("") == 0);
		REQUIRE(ovr(lit) == 0);
		REQUIRE(ovr(ptr) == 0);
		REQUIRE(ovr(sv) == 0);
		//REQUIRE(ovr(s) == 1);

		REQUIRE(bovr("") == ovr(""));
		REQUIRE(bovr(lit) == ovr(lit));
		REQUIRE(bovr(ptr) == ovr(ptr));
		REQUIRE(bovr(sv) == ovr(sv));
		//REQUIRE(bovr(s) == ovr(s));
	}
}
