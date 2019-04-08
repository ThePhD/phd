#pragma once

#ifndef PHD_META_IS_DETECTED_HPP
#define PHD_META_IS_DETECTED_HPP

#include <type_traits>

namespace phd {

	namespace meta_detail {
		template <template <class...> class Test, class, class... Args>
		struct is_detected : std::false_type {};

		template <template <class...> class Test, class... Args>
		struct is_detected<Test, std::void_t<Test<Args...>>, Args...> : std::true_type {};
	} // namespace meta_detail

	template <template <class...> class Trait, class... Args>
	using is_detected = typename meta_detail::is_detected<Trait, void, Args...>::type;

	template <template <class...> class Trait, class... Args>
	constexpr inline bool is_detected_v = is_detected<Trait, Args...>::value;
} // namespace phd

#endif // PHD_META_IS_DETECTED_HPP