set(phd_out_ptr_tests_sources
	"source/clever_out_ptr_test.cpp"
	"source/simple_out_ptr_test.cpp"
	"source/out_ptr_test.cpp"
	"source/raw_out_ptr_test.cpp"
	"source/customized_out_ptr_test.cpp"
	"source/custom_unique_deleter_test.cpp"

	"source/clever_inout_ptr_test.cpp"
	"source/simple_inout_ptr_test.cpp"
	"source/inout_ptr_test.cpp"
	"source/customized_inout_ptr_test.cpp"

	"source/out_ptr_tests_main.cpp"
)

prepend(prefixed_phd_out_ptr_tests_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd_out_ptr_tests_sources})

add_executable(phd_out_ptr_tests ${prefixed_phd_out_ptr_tests_sources})
if (MSVC)
	target_compile_options(phd_out_ptr_tests
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd_out_ptr_tests
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd_out_ptr_tests
	PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/vendor/catch2/single_include")
target_link_libraries(phd_out_ptr_tests 
	PRIVATE phd::phd ficapi ${CMAKE_DL_LIBS}
)
add_test(NAME phd_out_ptr_tests COMMAND phd_out_ptr_tests)
