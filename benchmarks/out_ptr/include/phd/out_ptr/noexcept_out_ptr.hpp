#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_FRIENDLY_OUT_PTR_HPP
#define PHD_OUT_PTR_OUT_PTR_FRIENDLY_OUT_PTR_HPP

#include <phd/out_ptr/detail/base_out_ptr_impl.hpp>
#include <phd/out_ptr/detail/out_ptr_traits.hpp>
#include <phd/out_ptr/detail/voidpp_op.hpp>
#include <phd/out_ptr/pointer_of.hpp>

#include <cstdlib>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <tuple>

namespace phd { namespace out_ptr { namespace detail {

	template <typename Smart, typename Pointer, typename Args, typename List>
	class PHD_OUT_PTR_TRIVIAL_ABI noexcept_out_ptr_impl;

	template <typename Smart, typename Pointer, typename Base, std::size_t... Indices>
	class PHD_OUT_PTR_TRIVIAL_ABI noexcept_out_ptr_impl<Smart, Pointer, Base, std::index_sequence<Indices...>>
	: public voidpp_op<noexcept_out_ptr_impl<Smart, Pointer, Base, std::index_sequence<Indices...>>, Pointer>, protected Base {
	protected:
		using storage = Pointer;
		Smart* m_smart_ptr;
		storage m_target_ptr;

		static_assert(!(is_specialization_of<Smart, std::shared_ptr>::value || is_specialization_of<Smart, ::boost::shared_ptr>::value)
				|| (sizeof...(Indices) > 0), // clang-format hack
			"shared_ptr<T> must pass a deleter in alongside the out_ptr "
			"so when reset is called the deleter can be properly "
			"initialized, otherwise the deleter will be defaulted "
			"by the shared_ptr<T>::reset() call!");

		noexcept_out_ptr_impl(Smart& ptr, Base&& args, storage initial) noexcept
		: Base(std::move(args)), m_smart_ptr(std::addressof(ptr)), m_target_ptr(initial) {
		}

	public:
		noexcept_out_ptr_impl(Smart& ptr, Base&& args) noexcept
		: Base(std::move(args)), m_smart_ptr(std::addressof(ptr)), m_target_ptr{} {
		}

		noexcept_out_ptr_impl(noexcept_out_ptr_impl&& right) noexcept
		: Base(std::move(right)), m_smart_ptr(right.m_smart_ptr), m_target_ptr(std::move(right.m_target_ptr)) {
			right.m_smart_ptr = nullptr;
		}
		noexcept_out_ptr_impl& operator=(noexcept_out_ptr_impl&& right) noexcept {
			static_cast<Base&>(*this) = std::move(right);
			this->m_smart_ptr		 = std::move(right.m_smart_ptr);
			this->m_target_ptr		 = std::move(right.m_target_ptr);
			right.m_smart_ptr		 = nullptr;
			return *this;
		}

		operator Pointer*() const noexcept {
			return std::addressof(const_cast<storage&>(this->m_target_ptr));
		}

		~noexcept_out_ptr_impl() noexcept {
			if (this->m_smart_ptr) {
				Base&& args = std::move(static_cast<Base&>(*this));
				(void)args; // unused if "Indices" is empty
				this->m_smart_ptr->reset(this->m_target_ptr, std::get<Indices>(std::move(args))...);
			}
		}
	};

	template <typename Smart, typename Pointer, typename... Args>
	class noexcept_out_ptr_t : public noexcept_out_ptr_impl<Smart, Pointer, std::tuple<Args...>, std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>> {
	private:
		using list_t = std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>;
		using core_t = noexcept_out_ptr_impl<Smart, Pointer, std::tuple<Args...>, list_t>;

	public:
		noexcept_out_ptr_t(Smart& s, Args... args)
		: core_t(s, std::forward_as_tuple(std::forward<Args>(args)...)) {
		}
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	decltype(auto) noexcept_out_ptr(Smart& p, Args&&... args) {
		return noexcept_out_ptr_t<Smart, Pointer, Args...>(p, std::forward<Args>(args)...);
	}

	template <typename Smart,
		typename... Args>
	auto noexcept_out_ptr(Smart& p, Args&&... args) {
		using Pointer = pointer_of_t<Smart>;
		return noexcept_out_ptr<Pointer>(p, std::forward<Args>(args)...);
	}
}}} // namespace phd::out_ptr::detail

#endif // PHD_OUT_PTR_OUT_PTR_FRIENDLY_OUT_PTR_HPP
