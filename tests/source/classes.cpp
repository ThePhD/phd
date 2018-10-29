#include <tests/classes.hpp>

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
