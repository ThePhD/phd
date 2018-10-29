#pragma once

#ifndef PHD_OUT_PTR_DETAIL_BASE_INOUT_PTR_IMPL_HPP
#define PHD_OUT_PTR_DETAIL_BASE_INOUT_PTR_IMPL_HPP

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>

#include <cstdlib>
#include <type_traits>
#include <memory>
#include <utility>

namespace phd::out_ptr_detail {

	template <typename Smart, typename Pointer, typename Args, typename List>
	struct base_inout_ptr_impl : base_out_ptr_impl<Smart, Pointer, Args, List> {
	private:
		typedef base_out_ptr_impl<Smart, Pointer, Args, List> base_t;

	public:
		base_inout_ptr_impl(Smart& ptr, Args&& args)
		: base_t(ptr, std::move(args), ptr.release()) {
			static_assert(meta::is_releasable<Smart>::value, "You cannot use an inout pointer with something that cannot release() its pointer!");
		}

		base_inout_ptr_impl(base_inout_ptr_impl&& right)
		: base_t(std::move(right)) {
		}
		base_inout_ptr_impl& operator=(base_inout_ptr_impl&& right) {
			static_cast<base_t&>(*this) = std::move(right);
		}

		base_inout_ptr_impl(const base_inout_ptr_impl&) = delete;
		base_inout_ptr_impl& operator=(const base_inout_ptr_impl&) = delete;

		~base_inout_ptr_impl() {
		}
	};
} // namespace phd::out_ptr_detail

#endif // PHD_OUT_PTR_DETAIL_BASE_INOUT_PTR_IMPL_HPP
