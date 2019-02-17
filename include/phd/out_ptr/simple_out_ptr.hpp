#pragma once

#ifndef PHD_OUT_PTR_SIMPLE_OUT_PTR_HPP
#define PHD_OUT_PTR_SIMPLE_OUT_PTR_HPP

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>

#include <cstddef>
#include <type_traits>
#include <memory>
#include <utility>
#include <tuple>

namespace phd {

	template <typename Smart, typename Pointer, typename... Args>
	class simple_out_ptr_t : public out_ptr_detail::base_out_ptr_impl<Smart, Pointer, std::tuple<Args...>, std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>> {
	private:
		using list_t = std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>;
		using core_t = out_ptr_detail::base_out_ptr_impl<Smart, Pointer, std::tuple<Args...>, list_t>;

	public:
		simple_out_ptr_t(Smart& s, Args... args)
		: core_t(s, std::forward_as_tuple(std::forward<Args>(args)...)) {
		}
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	auto simple_out_ptr(Smart& p, Args&&... args) noexcept {
		using P = simple_out_ptr_t<Smart, Pointer, Args...>;
		return P(p, std::forward<Args>(args)...);
	}

	template <typename Smart,
		typename... Args>
	auto simple_out_ptr(Smart& p, Args&&... args) noexcept {
		using Pointer = meta::pointer_of_t<Smart>;
		using P = simple_out_ptr_t<Smart, Pointer, Args...>;
		return P(p, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // PHD_OUT_PTR_SIMPLE_OUT_PTR_HPP
