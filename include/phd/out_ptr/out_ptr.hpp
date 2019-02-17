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
		template <typename Smart, typename Pointer, typename... Args>
		using core_out_ptr_t = simple_out_ptr_t<Smart, Pointer, Args...>;
#else
		template <typename Smart, typename Pointer, typename... Args>
		using core_out_ptr_t = clever_out_ptr_t<Smart, Pointer, Args...>;
#endif // PHD_OUT_PTR_NO_CLEVERNESS
	} // namespace detail

	template <typename Smart, typename Pointer, typename... Args>
	class out_ptr_t : public detail::core_out_ptr_t<Smart, Pointer, Args...> {
	private:
		using base_t = detail::core_out_ptr_t<Smart, Pointer, Args...>;

	public:
		using base_t::base_t;
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	auto out_ptr(Smart& s, Args&&... args) noexcept {
		using P = out_ptr_t<Smart, Pointer, Args...>;
		return P(s, std::forward<Args>(args)...);
	}

	template <typename Smart,
		typename... Args>
	auto out_ptr(Smart& s, Args&&... args) noexcept {
		using Pointer = meta::pointer_of_t<Smart>;
		using P = out_ptr_t<Smart, Pointer, Args...>;
		return P(s, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // PHD_OUT_PTR_HPP
