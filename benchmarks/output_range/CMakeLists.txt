include(Common/core)

# Source files
file(GLOB phd.output_range.benchmark_sources LIST_DIRECTORIES FALSE "source/*")

add_executable(phd.output_range.benchmarks ${phd.output_range.benchmark_sources})
if (MSVC)
	target_compile_options(phd.output_range.benchmarks
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd.output_range.benchmarks
		PRIVATE -std=c++2a)
endif()
target_include_directories(phd.output_range.benchmarks PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(phd.output_range.benchmarks
	PRIVATE
	tl::optional
	benchmark
	barrier
	${CMAKE_DL_LIBS}
)

set(PHD_OUTPUT_RANGE_BENCHMARKS_REPETITIONS 50
	CACHE STRING "The number of times to re-rerun the benchmarks to gather additional data")
set(PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT json
	CACHE STRING "The output format of the data. Must be json")
string(TOLOWER ${PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT} PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT_LOWER)
set(PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT ${PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT_LOWER} 
	CACHE STRING "The output format of the data. Must be json" FORCE)

set(PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTDIR "${CMAKE_SOURCE_DIR}/benchmark_results/output_range")
set(PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE "${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTDIR}/output_range_benchmarks_data.${PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT}")
file(MAKE_DIRECTORY "${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTDIR}")

set(phd.output_range.benchmarks_categories
	iterator_iterator,iterator_sink,source_sink)
set(phd.output_range.benchmarks_category_names
	"Stepanov" "Iterator-Sink" "Source-Sink")

add_custom_target(phd.output_range.benchmarks_runner
	COMMAND phd.output_range.benchmarks
		"--benchmark_out=${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE}"
		"--benchmark_out_format=${PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT}"
		"--benchmark_repetitions=${PHD_OUTPUT_RANGE_BENCHMARKS_REPETITIONS}"
		"--benchmark_display_aggregates_only=true"
	DEPENDS phd.output_range.benchmarks
	BYPRODUCTS ${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE}
	COMMENT "Executing Benchmarks and writing to '${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE}'"
)

if (Python3_Interpreter_FOUND)
	add_custom_target(phd.output_range.benchmarks_graphs_generator
		COMMAND ${Python3_EXECUTABLE} "${CMAKE_SOURCE_DIR}/benchmarks/tools/generate_graphs.py"
			"--input=${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE}"
			"--input_format=${PHD_OUTPUT_RANGE_BENCHMARKS_FORMAT}"
			"--output_dir=${PHD_OUTPUT_RANGE_BENCHMARKS_GRAPH_OUTDIR}"
			"--categories=${phd.output_range.benchmarks_categories}"
			--category_names ${phd.output_range.benchmarks_category_names}
		DEPENDS phd.output_range.benchmarks "${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE}"
		COMMENT "Graphing '${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTFILE}' data to '${PHD_OUTPUT_RANGE_BENCHMARKS_RESULTS_OUTDIR}'"
	)
endif()
