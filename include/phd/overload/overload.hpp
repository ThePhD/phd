#pragma once
#ifndef PHD_OVERLOAD_OVERLOAD_HPP
#define PHD_OVERLOAD_OVERLOAD_HPP

#include <phd/meta/remove_cv_ref.hpp>
#include <phd/overload/overload_detail.hpp>

namespace phd {

	namespace overload_detail {
		template <typename... Fs>
		struct noexcept_movable;

		template <>
		struct noexcept_movable<> : std::true_type {};

		template <typename... Fs>
		inline constexpr bool noexcept_movable_v = noexcept_movable<Fs...>::value;

		template <typename F, typename... Fs>
		struct noexcept_movable<F, Fs...> : std::integral_constant<bool, std::is_nothrow_move_constructible_v<F> && noexcept_movable_v<Fs...>> {};

	} // namespace overload_detail

	template <typename... Fs>
	constexpr auto overload(Fs&&... fs) noexcept(overload_detail::noexcept_movable_v<Fs...>) {
		return overload_detail::t_<meta::remove_cv_ref_t<Fs>...>(std::forward<Fs>(fs)...);
	}

} // namespace phd

#endif // PHD_OVERLOAD_OVERLOAD_HPP
