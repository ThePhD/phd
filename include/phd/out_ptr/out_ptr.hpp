#pragma once

#ifndef PHD_OUT_PTR_HPP
#define PHD_OUT_PTR_HPP

#include <phd/out_ptr/simple_out_ptr.hpp>
#include <phd/out_ptr/clever_out_ptr.hpp>
#include <phd/version/version.hpp>

#include <type_traits>
#include <tuple>

namespace phd {

	namespace detail {
#if defined(PHD_OUT_PTR_NO_CLEVERNESS) && PHD_OUT_PTR_NO_CLEVERNESS != 0
		template <typename Smart, typename Ptr, typename Args, typename List>
		using core_out_ptr_t = simple_out_ptr_t<Smart, Ptr, Args, List>;
#else
		template <typename Smart, typename Ptr, typename Args, typename List>
		using core_out_ptr_t = clever_out_ptr_t<Smart, Ptr, Args, List>;
#endif // PHD_OUT_PTR_NO_CLEVERNESS
	} // namespace detail

	template <typename Smart, typename Ptr, typename Args>
	struct out_ptr_t : detail::core_out_ptr_t<Smart, Ptr, Args, std::make_index_sequence<std::tuple_size<std::remove_reference_t<Args>>::value>> {
	private:
		using list_t = std::make_index_sequence<std::tuple_size<std::remove_reference_t<Args>>::value>;
		using core_t = detail::core_out_ptr_t<Smart, Ptr, Args, list_t>;

	public:
		using core_t::core_t;
	};

	template <typename Ptr,
		typename Smart,
		typename... Args>
	auto out_ptr(Smart& s, Args&&... args) noexcept {
		using T = decltype(std::forward_as_tuple(std::forward<Args>(args)...));
		using P = out_ptr_t<Smart, Ptr, T>;
		return P(s, std::forward_as_tuple(std::forward<Args>(args)...));
	}

	template <typename Smart,
		typename... Args>
	auto out_ptr(Smart& s, Args&&... args) noexcept {
		typedef typename meta::fancy_pointer_traits<Smart>::pointer Ptr;
		return out_ptr<Ptr>(s, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // PHD_OUT_PTR_HPP
