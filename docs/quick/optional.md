# optional benchmarks

This was for figuring out if references propagated through `optional` had the same performance characteristics at just using values and "letting the compiler optimize it all away".

It turns out, the compiler can't! Run the benchmarks from the CMake Target, or see [this blog post](https://thephd.github.io/2018/10/25/Big-Papers-Optional.html) for most details.