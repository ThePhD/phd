# overload

A utility for created an object type with an overloaded `operator()`, useful for `std::visit` calls and similar:

````c++
#include <variant>
#include <phd/overload/overload.hpp>

void f(int value) { /* work */ }
void g(const std::string& value) { /* work */ }

int main () {
	using my_variant = std::variant<
		int, 
		std::string, 
		std::vector<int>
	>;
	my_variant v1("bark");
	my_variant v2(2);

	auto h = [](const std::vector<int>& value) {
		/* work */
	};
	auto visitor = std::overload(f, g, h);

	// calls g
	std::visit(visitor, v1);
	// calls f
	std::visit(visitor, v2);
	// can be used on its own:
	// calls the lambda
	visitor(std::vector<int>{2, 4, 6, 8});

	return 0;
}
````