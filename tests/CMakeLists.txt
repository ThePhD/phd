# Dependencies
if (NOT TARGET Catch2::Catch2)
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2
	)
	FetchContent_MakeAvailable(Catch2)
endif()

# Tests
add_subdirectory(overload)
if (TARGET phd::out_ptr)
	add_subdirectory(handle)
endif()
