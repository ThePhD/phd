#pragma once

#ifndef PHD_OUT_OTR_OUT_PTR_POINTER_OF_HPP
#define PHD_OUT_OTR_OUT_PTR_POINTER_OF_HPP

#include <phd/meta/pointer_of.hpp>
#include <type_traits>

namespace phd {

	namespace detail {

		template <typename T, typename = void>
		struct is_releasable : std::false_type {};

		template <typename T>
		struct is_releasable<T, std::void_t<decltype(std::declval<T&>().release())>> : std::true_type {};

		template <typename T>
		inline constexpr bool is_releasable_v = is_releasable<T>::value;

		template <typename T, typename... Args>
		struct is_resetable_with {
		private:
			template <typename S>
			static std::true_type f(decltype(std::declval<S>().reset(std::declval<Args>()...))*);
			template <typename S>
			static std::false_type f(...);

		public:
			constexpr static bool value = std::is_same_v<decltype(f<T>(0)), std::true_type>;
		};

		template <typename T, typename... Args>
		struct is_resetable : std::integral_constant<bool, is_resetable_with<T, Args...>::value> {};

		template <typename T, typename... Args>
		inline constexpr bool is_resetable_v = is_resetable<T, Args...>::value;

	} // namespace detail

	template <typename T>
	using pointer_of = meta::pointer_of<T>;

	template <typename T>
	using pointer_of_t = meta::pointer_of_t<T>;

	template <typename T, typename P>
	using pointer_of_or = meta::pointer_of_or<T, P>;

	template <typename T, typename P>
	using pointer_of_or_t = meta::pointer_of_or_t<T, P>;

} // namespace phd

#endif // PHD_OUT_OTR_OUT_PTR_POINTER_OF_HPP
