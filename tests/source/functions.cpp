#include <phd/tests/functions.hpp>

int f(int) {
	return 0;
}
int f(double) {
	return 1;
}
int f(std::string) {
	return 2;
}

int i_to_i(int a) {
	return f(a);
}

int d_to_i(double a) {
	return f(a);
}

int s_to_i(std::string s) {
	return f(s);
}
