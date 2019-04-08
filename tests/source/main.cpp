#define CATCH_CONFIG_RUNNER 1
#include <catch2/catch.hpp>

#include <clocale>

int main(int argc, char* argv[]) {
	std::setlocale(LC_ALL, "");

	int r = Catch::Session().run(argc, argv);
	return r;
}
