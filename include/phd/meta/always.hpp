#pragma once

#ifndef PHD_META_ALWAYS_HPP
#define PHD_META_ALWAYS_HPP

#include <type_traits>

namespace phd::meta {

	template <typename>
	using always_false = std::integral_constant<bool, false>;

	template <typename T>
	constexpr inline bool always_false_v = always_false<T>::value;

	template <typename>
	using always_true = std::integral_constant<bool, true>;

	template <typename T>
	constexpr inline bool always_true_v = always_true<T>::value;

} // namespace phd::meta

#endif // PHD_META_ALWAYS_HPP