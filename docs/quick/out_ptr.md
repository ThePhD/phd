# out_ptr

ᴮᵉᶜᵃᵘˢᵉ ˢᵒᵐᵉᵇᵒᵈʸ ᵗᵒᶫᵈ ᵐᵉ ᶦᵗ ᵐᶦᵍʰᵗ ᵇᵉ ᶠᵃˢᵗᵉʳ ᵗʰᵃᶰ ᵗʰᵉ ʳᵉᵍᵘᶫᵃʳ ᶦᵐᵖᶫᵉᵐᵉᶰᵗᵃᵗᶦᵒᶰ ˢᵒ ᶦᵗˢ ᵗᶦᵐᵉ ᵗᵒ ʰᵒᵖ ᵒᶰ ᵗʰᵃᵗ ᵖᵉʳᶠᵒʳᵐᵃᶰᶜᵉ ᵇᵘˢ 🚌 ᵇᵉᵉᵖ ᵇᵉᵉᵉᵉᵉᵖ⋅

This library is for allowing a person to use a C-style out-pointer API with pointers such as `std::unique_ptr`, `std::shared_ptr`, `boost::shared_ptr` and the upcoming [`std::retain_ptr`](https://wg21.link/p0468). It is meant to simplify the handling of these resources with these APIs and make it easier to handle these cases.

It is also for testing whether or not an abstraction like this can be as fast as the hand-written C-code that stuffs raw-pointer out results back into their C++ counterparts after performing a `release` call or a `release`/`reset` call pair.

## Build

There is no build to just use the library! This is a header-only, linker-hassle-free™ library.

If you want to build the tests or benchmarks, pass `-DOUT_PTR_BENCHMARKS=ON` and `-DOUT_PTR_TESTS=ON` to the cmake build system. You can import the `phd::out_ptr` target from CMake, as it is exported and configured from CMake as well.

## Benchmarks

`out_ptr benchmarks` for testing how efficient the library may or may not be by employing some clever tricks. You can run these benchmarks and report the results in an issue request, e-mail, or anywhere else you can find and emssage me. I would certainly appreciate any tips, tricks, pointers and other tidbits about how to write the benchmarks or if there are other cases I am not considering.

The benchmarks are written against a fictional C API (`ficapi`), and use a non-allocating version of the fictional C API to do the benchmarks. Benchmarks are done using [Google Benchmark](https://github.com/google/benchmark). Graphs will come soon once I go to the python meetup and waste an inordinate amount of time making pretty, pretty graphs.

You can run the benchmarks by building with `cmake  -DOUT_PTR_BENCHMARKS=ON path/to/out_ptr/directory`, executing `cmake --build .`, then using `<chosen-build-system-name> run_benchmarks` (where the chosen build system can be `make`  or `ninja`  or similar). You can also run the resulting `out_ptr_benchmarks` executable with your own settings if you like! The canonical runs and benchmark numbers and pretty graphs are kept [here](../../benchmark_results).

![Locally-defined out_ptr](../../benchmark_results/local%20out%20ptr.png)

![Pointer defined outside scope and constantly reset with out_ptr](benchmark_results/reset%20out%20ptr.png)

![Locally-defined with inout_ptr](../../benchmark_results/local%20inout%20ptr.png)

![Pointer defined outside scope and constantly reset with inout_ptr](../../benchmark_results/reset%20inout%20ptr.png)

The results of the benchmarks seem to suggest that the clever version of the `out_ptr` abstraction is actually faster than the simple implementation, and competitive with the `c_code` implementation. The `ficapi` code is compiled as a DLL so that the compiler cannot see past it or perform Link-Time Optimization/Code Generation, leaving all benchmarks to incur the same consistent cost for calling the creation and deletion code.

Peculiarly, some of the `reset` cases in the benchmarks have the `clever_out_ptr` going somewhat noticeably faster than the other versions. My personal belief is that this is environment noise, but I need to look at the disassembly to be sure.

The canonical benchmarks were run on an Intel® Core™ i7 CPU 860 @ 2.80 GHz with 12.0 GB of RAM on a 64-bit configured machine with Visual C++ (version MSVC 19.13.26129.0), but you likely don't need even an eighth of that to run these benchmarks (I do encourage people to check out and test the benchmarks!! More numbers are better.). Code is implemented for a C++17 world, but it could probably be easily adapted to C++11 (heck, it might even work with it right now, not that I'm checking closely!).
