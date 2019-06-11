#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_FRIENDLY_OUT_PTR_HPP
#define PHD_OUT_PTR_OUT_PTR_FRIENDLY_OUT_PTR_HPP

#if defined(PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR) && PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR != 0

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>
#include <phd/out_ptr/detail/voidpp_op.hpp>
#include <phd/out_ptr/version.hpp>
#include <phd/out_ptr/pointer_of.hpp>

#include <cstdlib>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <tuple>

namespace phd {

	namespace detail {
		template <typename Smart, typename Pointer, typename Args, typename List>
		class friendly_out_ptr_impl : public base_out_ptr_impl<Smart, Pointer, Args, List> {
		private:
			using base_t = base_out_ptr_impl<Smart, Pointer, Args, List>;

		public:
			using base_t::base_t;
		};

		template <typename T, typename D, typename Pointer>
		struct friendly_out_ptr_impl<std::friendly_unique_ptr<T, D>, Pointer, std::tuple<>, std::index_sequence<>>
		: voidpp_op<friendly_out_ptr_impl<std::friendly_unique_ptr<T, D>, Pointer, std::tuple<>, std::index_sequence<>>, Pointer> {
		public:
			using Smart		 = std::friendly_unique_ptr<T, D>;
			using source_pointer = pointer_of_or_t<Smart, Pointer>;

		private:
			Smart* m_smart_ptr;
			Pointer m_old_ptr;
			Pointer* m_target_ptr;

		public:
			friendly_out_ptr_impl(Smart& ptr, std::tuple<>&&) noexcept
			: m_smart_ptr(std::addressof(ptr)), m_old_ptr(static_cast<Pointer>(ptr.get())), m_target_ptr(static_cast<Pointer*>(static_cast<void*>(std::addressof(ptr.friendly_get())))) {
			}

			friendly_out_ptr_impl(friendly_out_ptr_impl&& right) noexcept
			: m_smart_ptr(right.m_smart_ptr), m_old_ptr(right.m_old_ptr), m_target_ptr(right.m_target_ptr) {
				right.m_old_ptr = nullptr;
			}
			friendly_out_ptr_impl& operator=(friendly_out_ptr_impl&& right) noexcept {
				m_smart_ptr	= right.m_smart_ptr;
				m_old_ptr		 = right.m_old_ptr;
				m_target_ptr    = right.m_target_ptr;
				right.m_old_ptr = nullptr;
				return *this;
			}
			friendly_out_ptr_impl(const friendly_out_ptr_impl&) = delete;
			friendly_out_ptr_impl& operator=(const friendly_out_ptr_impl&) = delete;
			operator Pointer*() noexcept {
				return m_target_ptr;
			}
			operator Pointer&() noexcept {
				return *m_target_ptr;
			}
			~friendly_out_ptr_impl() noexcept {
				if (m_old_ptr != nullptr) {
					m_smart_ptr->get_deleter()(m_old_ptr);
				}
			}
		};
	} // namespace detail

	template <typename Smart, typename Pointer, typename... Args>
	struct friendly_out_ptr_t : public detail::friendly_out_ptr_impl<Smart, Pointer, std::tuple<Args...>, std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>> {
	private:
		using list_t = std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>;
		using core_t = detail::friendly_out_ptr_impl<Smart, Pointer, std::tuple<Args...>, list_t>;

	public:
		friendly_out_ptr_t(Smart& s, Args... args)
		: core_t(s, std::forward_as_tuple(std::forward<Args>(args)...)) {
		}
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	decltype(auto) friendly_out_ptr(Smart& p, Args&&... args) {
		return friendly_out_ptr_t<Smart, Pointer, Args...>(p, std::forward<Args>(args)...);
	}

	template <typename Smart,
		typename... Args>
	auto friendly_out_ptr(Smart& p, Args&&... args) {
		using Pointer = pointer_of_t<Smart>;
		return friendly_out_ptr<Pointer>(p, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR

#endif // PHD_OUT_PTR_OUT_PTR_FRIENDLY_OUT_PTR_HPP
