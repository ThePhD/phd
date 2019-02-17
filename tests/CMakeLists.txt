
add_subdirectory(out_ptr)

set(phd_test_sources
	"source/handle/handle_test.cpp"

	"source/overload/overload_basic.cpp"
	"source/overload/overload_basic_variant.cpp"

	"source/text/text.cpp"
	"source/text/text_view.cpp"

	"source/classes.cpp"
	"source/functions.cpp"

	"source/main.cpp"
)

prepend(prefixed_phd_test_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd_test_sources})

add_executable(phd_test ${prefixed_phd_test_sources})
if (MSVC)
	target_compile_options(phd_test
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd_test
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd_test
	PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/vendor/catch2/single_include")
target_link_libraries(phd_test 
	PRIVATE phd::phd ficapi ${CMAKE_DL_LIBS}
)
add_test(NAME phd_test COMMAND phd_test)
