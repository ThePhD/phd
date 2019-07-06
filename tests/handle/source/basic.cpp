#include <phd/handle/handle.hpp>

#include <catch2/catch.hpp>

#include <iostream>

struct my_stateful_int_deleter {
	int the_null_value;

	my_stateful_int_deleter(int stateful_null_value = 24)
	: the_null_value(stateful_null_value) {
	}

	void write_null(int& h) const {
		h = the_null_value;
	}

	bool is_null(const int& h) const {
		return h == the_null_value;
	}

	void operator()(const int& h) const {
		if (h < the_null_value) {
			INFO("Less than the null value on deletion");
		}
		else {
			INFO("More than or equal to the null value on deletion");
		}
	}
};

TEST_CASE("handle/basic", "handle has basic fancy pointer amenities") {
	phd::handle<int> my_handle(24);
	REQUIRE(my_handle != nullptr);
	int released_handle = my_handle.release();
	REQUIRE(released_handle == 24);
	REQUIRE(my_handle == nullptr);
}

TEST_CASE("handle/stateful", "handle works with stateful deleters") {
	phd::handle<int, my_stateful_int_deleter> my_handle(1, 25);
	REQUIRE(my_handle != nullptr);
	REQUIRE(my_handle.get() == 1);
	my_handle.reset(50);
	REQUIRE(my_handle.get() == 50);
}
