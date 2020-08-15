set(phd.overload.test_sources
	"source/overload_basic.cpp"
	"source/overload_basic_variant.cpp"
	"source/main.cpp"

	"source/classes.cpp"
	"source/functions.cpp"
)

add_executable(phd.overload.test ${phd.overload.test_sources})
if (MSVC)
	target_compile_options(phd.overload.test
		PRIVATE /std:c++latest /source-charset:utf-8)
else()
	target_compile_options(phd.overload.test
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd.overload.test
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(phd.overload.test 
	PRIVATE
	phd::phd
	Catch2::Catch2
	${CMAKE_DL_LIBS}
)
add_test(NAME phd.overload.test COMMAND phd.overload.test)
