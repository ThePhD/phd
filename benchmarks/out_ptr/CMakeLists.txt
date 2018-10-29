include(Common/core)

set(phd_out_ptr_benchmarks_sources
	"include/benchmarks/benchmark_compute.hpp"
	"source/benchmark_compute.cpp"

	"source/out_ptr_benchmarks_local.cpp"
	"source/out_ptr_benchmarks_reset.cpp"

	"source/inout_ptr_benchmarks_local.cpp"
	"source/inout_ptr_benchmarks_reset.cpp"

	"source/out_ptr_benchmarks_main.cpp"
)

prepend(phd_out_ptr_benchmarks_sources "${CMAKE_CURRENT_SOURCE_DIR}/" ${phd_out_ptr_benchmarks_sources})

add_executable(phd_out_ptr_benchmarks ${phd_out_ptr_benchmarks_sources})
target_include_directories(phd_out_ptr_benchmarks PRIVATE
	"include")
if (MSVC)
	target_compile_options(phd_out_ptr_benchmarks
		PRIVATE /std:c++latest)
else()
	target_compile_options(phd_out_ptr_benchmarks
		PRIVATE -std=c++1z)
endif()
target_link_libraries(phd_out_ptr_benchmarks
	PRIVATE phd::phd ficapi benchmark ${CMAKE_DL_LIBS}
)

set(PHD_OUT_PTR_BENCHMARKS_REPETITIONS 250
	CACHE STRING "The number of times to re-rerun the benchmarks to gather additional data")
set(PHD_OUT_PTR_BENCHMARKS_FORMAT json 
	CACHE STRING "The output format of the data. Must be json or csv")
string(TOLOWER ${PHD_OUT_PTR_BENCHMARKS_FORMAT} PHD_OUT_PTR_BENCHMARKS_FORMAT_LOWER)
set(PHD_OUT_PTR_BENCHMARKS_FORMAT ${PHD_OUT_PTR_BENCHMARKS_FORMAT_LOWER} 
	CACHE STRING "The output format of the data. Must be json or csv" FORCE)

set(PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR "${CMAKE_SOURCE_DIR}/benchmark_results/")
set(PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE "${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR}/PHD_OUT_PTR_benchmarks.${PHD_OUT_PTR_BENCHMARKS_FORMAT}")
file(MAKE_DIRECTORY "${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR}")

set(phd_out_ptr_benchmarks_categories local_out_ptr,reset_out_ptr,local_inout_ptr,reset_inout_ptr)

add_custom_target(phd_out_ptr_benchmarks_runner
	COMMAND phd_out_ptr_benchmarks 
		"--benchmark_out=${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}" 
		"--benchmark_out_format=${PHD_OUT_PTR_BENCHMARKS_FORMAT}" 
		"--benchmark_repetitions=${PHD_OUT_PTR_BENCHMARKS_REPETITIONS}"
	DEPENDS phd_out_ptr_benchmarks
	BYPRODUCTS ${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}
	COMMENT "Executing Benchmarks and outputting to '${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}'"
)

if (PYTHONINTERP_FOUND)
	add_custom_target(phd_out_ptr_benchmarks_graphs_generator
		COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/benchmarks/tools/generate_graphs.py" 
			"--input=${PHD_OUT_PTR_BENCHMARKS_RESET_OUTFILE}"
			"--input_format=${PHD_OUT_PTR_BENCHMARKS_FORMAT}"
			"--output_dir=${PHD_OUT_PTR_BENCHMARKS_GRAPH_OUTDIR}"
			"--categories=${phd_out_ptr_benchmarks_categories}"
		DEPENDS phd_out_ptr_benchmarks phd_out_ptr_benchmarks_runner
		COMMENT "Graphing '${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTFILE}' data to '${PHD_OUT_PTR_BENCHMARKS_RESULTS_OUTDIR}'"
	)
endif()