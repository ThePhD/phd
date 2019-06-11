#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_DETAIL_BASE_OUT_PTR_IMPL_HPP
#define PHD_OUT_PTR_OUT_PTR_DETAIL_BASE_OUT_PTR_IMPL_HPP

#include <phd/meta/is_specialization_of.hpp>
#include <phd/out_ptr/pointer_of.hpp>
#include <phd/out_ptr/detail/voidpp_op.hpp>

#include <cstdlib>
#include <type_traits>
#include <memory>
#include <utility>
#include <tuple>

namespace phd::detail {

	template <typename Smart, typename... Args>
	void reset_or_create(std::true_type, Smart& s, Args&&... args) {
		s.reset(std::forward<Args>(args)...);
	}

	template <typename Smart, typename... Args>
	void reset_or_create(std::false_type, Smart& s, Args&&... args) {
		s = Smart(std::forward<Args>(args)...);
	}

	template <typename Smart, typename Pointer, typename Args, typename List>
	struct base_out_ptr_impl;

	template <typename Smart, typename Pointer, typename Args, std::size_t... Indices>
	struct base_out_ptr_impl<Smart, Pointer, Args, std::index_sequence<Indices...>>
	: voidpp_op<base_out_ptr_impl<Smart, Pointer, Args, std::index_sequence<Indices...>>, Pointer>,
	  Args {
	protected:
		using source_pointer = pointer_of_or_t<Smart, Pointer>;
		using can_reset	 = is_resetable<Smart,
			decltype(static_cast<source_pointer>(nullptr)),
			decltype(std::get<Indices>(std::declval<Args&&>()))...>;

		Smart* m_smart_ptr;
		Pointer m_target_ptr;

		static_assert(!meta::is_specialization_of_v<Smart, std::shared_ptr> || (sizeof...(Indices) > 0),
			"shared_ptr<T> must pass a deleter in alongside the out_ptr "
			"so when reset is called the deleter can be properly "
			"initialized, otherwise the deleter will be defaulted "
			"by the shared_ptr<T>::reset() call!");

		base_out_ptr_impl(Smart& ptr, Args&& args, Pointer initial) noexcept
		: Args(std::move(args)), m_smart_ptr(std::addressof(ptr)), m_target_ptr(initial) {
		}

		base_out_ptr_impl(Smart& ptr, Args&& args, meta::meta_detail::disambiguate_) noexcept
		: Args(std::move(args)), m_smart_ptr(std::addressof(ptr)), m_target_ptr() {
		}

	public:
		base_out_ptr_impl(Smart& ptr, Args&& args) noexcept
		: base_out_ptr_impl(ptr, std::move(args), meta::meta_detail::disambiguate_()) {
		}

		base_out_ptr_impl(base_out_ptr_impl&& right) noexcept
		: Args(std::move(*this)), m_smart_ptr(right.m_smart_ptr), m_target_ptr(right.m_target_ptr) {
			right.m_smart_ptr = nullptr;
		}
		base_out_ptr_impl& operator=(base_out_ptr_impl&& right) noexcept {
			static_cast<Args&>(*this) = std::move(right);
			this->m_smart_ptr		 = right.m_smart_ptr;
			this->m_target_ptr		 = right.m_target_ptr;
			right.m_smart_ptr		 = nullptr;
			return *this;
		}
		base_out_ptr_impl(const base_out_ptr_impl&) noexcept = delete;
		base_out_ptr_impl& operator=(const base_out_ptr_impl&) noexcept = delete;

		operator Pointer*() noexcept {
			return std::addressof(this->m_target_ptr);
		}
		operator Pointer&() noexcept {
			return this->m_target_ptr;
		}

		~base_out_ptr_impl() noexcept(noexcept(reset_or_create(can_reset(), std::declval<Smart&>(), std::declval<source_pointer>(), std::get<Indices>(std::declval<Args&&>())...))) {
			if (m_smart_ptr) {
				Args&& args = std::move(static_cast<Args&>(*this));
				// lmao "if constexpr" xD
				reset_or_create(can_reset(), *this->m_smart_ptr, static_cast<source_pointer>(this->m_target_ptr), std::get<Indices>(std::move(args))...);
			}
		}
	};
} // namespace phd::detail

#endif // PHD_OUT_PTR_OUT_PTR_DETAIL_BASE_OUT_PTR_IMPL_HPP
