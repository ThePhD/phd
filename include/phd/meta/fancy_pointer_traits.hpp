#pragma once

#ifndef PHD_META_FANCY_POINTER_TRAITS_HPP
#define PHD_META_FANCY_POINTER_TRAITS_HPP

#include <type_traits>
#include <memory>

namespace phd::meta {
	namespace meta_detail {
		struct disambiguate_ {};

		template <typename T, typename = void>
		struct fancy_pointer_traits {
			using pointer = typename std::pointer_traits<T>::element_type*;
		};

		template <typename T>
		struct fancy_pointer_traits<T, std::void_t<typename T::pointer>> {
			using pointer = typename T::pointer;
		};
	} // namespace meta_detail

	template <typename T>
	struct fancy_pointer_traits : meta_detail::fancy_pointer_traits<T> {};

	template <typename T>
	struct fancy_pointer_traits<T*> {
		using pointer = T*;
	};

	template <typename T, typename = void>
	struct is_releasable : std::false_type {};

	template <typename T>
	struct is_releasable<T, std::void_t<decltype(std::declval<T&>().release())>> : std::true_type {};

} // namespace phd::meta

#endif // PHD_META_FANCY_POINTER_TRAITS_HPP
