# # barrier��
# The Kek License (KEK)

# Copyright � 2018 ThePhD.

# kek

set(barrier_sources
	"include/barrier/barrier.hpp"
	"source/barrier.cpp"
)

prepend(prefixed_barrier_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${barrier_sources})

add_library(barrier SHARED ${prefixed_barrier_sources})
if (MSVC)
	target_compile_options(barrier
		PRIVATE /std:c++latest)
else()
	target_compile_options(barrier
		PRIVATE -std=c++1z)
endif()
set_target_properties(barrier
	PROPERTIES 
	POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(barrier
	PRIVATE PHD_BARRIER_BUILD=1
	PUBLIC PHD_BARRIER_DLL=1)
target_include_directories(barrier
	PUBLIC "include")
target_link_libraries(barrier 
	PUBLIC ${CMAKE_DL_LIBS})
