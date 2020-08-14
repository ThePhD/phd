include(Common/core)

# Source files
set(phd.optional.benchmarks_sources
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

prepend(phd.optional.benchmarks_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd.optional.benchmarks_sources})

add_executable(phd.optional.benchmarks ${phd.optional.benchmarks_sources})
if (MSVC)
	target_compile_options(phd.optional.benchmarks
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd.optional.benchmarks
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd.optional.benchmarks PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(phd.optional.benchmarks
	PRIVATE 
	optional 
	benchmark 
	barrier 
	${CMAKE_DL_LIBS}
)

set(PHD_OPTIONAL_BENCHMARKS_REPETITIONS 250
	CACHE STRING "The number of times to re-rerun the benchmarks to gather additional data")
set(PHD_OPTIONAL_BENCHMARKS_FORMAT json 
	CACHE STRING "The output format of the data. Must be json")
string(TOLOWER ${PHD_OPTIONAL_BENCHMARKS_FORMAT} PHD_OPTIONAL_BENCHMARKS_FORMAT_LOWER)
set(PHD_OPTIONAL_BENCHMARKS_FORMAT ${PHD_OPTIONAL_BENCHMARKS_FORMAT_LOWER} 
	CACHE STRING "The output format of the data. Must be json" FORCE)

set(PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR "${CMAKE_SOURCE_DIR}/benchmark_results/optional")
set(PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE "${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR}/optional_benchmarks_data.${PHD_OPTIONAL_BENCHMARKS_FORMAT}")
file(MAKE_DIRECTORY "${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR}")

set(phd.optional.benchmarks_categories 
	int_3x_by,int_by,vector_by,vector_3x_by)
set(phd.optional.benchmarks_category_names 
	"int 3x" "int 1x" "vector 1x" "vector 3x")

add_custom_target(phd.optional.benchmarks_runner
	COMMAND phd.optional.benchmarks 
		"--benchmark_out=${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}" 
		"--benchmark_out_format=${PHD_OPTIONAL_BENCHMARKS_FORMAT}" 
		"--benchmark_repetitions=${PHD_OPTIONAL_BENCHMARKS_REPETITIONS}"
	DEPENDS phd.optional.benchmarks
	BYPRODUCTS ${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}
	COMMENT "Executing Benchmarks and outputting to '${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}'"
)

if (Python3_Interpreter_FOUND)
	add_custom_target(phd.optional.benchmarks_graphs_generator
		COMMAND ${Python3_EXECUTABLE} "${CMAKE_SOURCE_DIR}/benchmarks/tools/generate_graphs.py" 
			"--input=${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}"
			"--input_format=${PHD_OPTIONAL_BENCHMARKS_FORMAT}"
			"--output_dir=${PHD_OPTIONAL_BENCHMARKS_GRAPH_OUTDIR}"
			"--categories=${phd.optional.benchmarks_categories}"
			--category_names ${phd.optional.benchmarks_category_names}
		DEPENDS phd.optional.benchmarks "${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}"
		COMMENT "Graphing '${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTFILE}' data to '${PHD_OPTIONAL_BENCHMARKS_RESULTS_OUTDIR}'"
	)
endif()
