# Need pthreads
find_package(Threads REQUIRED)

# # Examples
set(phd.out_ptr.example_sources
	"customization.handle.cpp"
	"customization.traits.handle.cpp"
	"std.custom_unique_ptr.cpp"
	"std.shared_ptr.cpp"
	"std.unique_ptr.cpp"
)

foreach (example_source_name ${phd.out_ptr.example_sources})
	set(example_source_file source/${example_source_name})
	set(example_target phd.out_ptr.${example_source_name})
	add_executable(${example_target} ${example_source_file})
	set_target_properties(${example_target} PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED TRUE
		CXX_EXTENSIONS FALSE
	)
	if (MSVC)
		target_compile_options(${example_target}
			PRIVATE /W4)
	else()
		target_compile_options(${example_target}
			PRIVATE -Wpedantic -Wall -Werror)
	endif()
	target_compile_definitions(${example_target} PRIVATE __STDC_WANT_LIB_EXT1__=1)
	target_link_libraries(${example_target}
		PRIVATE 
		phd::phd
		Threads::Threads
		ficapi
		${CMAKE_DL_LIBS}
	)
	target_include_directories(${example_target} PRIVATE
		"../vendor/handle/include"
		"include"
	)
	if (PHD_OUT_PTR_TESTS)
		add_test(NAME ${example_target} COMMAND ${example_target})
	endif()
endforeach()

set(phd.out_ptr.atl_dir "${CMAKE_BINARY_DIR}/phd.out_ptr.test_atl_include")
set(phd.out_ptr.atl_bin_dir "${phd.out_ptr.atl_dir}/bin")
set(phd.out_ptr.atl_file "${phd.out_ptr.atl_dir}/atl_test.cpp")
file(MAKE_DIRECTORY ${phd.out_ptr.atl_dir})
file(MAKE_DIRECTORY ${phd.out_ptr.atl_bin_dir})
file(WRITE ${phd.out_ptr.atl_file}
"#include <atlbase.h>
int main (int, char*[]) { return 0; }")
try_run(phd.out_ptr.examples_has_atl_run phd.out_ptr.examples_has_atl_compile 
	${phd.out_ptr.atl_bin_dir} ${phd.out_ptr.atl_file}
	CMAKE_FLAGS "CMAKE_CXX_FLAGS:${CMAKE_CXX_FLAGS}" "CMAKE_C_FLAGS:${CMAKE_C_FLAGS}"
	"CMAKE_C_COMPILER:${CMAKE_C_COMPILER}" "CMAKE_CXX_COMPILER:${CMAKE_CXX_COMPILER}"
)
if (phd.out_ptr.examples_has_atl_run)
	set(example_source_name "com.intrusive_ptr.cpp")
	set(example_source_file source/${example_source_name})
	set(example_target phd.out_ptr.${example_source_name})
	add_executable(${example_target} ${example_source_file})
	set_target_properties(${example_target} PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED TRUE
		CXX_EXTENSIONS FALSE
	)
	if (MSVC)
		target_compile_options(${example_target}
			PRIVATE /W4)
	else()
		target_compile_options(${example_target}
			PRIVATE -Wpedantic -Wall -Werror)
	endif()
	target_compile_definitions(${example_target} PRIVATE __STDC_WANT_LIB_EXT1__=1)
	target_link_libraries(${example_target}
		PRIVATE 
		
		Threads::Threads
		${CMAKE_DL_LIBS}
	)
	target_include_directories(${example_target} PRIVATE
		"include"
	)
	if (PHD_OUT_PTR_TESTS)
		add_test(NAME ${example_target} COMMAND ${example_target})
	endif()
endif()

if (CMAKE_USE_PTHREADS_INIT)
	set(example_source_name "pthread.cpp")
	set(example_source_file source/${example_source_name})
	set(example_target phd.out_ptr.${example_source_name})
	add_executable(${example_target} ${example_source_file})
	set_target_properties(${example_target} PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED TRUE
		CXX_EXTENSIONS FALSE
	)
	if (MSVC)
		target_compile_options(${example_target}
			PRIVATE /W4)
	else()
		target_compile_options(${example_target}
			PRIVATE -Wpedantic -Wall -Werror)
	endif()
	target_compile_definitions(${example_target} PRIVATE __STDC_WANT_LIB_EXT1__=1)
	target_link_libraries(${example_target}
		PRIVATE 
		Threads::Threads
		${CMAKE_DL_LIBS}
	)
	target_include_directories(${example_target} PRIVATE
		"include"
	)	
	if (PHD_OUT_PTR_TESTS)
		add_test(NAME ${example_target} COMMAND ${example_target})
	endif()
endif()
