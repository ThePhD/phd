include(Common/core)

# Get Python interpreter for cache stuff
find_package(PythonInterp 3)

# Subdirectory TL's optional
set(OPTIONAL_ENABLE_TESTS OFF CACHE INTERNAL "Enable tests." FORCE)
set(OPTIONAL_ENABLE_DOCS OFF CACHE INTERNAL "Enable documentation." FORCE)
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/optional" "vendor/optional")

# Subdirectory barrier
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/barrier" "vendor/barrier")

# Source files
set(phd_optional_benchmarks_sources
	"source/references.cpp"
	"source/values.cpp"
	"source/references.transparent.cpp"
	"source/values.transparent.cpp"
	"source/references.failure.cpp"
	"source/values.failure.cpp"
	"source/references.transparent.failure.cpp"
	"source/values.transparent.failure.cpp"
	"source/noop.cpp"

	"source/stats.cpp"
	"source/data.cpp"

	"source/main.cpp"
)

prepend(phd_optional_benchmarks_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd_optional_benchmarks_sources})

add_executable(phd_optional_benchmarks ${phd_optional_benchmarks_sources})
if (MSVC)
	target_compile_options(phd_optional_benchmarks
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd_optional_benchmarks
		PRIVATE -std=c++1z)
endif()
target_include_directories(phd_optional_benchmarks PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(phd_optional_benchmarks
	PRIVATE optional benchmark barrier ${CMAKE_DL_LIBS}
)

set(PHD_OPTIONAL_BENCHMARKS_REPETITIONS 250
	CACHE STRING "The number of times to re-rerun the benchmarks to gather additional data")
set(PHD_OPTIONAL_BENCHMARKS_FORMAT json 
	CACHE STRING "The output format of the data. Must be json or csv")
string(TOLOWER ${PHD_OPTIONAL_BENCHMARKS_FORMAT} PHD_OPTIONAL_BENCHMARKS_FORMAT_LOWER)
set(PHD_OPTIONAL_BENCHMARKS_FORMAT ${PHD_OPTIONAL_BENCHMARKS_FORMAT_LOWER} 
	CACHE STRING "The output format of the data. Must be json or csv" FORCE)

set(PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR "${CMAKE_SOURCE_DIR}/benchmark_results/optional/")
set(PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE "${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR}/optional_benchmarks.${PHD_OPTIONAL_BENCHMARKS_FORMAT}")
file(MAKE_DIRECTORY "${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR}")

set(phd_optional_benchmarks_categories 
	int_3x_by,int_by,vector_by,vector_3x_by)
set(phd_optional_benchmarks_category_names 
	"int 3x,int 1x,vector 1x,vector 3x")

add_custom_target(phd_optional_benchmarks_runner
	COMMAND phd_optional_benchmarks 
		"--benchmark_out=${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}" 
		"--benchmark_out_format=${PHD_OPTIONAL_BENCHMARKS_FORMAT}" 
		"--benchmark_repetitions=${PHD_OPTIONAL_BENCHMARKS_REPETITIONS}"
	DEPENDS phd_optional_benchmarks
	BYPRODUCTS ${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}
	COMMENT "Executing Benchmarks and outputting to '${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}'"
)

if (PYTHONINTERP_FOUND)
	add_custom_target(phd_optional_benchmarks_graphs_generator
		COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/benchmarks/tools/generate_graphs.py" 
			"--input=${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}"
			"--input_format=${PHD_OPTIONAL_BENCHMARKS_FORMAT}"
			"--output_dir=${PHD_OPTIONAL_BENCHMARKS_GRAPH_OUTDIR}"
			"--categories=${phd_optional_benchmarks_categories}"
			"--category_names=${phd_optional_benchmarks_category_names}"
		COMMENT "Graphing '${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}' data to '${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR}'"
	)
endif()
