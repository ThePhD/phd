
set(phd.handle.test_sources
	"source/basic.cpp"
	"source/main.cpp"
)

add_executable(phd.handle.test ${phd.handle.test_sources})
if (MSVC)
	target_compile_options(phd.handle.test
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd.handle.test
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd.handle.test
	PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(phd.handle.test 
	PRIVATE 
	phd::phd	
	Catch2::Catch2 
	${CMAKE_DL_LIBS}
)
add_test(NAME phd.handle.test COMMAND phd.handle.test)
