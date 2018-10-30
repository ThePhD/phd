#include <tests/classes.hpp>
#include <tests/functions.hpp>

my_convertible_class::operator i_to_i_f_ptr() const {
	return i_to_i;
}

int my_class::i_to_i(int) {
	return 0;
}

int my_class::d_to_i(double) {
	return 1;
}

int my_class::s_to_i(std::string) {
	return 2;
}

int my_class_mixed::i_to_i(int) {
	return 0;
}

int my_class_mixed::d_to_i(double) const {
	return 1;
}

int my_class_mixed::s_to_i(std::string) {
	return 2;
}

int my_class_mixed_o::f(int) {
	return 0;
}

int my_class_mixed_o::f(double) const {
	return 1;
}

int my_class_mixed_o::f(std::string) {
	return 2;
}

int my_derived_class::i_to_i(int) {
	return 0;
}

int my_derived_class::d_to_i(double) {
	return 1;
}

int my_derived_class::s_to_i(std::string) {
	return 2;
}

int my_final_callable_i::operator()(int) {
	return 0;
}

int my_final_callable_d::operator()(double) {
	return 1;
}

int my_final_callable_s::operator()(std::string) const {
	return 2;
}
