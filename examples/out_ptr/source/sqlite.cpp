

// Thanks to David Sankel for coming up with this example

#include <phd/out_ptr.hpp>

#include <sqlite3.h>

#include <memory>
#include <iostream>

int main(int, char* []) {
	using bop = boost::out_ptr;

	std::shared_ptr<sqlite3> database;

	if (SQLITE_OK != sqlite3_open("test.db", bop::out_ptr(database, sqlite3_close))) {
		std::cout << "Problem opening the file";
	}

	using Sqlite3Ptr = std::unique_ptr<sqlite3, decltype(&sqlite3_close)>;
	Sqlite3Ptr database2(nullptr, &sqlite3_close);
	if (SQLITE_OK != sqlite3_open("test.db", bop::out_ptr(database2, sqlite3_close))) {
		std::cout << "Problem opening the file";
	}

	return 0;
}
