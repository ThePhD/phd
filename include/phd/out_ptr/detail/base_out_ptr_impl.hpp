#pragma once

#ifndef PHD_OUT_PTR_DETAIL_BASE_OUT_PTR_IMPL_HPP
#define PHD_OUT_PTR_DETAIL_BASE_OUT_PTR_IMPL_HPP

#include <phd/meta/is_specialization_of.hpp>
#include <phd/meta/fancy_pointer_traits.hpp>
#include <phd/out_ptr/detail/voidpp_op.hpp>

#include <cstdlib>
#include <type_traits>
#include <memory>
#include <utility>

namespace phd::out_ptr_detail {

	template <typename Smart, typename Pointer, typename Args, typename List>
	struct base_out_ptr_impl;

	template <typename Smart, typename Pointer, typename Args, std::size_t... Indices>
	struct base_out_ptr_impl<Smart, Pointer, Args, std::index_sequence<Indices...>>
	: voidpp_op<base_out_ptr_impl<Smart, Pointer, Args, std::index_sequence<Indices...>>, Pointer>,
	  Args {
	public:
		typedef typename meta::fancy_pointer_traits<Smart>::pointer source_pointer;

		static_assert(!meta::is_specialization_of<Smart, std::shared_ptr>::value
				|| (sizeof...(Indices) > 0),
			"shared_ptr<T> must pass a deleter in alongside the out_ptr "
			"so when reset is called the deleter can be properly "
			"initialized, otherwise the deleter will be defaulted "
			"by the shared_ptr<T>::reset() call!");

	protected:
		Smart* m_smart_ptr;
		Pointer m_target_ptr;

		base_out_ptr_impl(Smart& ptr, Args&& args, Pointer initial)
		: Args(std::move(args)), m_smart_ptr(std::addressof(ptr)), m_target_ptr(initial) {
		}

		base_out_ptr_impl(Smart& ptr, Args&& args, meta::meta_detail::disambiguate_)
		: Args(std::move(args)), m_smart_ptr(std::addressof(ptr)) {
		}

	public:
		base_out_ptr_impl(Smart& ptr, Args&& args)
		: base_out_ptr_impl(ptr, std::move(args), meta::meta_detail::disambiguate_()) {
		}

		base_out_ptr_impl(base_out_ptr_impl&& right)
		: Args(std::move(*this)), m_smart_ptr(right.m_smart_ptr), m_target_ptr(right.m_target_ptr) {
			right.m_smart_ptr = nullptr;
		}
		base_out_ptr_impl& operator=(base_out_ptr_impl&& right) {
			static_cast<Args&>(*this) = std::move(right);
			this->m_smart_ptr = right.m_smart_ptr;
			this->m_target_ptr = right.m_target_ptr;
			right.m_smart_ptr = nullptr;
			return *this;
		}
		base_out_ptr_impl(const base_out_ptr_impl&) = delete;
		base_out_ptr_impl& operator=(const base_out_ptr_impl&) = delete;

		operator Pointer*() noexcept {
			return std::addressof(this->m_target_ptr);
		}
		operator Pointer&() noexcept {
			return this->m_target_ptr;
		}

		~base_out_ptr_impl() {
			if (m_smart_ptr) {
				Args&& args = std::move(static_cast<Args&>(*this));
				this->m_smart_ptr->reset(static_cast<source_pointer>(this->m_target_ptr), std::get<Indices>(std::move(args))...);
			}
		}
	};
} // namespace phd::out_ptr_detail

#endif // PHD_OUT_PTR_DETAIL_BASE_OUT_PTR_IMPL_HPP