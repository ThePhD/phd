include(Common/core)

set(phd.out_ptr.benchmarks_sources
	"include/benchmarks/statistics.hpp"
	"source/statistics.cpp"

	"source/shared_local_out_ptr.cpp"
	"source/shared_reset_out_ptr.cpp"

	"source/local_out_ptr.cpp"
	"source/reset_out_ptr.cpp"

	"source/local_inout_ptr.cpp"
	"source/reset_inout_ptr.cpp"

	"source/main.cpp"
)

prepend(phd.out_ptr.benchmarks_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd.out_ptr.benchmarks_sources})

add_executable(phd.out_ptr.benchmarks ${phd.out_ptr.benchmarks_sources})
target_include_directories(phd.out_ptr.benchmarks PRIVATE
	"include")
if (MSVC)
	target_compile_options(phd.out_ptr.benchmarks
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd.out_ptr.benchmarks
		PRIVATE -std=c++1z)
endif()
target_link_libraries(phd.out_ptr.benchmarks
	PRIVATE phd::phd ficapi benchmark ${CMAKE_DL_LIBS}
)

set(PHD_OUT_PTR_BENCHMARKS_REPETITIONS 250
	CACHE STRING "The number of times to re-rerun the benchmarks to gather additional data")
set(PHD_OUT_PTR_BENCHMARKS_FORMAT json 
	CACHE STRING "The output format of the data. Must be json or csv")
string(TOLOWER ${PHD_OUT_PTR_BENCHMARKS_FORMAT} PHD_OUT_PTR_BENCHMARKS_FORMAT_LOWER)
set(PHD_OUT_PTR_BENCHMARKS_FORMAT ${PHD_OUT_PTR_BENCHMARKS_FORMAT_LOWER} 
	CACHE STRING "The output format of the data. Must be json or csv" FORCE)

set(PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR "${CMAKE_SOURCE_DIR}/benchmark_results/out_ptr")
set(PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE "${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR}/out_ptr_benchmarks.${PHD_OUT_PTR_BENCHMARKS_FORMAT}")
set(PHD_OUT_PTR_BENCHMARKS_GRAPH_OUTDIR "${CMAKE_SOURCE_DIR}/benchmark_results/out_ptr")
file(MAKE_DIRECTORY "${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR}")

set(phd.out_ptr.benchmarks_categories shared_local_out_ptr shared_reset_out_ptr local_out_ptr reset_out_ptr local_inout_ptr reset_inout_ptr)

add_custom_command(
	OUTPUT "${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}"
	COMMAND phd.out_ptr.benchmarks 
		"--benchmark_out=${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}" 
		"--benchmark_out_format=${PHD_OUT_PTR_BENCHMARKS_FORMAT}" 
		"--benchmark_repetitions=${PHD_OUT_PTR_BENCHMARKS_REPETITIONS}"
	DEPENDS phd.out_ptr.benchmarks
	COMMENT "Executing Benchmarks and outputting to '${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}'"
)

if (Python3_Interpreter_FOUND)
	add_custom_target(phd.out_ptr.benchmarks_graphs_generator
		COMMAND ${Python3_EXECUTABLE} "${CMAKE_SOURCE_DIR}/benchmarks/tools/generate_graphs.py" 
			"--input=${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}"
			"--input_format=${PHD_OUT_PTR_BENCHMARKS_FORMAT}"
			"--output_dir=${PHD_OUT_PTR_BENCHMARKS_GRAPH_OUTDIR}"
			--categories ${phd.out_ptr.benchmarks_categories}
		DEPENDS phd.out_ptr.benchmarks "${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}"
		COMMENT "Graphing '${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}' data to '${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR}'"
	)
endif()
