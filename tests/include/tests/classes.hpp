#pragma once

#ifndef PHD_TESTS_CLASSES_HPP
#define PHD_TESTS_CLASSES_HPP

#include <string>
#include <memory>

template <typename T>
struct my_reference_wrapper {
private:
	T* p;

public:
	my_reference_wrapper(T& r)
	: p(std::addressof(r)) {
	}

	operator T&() const {
		return *p;
	}
};

struct my_class {
	int i = 0;
	const double d = 1.0;
	std::string s = "2";

	int i_to_i(int);

	int d_to_i(double);

	int s_to_i(std::string);
};

struct my_class_mixed {
	int i = 0;
	const double d = 1.0;
	std::string s = "2";

	int i_to_i(int);

	int d_to_i(double) const;

	int s_to_i(std::string);
};

struct my_class_mixed_o {
	int i = 0;
	const double d = 1.0;
	std::string s = "2";

	int f(int);
	int f(double) const;
	int f(std::string);
};

struct my_base_class_i {
	virtual int i_to_i(int) = 0;
};

struct my_base_class_d {
	virtual int d_to_i(double) = 0;
};

struct my_base_class_s {
	virtual int s_to_i(std::string) = 0;
};

struct my_derived_class : my_base_class_i, my_base_class_d, my_base_class_s {
	virtual int i_to_i(int) override;

	virtual int d_to_i(double) override;

	virtual int s_to_i(std::string) override;
};

struct my_final_callable_i final {
	int operator()(int) {
		return 0;
	}
};

struct my_final_callable_d final {
	int operator()(double) {
		return 1;
	}
};

struct my_final_callable_s final {
	int operator()(std::string) const {
		return 2;
	}
};

#endif // PHD_TESTS_CLASSES_HPP
