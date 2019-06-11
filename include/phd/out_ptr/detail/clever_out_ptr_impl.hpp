#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_DETAIL_CLEVER_OUT_PTR_IMPL_HPP
#define PHD_OUT_PTR_OUT_PTR_DETAIL_CLEVER_OUT_PTR_IMPL_HPP

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>

#include <memory>

namespace phd::detail {

	template <typename Smart, typename Pointer, typename Args, typename List, typename = void>
	struct clever_out_ptr_impl : public base_out_ptr_impl<Smart, Pointer, Args, List> {
	private:
		using base_t = base_out_ptr_impl<Smart, Pointer, Args, List>;

	public:
		using base_t::base_t;
	};

	template <typename T, typename D, typename Pointer>
	struct clever_out_ptr_impl<std::unique_ptr<T, D>, Pointer, std::tuple<>, std::index_sequence<>,
		std::enable_if_t<
			std::is_same_v<pointer_of_or_t<std::unique_ptr<T, D>, Pointer>, Pointer> || std::is_base_of_v<pointer_of_or_t<std::unique_ptr<T, D>, Pointer>, Pointer> || !std::is_convertible_v<pointer_of_or_t<std::unique_ptr<T, D>, Pointer>, Pointer>>>
	: voidpp_op<clever_out_ptr_impl<std::unique_ptr<T, D>, Pointer, std::tuple<>, std::index_sequence<>>, Pointer> {
	public:
		using Smart		 = std::unique_ptr<T, D>;
		using source_pointer = pointer_of_or_t<Smart, Pointer>;

	private:
		using can_aliasing_optimization = std::integral_constant<bool,
			sizeof(std::unique_ptr<T, D>) <= sizeof(Pointer)
				&& sizeof(std::unique_ptr<T, D>) <= sizeof(source_pointer)>;
		Smart* m_smart_ptr;
		Pointer m_old_ptr;
		Pointer* m_target_ptr;

		clever_out_ptr_impl(std::true_type, Smart& ptr) noexcept
		: m_smart_ptr(std::addressof(ptr)), m_old_ptr(static_cast<Pointer>(ptr.get())), m_target_ptr(static_cast<Pointer*>(static_cast<void*>(this->m_smart_ptr))) {
			// we can assume things are cleaner here
#if defined(PHD_OUT_PTR_CLEVER_SANITY_CHECK) && PHD_OUT_PTR_CLEVER_SANITY_CHECK != 0
			if (*this->m_target_ptr != this->m_old_ptr) {
				std::terminate();
			}
#endif // Clever Sanity Checks
		}

		clever_out_ptr_impl(std::false_type, Smart& ptr) noexcept
		: m_smart_ptr(std::addressof(ptr)), m_old_ptr(static_cast<Pointer>(ptr.get())) {
			// analysis necessary
#if defined(PHD_OUT_PTR_CLEVER_UNIQUE_IMPLEMENTATION_FIRST_MEMBER) && PHD_OUT_PTR_CLEVER_UNIQUE_IMPLEMENTATION_FIRST_MEMBER != 0
			// implementation has Pointer as first member: alias directly
			void* target = static_cast<void*>(this->m_smart_ptr);
#else
			// implementation has Pointer as second member: shift, align, alias
			constexpr const std::size_t memory_start = std::is_empty_v<D> ? 0 : sizeof(D) + (sizeof(D) % alignof(D));
			std::size_t max_space				 = sizeof(Smart) - memory_start;
			void* source						 = static_cast<void*>(static_cast<char*>(static_cast<void*>(this->m_smart_ptr)) + memory_start);
			void* target						 = std::align(alignof(source_pointer), sizeof(source_pointer), source, max_space);
#endif
			// get direct Pointer
			this->m_target_ptr = static_cast<Pointer*>(target);
#if defined(PHD_OUT_PTR_CLEVER_SANITY_CHECK) && PHD_OUT_PTR_CLEVER_SANITY_CHECK != 0
			if (*this->m_target_ptr != this->m_old_ptr) {
				std::terminate();
			}
#endif // Clever Sanity Checks
		}

	public:
		clever_out_ptr_impl(Smart& ptr, std::tuple<>&&) noexcept
		: clever_out_ptr_impl(can_aliasing_optimization(), ptr) {
		}
		clever_out_ptr_impl(clever_out_ptr_impl&& right) noexcept
		: m_smart_ptr(right.m_smart_ptr), m_old_ptr(right.m_old_ptr), m_target_ptr(right.m_target_ptr) {
			right.m_old_ptr == nullptr;
		}
		clever_out_ptr_impl& operator=(clever_out_ptr_impl&& right) noexcept {
			this->m_smart_ptr  = right.m_smart_ptr;
			this->m_old_ptr    = right.m_old_ptr;
			this->m_target_ptr = right.m_target_ptr;
			right.m_old_ptr == nullptr;
			return *this;
		}
		clever_out_ptr_impl(const clever_out_ptr_impl&) = delete;
		clever_out_ptr_impl& operator=(const clever_out_ptr_impl&) = delete;

		operator Pointer*() noexcept {
			return this->m_target_ptr;
		}
		operator Pointer&() noexcept {
			return *this->m_target_ptr;
		}
		~clever_out_ptr_impl() noexcept {
			if (this->m_old_ptr != nullptr) {
				this->m_smart_ptr->get_deleter()(static_cast<source_pointer>(this->m_old_ptr));
			}
		}
	};
} // namespace phd::detail

#endif // PHD_OUT_PTR_OUT_PTR_DETAIL_CLEVER_OUT_PTR_IMPL_HPP
