# # phd 


cmake_minimum_required(VERSION 3.12.0)

# # Project declaration
# informs about the project, gives a description, version and MOST IMPORTANTLY
# the languages the project is going to use. Required.
project(phd VERSION 1.0.0 DESCRIPTION "A library of useful utiliti- SNRK. Yeah, okay, it's just shit I put in here to serve whatever purpose. :D" LANGUAGES C CXX)

# # Modules
# Include modules useful to the project, whether locally made in our own cmake DIRECTORY
# our from the standard cmake libraries
# Add home-rolled modules path to front of module path list
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" "${CMAKE_MODULE_PATH}")

# # # Top-Level Directories
# Check if this is the top-level project or not
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(PHD_IS_TOP_LEVEL_PROJECT true)
else()
    set(PHD_IS_TOP_LEVEL_PROJECT false)
endif()

# Modify bad flags / change defaults if that is the case
if (PHD_IS_TOP_LEVEL_PROJECT)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CFG_INT_DIR}/lib")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CFG_INT_DIR}/bin")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x86/${CFG_INT_DIR}/bin")
	else()
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CFG_INT_DIR}/lib")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CFG_INT_DIR}/bin")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/x64/${CFG_INT_DIR}/bin")
	endif()

	if (MSVC)
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	endif()
endif()

# # Include standard modules
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(Common/core)

# # Options
option(PHD_CI "Whether or not we are in continguous integration mode" OFF)
option(PHD_TESTS "Enable build of tests" OFF)
option(PHD_EXAMPLES "Enable build of examples" OFF)
option(PHD_BENCHMARKS "Enable build of benchmarks" OFF)

# # Targets
set(phd_sources 
	"include/phd/phd.hpp"
)
prepend(prefixed_phd_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd_sources})

add_library(phd INTERFACE)
add_library(phd::phd ALIAS phd)
target_include_directories(phd INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
	$<INSTALL_INTERFACE:include>
)
target_include_directories(phd INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/range-v3/include>  
	$<INSTALL_INTERFACE:"">
)
if (MSVC)
	target_compile_options(phd INTERFACE 
		/std:c++latest
		/permissive-
	)
else()
	target_compile_options(phd INTERFACE 
		-std=c++2a
	)
endif()
target_sources(phd INTERFACE 
	$<BUILD_INTERFACE:${prefixed_phd_sources}>  
	$<INSTALL_INTERFACE:${phd_sources}>
)
set_target_properties(phd
	PROPERTIES
	EXPORT_NAME phd::phd
)

# # Benchmarks, Tests, Examples
if (PHD_TESTS OR PHD_BENCHMARKS)
	enable_testing()
	add_subdirectory(vendor/ficapi)
endif()
if (PHD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(PHD_TESTS)
if (PHD_BENCHMARKS)
	add_subdirectory(benchmarks)
endif(PHD_BENCHMARKS)
if (PHD_EXAMPLES)
	add_subdirectory(examples)
endif(PHD_EXAMPLES)

# # Config / Version packaging
# Version configurations
configure_package_config_file(
	cmake/phd-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/phd-config.cmake"
	INSTALL_DESTINATION lib/cmake/phd/phd
	NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/phd-config-version.cmake"
	COMPATIBILITY AnyNewerVersion)

export(TARGETS phd FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/phd-targets.cmake")

install(TARGETS phd
	EXPORT phd)

install(EXPORT phd
	FILE phd-targets.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/phd/phd")

install(DIRECTORY "include/phd/phd"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/phd-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/phd/phd-config-version.cmake"
DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/phd")
