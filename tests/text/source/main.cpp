#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <clocale>

int main(int argc, char* argv[]) {
#ifdef _WIN32
	char* res = std::setlocale(LC_ALL, ".65001");
	if (res == nullptr) {
		std::cerr << "cannot set the locale in Windows to UTF8" << std::endl;
		return -1;
	}
#else
	char* res = std::setlocale(LC_ALL, "en_US.utf8");
	if (res == nullptr) {
		std::cerr << "cannot set the locale in non-Windows to UTF8" << std::endl;
		return -1;
	}
#endif

	int result = Catch::Session().run(argc, argv);
	return result;
}
