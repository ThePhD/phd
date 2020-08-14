// barrier
// The Kek License (KEK)

// Copyright � 2018-2020 ThePhD

// kek :3

#pragma once

#ifndef PHD_BARRIER_BARRIER_HPP
#define PHD_BARRIER_BARRIER_HPP

#include <vector>

#if defined _MSC_VER
#define PHD_BARRIER_VC
#elif defined __GNUC__
#define PHD_BARRIER_GCC
#elif defined __clang__
#define PHD_BARRIER_CLANG
#endif

#if defined PHD_BARRIER_VC
#if defined PHD_BARRIER_DLL
#if defined PHD_BARRIER_BUILD
#define PHD_BARRIER_API __declspec(dllexport)
#else
#define PHD_BARRIER_API __declspec(dllexport)
#endif // PHD_BARRIER_BUILD - Building the Library vs. Using the Library
#else
#define PHD_BARRIER_API
#endif // Building a DLL vs. Static Library
#else  // g++ / clang++
#define PHD_BARRIER_API __attribute__((visibility("default")))
#endif // PHD_BARRIER_BUILD

namespace barrier {

	PHD_BARRIER_API std::vector<size_t>& inplace_transform_vector(std::vector<size_t>& source);

	PHD_BARRIER_API size_t& inplace_transform_int(size_t& source);

	PHD_BARRIER_API std::vector<size_t> pure_transform_vector(std::vector<size_t> source);

	PHD_BARRIER_API size_t pure_transform_int(size_t source);

	PHD_BARRIER_API size_t pure_write_int(size_t source);

	struct write {
		std::size_t* target;

		PHD_BARRIER_API void operator()(std::size_t value) noexcept;
	};

} // namespace barrier

namespace transparent {

	inline std::vector<size_t>& inplace_transform_vector(std::vector<size_t>& source) {
		for (auto& v : source) {
			v *= 2;
		}
		return source;
	}

	inline size_t& inplace_transform_int(size_t& source) {
		return source *= 2;
	}

	inline std::vector<size_t> pure_transform_vector(std::vector<size_t> data) {
		for (auto& v : data) {
			v *= 2;
		}
		return data;
	}

	inline size_t pure_transform_int(size_t source) {
		return source * 2;
	}

	inline size_t pure_write_int(size_t source) {
		return source;
	}

	struct write {
		std::size_t* target;

		inline void operator()(std::size_t value) noexcept {
			*target = value;
			target++;
		}
	};

} // namespace transparent

#endif // PHD_BARRIER_BARRIER_HPP
