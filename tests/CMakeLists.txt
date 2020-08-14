# Dependencies
if (NOT TARGET Catch2::Catch2)
	add_subdirectory(../vendor/catch2 "vendor/Catch2")
endif()

# Tests
add_subdirectory(overload)
add_subdirectory(text)
add_subdirectory(handle)
