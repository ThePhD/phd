// Copyright � 2018-2020 ThePhD

#pragma once

#ifndef PHD_OUTPUT_RANGE_BENCHMARKS_SINK_COPY_HPP
#define PHD_OUTPUT_RANGE_BENCHMARKS_SINK_COPY_HPP

template<typename It, typename End, typename Sink>
Sink iterator_sink_copy(It it, End end, Sink sink) {
    while (it != end) {
        sink(*it);
	   ++it;
    }
    return sink;
}

template<typename Source, typename Sink>
Sink source_sink_copy(Source source, Sink sink) {
    while (auto opt = source()) {
        sink(*opt);
    }
    return sink;
}

#endif // PHD_OUTPUT_RANGE_BENCHMARKS_SINK_COPY_HPP
