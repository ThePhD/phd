#pragma once

#ifndef PHD_OUT_PTR_SIMPLE_INOUT_PTR_HPP
#define PHD_OUT_PTR_SIMPLE_INOUT_PTR_HPP

#include <phd/out_ptr/detail/base_inout_ptr_impl.hpp>

#include <cstddef>
#include <type_traits>
#include <memory>
#include <utility>
#include <tuple>

namespace phd {

	template <typename Smart, typename Pointer, typename Args, typename List>
	struct simple_inout_ptr_t : public out_ptr_detail::base_inout_ptr_impl<Smart, Pointer, Args, List> {
	private:
		using base_t = out_ptr_detail::base_inout_ptr_impl<Smart, Pointer, Args, List>;

	public:
		using base_t::base_t;
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	auto simple_inout_ptr(Smart& p, Args&&... args) noexcept {
		using T = decltype(std::forward_as_tuple(std::forward<Args>(args)...));
		using List = std::make_index_sequence<sizeof...(Args)>;
		return simple_inout_ptr_t<Smart, Pointer, T, List>(p, std::forward_as_tuple(std::forward<Args>(args)...));
	}

	template <typename Smart,
		typename... Args>
	auto simple_inout_ptr(Smart& p, Args&&... args) noexcept {
		typedef typename meta::fancy_pointer_traits<Smart>::pointer Pointer;
		return simple_inout_ptr<Pointer>(p, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // PHD_OUT_PTR_SIMPLE_INOUT_PTR_HPP
