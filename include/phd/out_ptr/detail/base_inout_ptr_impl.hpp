#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_DETAIL_BASE_INOUT_PTR_IMPL_HPP
#define PHD_OUT_PTR_OUT_PTR_DETAIL_BASE_INOUT_PTR_IMPL_HPP

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>
#include <phd/out_ptr/pointer_of.hpp>

#include <cstdlib>
#include <type_traits>
#include <memory>
#include <utility>

namespace phd::detail {

	template <typename Smart>
	void call_release(std::true_type, Smart& s) {
		s.release();
	}

	template <typename Smart>
	void call_release(std::false_type, Smart&) {
		static_assert(std::is_pointer_v<Smart>, "the type that does not get release called on it must be a pointer type");
	}

	template <typename Smart, typename Pointer, typename Args, typename List>
	struct base_inout_ptr_impl : base_out_ptr_impl<Smart, Pointer, Args, List> {
	private:
		using base_t = base_out_ptr_impl<Smart, Pointer, Args, List>;

	public:
		base_inout_ptr_impl(Smart& ptr, Args&& args) noexcept
		: base_t(ptr, std::move(args), ptr.get()) {
			static_assert(is_releasable_v<Smart> || std::is_pointer_v<Smart>, "You cannot use an inout pointer with something that cannot release() its pointer!");
		}

		base_inout_ptr_impl(base_inout_ptr_impl&& right) noexcept
		: base_t(std::move(right)) {
		}
		base_inout_ptr_impl& operator=(base_inout_ptr_impl&& right) noexcept {
			static_cast<base_t&>(*this) = std::move(right);
		}

		base_inout_ptr_impl(const base_inout_ptr_impl&) noexcept = delete;
		base_inout_ptr_impl& operator=(const base_inout_ptr_impl&) noexcept = delete;

		~base_inout_ptr_impl() noexcept(noexcept(call_release(is_releasable<Smart>(), std::declval<Smart&>()))) {
			call_release(is_releasable<Smart>(), *(this->m_smart_ptr));
		}
	};
} // namespace phd::detail

#endif // PHD_OUT_PTR_OUT_PTR_DETAIL_BASE_INOUT_PTR_IMPL_HPP
