#pragma once

#ifndef PHD_META_POINTER_TYPE_HPP
#define PHD_META_POINTER_TYPE_HPP

#include <type_traits>

namespace phd::meta {

	namespace meta_detail {
		template <typename T>
		struct has_typedef_pointer {
			typedef std::true_type yes;
			typedef std::false_type no;

			template <typename C>
			static yes& test(typename C::pointer*);

			template <typename>
			static no& test(...);

			static constexpr const bool value = std::is_same_v<decltype(test<T>(0)), yes>;
		};

		template <bool b, typename T, typename Fallback>
		struct pointer_typedef_enable_if {};

		template <typename T, typename Fallback>
		struct pointer_typedef_enable_if<false, T, Fallback> {
			typedef Fallback type;
		};

		template <typename T, typename Fallback>
		struct pointer_typedef_enable_if<true, T, Fallback> {
			typedef typename T::pointer type;
		};
	} // namespace meta_detail

	template <typename T, typename Dx>
	struct pointer_type {
		typedef typename meta_detail::pointer_typedef_enable_if<meta_detail::has_typedef_pointer<Dx>::value, Dx, T>::type type;
	};

	template <typename T, typename D>
	using pointer_type_t = typename pointer_type<T, D>::type;

} // namespace phd::meta

#endif // PHD_META_POINTER_TYPE_HPP
