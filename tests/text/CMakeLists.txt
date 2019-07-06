# Tests
set(phd.text.test_sources
	"source/decode.cpp"
	"source/encode.cpp"
	"source/transcode.cpp"
	"source/errors.cpp"
	"source/encoding.cpp"
	"source/text.cpp"
	"source/text_view.cpp"

	"source/main.cpp"
)

add_executable(phd.text.test ${phd.text.test_sources})
if (MSVC)
	target_compile_options(phd.text.test
		PRIVATE /std:c++latest /utf-8)
else()
	target_compile_options(phd.text.test
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd.text.test
	PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(phd.text.test 
	PRIVATE 
	phd::phd 	
	Catch2::Catch2
	${CMAKE_DL_LIBS}
)
add_test(NAME phd.text.test COMMAND phd.text.test)
