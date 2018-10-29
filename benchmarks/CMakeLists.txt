# Get Python interpreter for cache stuff
find_package(PythonInterp 3)

# Subdirectory Google Benchmark, after setting all cache variables
set(BENCHMARK_ENABLE_EXCEPTIONS ON CACHE INTERNAL "Enable the use of exceptions in the benchmark library." FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "Enable testing of the benchmark library." FORCE)
set(BENCHMARK_ENABLE_LTO OFF CACHE INTERNAL "Enable link time optimisation of the benchmark library." FORCE)
set(BENCHMARK_USE_LIBCXX OFF CACHE INTERNAL "Build and test using libc++ as the standard library." FORCE)
set(BENCHMARK_BUILD_32_BITS OFF CACHE INTERNAL "Build a 32 bit version of the library." FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE INTERNAL "Enable installation of benchmark. (Projects embedding benchmark may want to turn this OFF.)" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE INTERNAL "Allow the downloading and in-tree building of unmet dependencies" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE INTERNAL "Enable building the unit tests which depend on gtest" FORCE)
add_subdirectory("../vendor/benchmark" "vendor/benchmark")

add_subdirectory(out_ptr)
add_subdirectory(optional)
