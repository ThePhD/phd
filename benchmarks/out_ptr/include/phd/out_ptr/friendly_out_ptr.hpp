#pragma once

#ifndef PHD_OUT_PTR_FRIENDLY_OUT_PTR_HPP
#define PHD_OUT_PTR_FRIENDLY_OUT_PTR_HPP

#if defined(PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR) && PHD_OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR != 0

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>
#include <phd/out_ptr/detail/voidpp_op.hpp>
#include <phd/out_ptr/version.hpp>
#include <phd/meta/fancy_pointer_traits.hpp>

#include <cstdlib>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <tuple>

namespace phd {

	namespace out_ptr_detail {
		template <typename Smart, typename Pointer, typename Args, typename List>
		struct friendly_out_ptr_impl : public base_out_ptr_impl<Smart, Pointer, Args, List> {
		private:
			using base_t = base_out_ptr_impl<Smart, Pointer, Args, List>;

		public:
			using base_t::base_t;
		};

		template <typename T, typename D, typename Pointer>
		struct friendly_out_ptr_impl<std::friendly_unique_ptr<T, D>, Pointer, std::tuple<>, std::index_sequence<>>
		: voidpp_op<friendly_out_ptr_impl<std::friendly_unique_ptr<T, D>, Pointer, std::tuple<>, std::index_sequence<>>, Pointer> {
		public:
			typedef std::friendly_unique_ptr<T, D> Smart;
			typedef typename meta::fancy_pointer_traits<Smart>::pointer source_pointer;

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
				m_smart_ptr = right.m_smart_ptr;
				m_old_ptr = right.m_old_ptr;
				m_target_ptr = right.m_target_ptr;
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
	} // namespace out_ptr_detail

	template <typename Smart, typename Pointer, typename Args, typename List>
	struct friendly_out_ptr_t : public out_ptr_detail::friendly_out_ptr_impl<Smart, Pointer, Args, List> {
	private:
		using base_t = out_ptr_detail::friendly_out_ptr_impl<Smart, Pointer, Args, List>;

	public:
		using base_t::base_t;
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	decltype(auto) friendly_out_ptr(Smart& p, Args&&... args) {
		using T = decltype(std::forward_as_tuple(std::forward<Args>(args)...));
		using List = std::make_index_sequence<sizeof...(Args)>;
		return friendly_out_ptr_t<Smart, Pointer, T, List>(p, std::forward_as_tuple(std::forward<Args>(args)...));
	}

	template <typename Smart,
		typename... Args>
	auto friendly_out_ptr(Smart& p, Args&&... args) {
		typedef typename meta::fancy_pointer_traits<Smart>::pointer Pointer;
		return friendly_out_ptr<Pointer>(p, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // OUT_PTR_HAS_FRIENDLY_UNIQUE_PTR

#endif // PHD_OUT_PTR_FRIENDLY_OUT_PTR_HPP
