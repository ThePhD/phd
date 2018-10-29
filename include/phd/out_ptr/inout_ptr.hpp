#pragma once

#ifndef PHD_OUT_PTR_INOUT_PTR_HPP
#define PHD_OUT_PTR_INOUT_PTR_HPP

#include <phd/out_ptr/clever_inout_ptr.hpp>
#include <phd/out_ptr/simple_inout_ptr.hpp>
#include <phd/version/version.hpp>

namespace phd {

	namespace detail {
#if defined(PHD_OUT_PTR_NO_CLEVERNESS) && PHD_OUT_PTR_NO_CLEVERNESS != 0
		template <typename Smart, typename Pointer, typename Args, typename List>
		using core_inout_ptr_t = simple_inout_ptr_t<Smart, Pointer, Args, List>;
#else
		template <typename Smart, typename Pointer, typename Args, typename List>
		using core_inout_ptr_t = clever_inout_ptr_t<Smart, Pointer, Args, List>;
#endif // PHD_OUT_PTR_NO_CLEVERNESS
	} // namespace detail

	template <typename Smart, typename Ptr, typename Args>
	struct inout_ptr_t : detail::core_inout_ptr_t<Smart, Ptr, Args, std::make_index_sequence<std::tuple_size<std::remove_reference_t<Args>>::value>> {
	private:
		using list_t = std::make_index_sequence<std::tuple_size<std::remove_reference_t<Args>>::value>;
		using core_t = detail::core_inout_ptr_t<Smart, Ptr, Args, list_t>;

	public:
		using core_t::core_t;
	};
	template <typename Pointer,
		typename Smart,
		typename... Args>
	auto inout_ptr(Smart& s, Args&&... args) noexcept {
		using T = decltype(std::forward_as_tuple(std::forward<Args>(args)...));
		using P = inout_ptr_t<Smart, Pointer, T>;
		return P(s, std::forward_as_tuple(std::forward<Args>(args)...));
	}

	template <typename Smart,
		typename... Args>
	auto inout_ptr(Smart& s, Args&&... args) noexcept {
		typedef typename meta::fancy_pointer_traits<Smart>::pointer Pointer;
		return inout_ptr<Pointer>(s, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // PHD_OUT_PTR_INOUT_PTR_HPP
