#pragma once
#ifndef PHD_OVERLOAD_OVERLOAD_DETAIL_HPP
#define PHD_OVERLOAD_OVERLOAD_DETAIL_HPP

#include <phd/meta/remove_cv_ref.hpp>
#include <phd/meta/callable_traits.hpp>

namespace phd::overload_detail {

	template <typename F, typename = void>
	struct b_m__ {
		using T = std::add_lvalue_reference_t<typename meta::callable_traits<F>::qualified_object_type>;
		using P = std::add_pointer_t<std::remove_reference_t<T>>;

		F f;

		template <typename Fa>
		b_m__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		template <typename... Args, typename = std::enable_if_t<std::is_invocable_v<F, T, Args...>>>
		decltype(auto) operator()(T self, Args&&... args) const {
			return (self.*f)(std::forward<Args>(args)...);
		}

		template <typename... Args, typename = std::enable_if_t<std::is_invocable_v<F, T, Args...>>>
		decltype(auto) operator()(P self, Args... args) const {
			return this->operator()(*self, std::forward<Args>(args)...);
		}
	};

	template <typename F>
	struct b_m__<F, std::enable_if_t<std::is_member_object_pointer_v<F>>> {
		using T = typename meta::callable_traits<F>::object_type;
		using R = typename meta::callable_traits<F>::return_type;

		F f;

		template <typename Fa>
		b_m__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		decltype(auto) operator()(const T& self) const {
			return (self.*f);
		}

		decltype(auto) operator()(T const* self) const {
			return this->operator()(*self);
		}

		template <typename FArg, typename = std::enable_if_t<!std::is_const_v<R> && std::is_assignable_v<R, FArg>>>
		decltype(auto) operator()(T& self, FArg&& arg) const {
			return (self.*f) = std::forward<FArg>(arg);
		}

		template <typename FArg, typename = std::enable_if_t<!std::is_const_v<R> && std::is_assignable_v<R, FArg>>>
		decltype(auto) operator()(T* self, FArg&& arg) const {
			return this->operator()(*self, std::forward<FArg>(arg));
		}
	};

	template <typename F, typename = void>
	struct b__ : F {
		using F::operator();

		template <typename Fa>
		b__(Fa&& fa)
		: F(std::forward<Fa>(fa)) {
		}
	};

	template <typename F>
	struct b__<F, std::enable_if_t<std::is_function_v<F> || std::is_final_v<F>>> {
		using stored_f = std::conditional_t<std::is_function_v<F>, std::decay_t<F>, F>;
		stored_f f;

		template <typename Fa>
		b__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		template <typename... Args, typename = std::enable_if_t<std::is_invocable_v<const F, Args...>>>
		decltype(auto) operator()(Args&&... args) const {
			return f(std::forward<Args>(args)...);
		}

		template <typename... Args, typename = std::enable_if_t<std::is_invocable_v<F, Args...>>>
		decltype(auto) operator()(Args&&... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F>
	struct b__<F, std::enable_if_t<std::is_member_pointer_v<meta::remove_cv_ref_t<F>>>> : b_m__<F> {

		using b_m__<F>::operator();

		template <typename Fa>
		b__(Fa&& fa)
		: b_m__<F>(std::forward<Fa>(fa)) {
		}
	};

	template <typename F>
	using b_ = b__<std::remove_pointer_t<F>>;

	template <typename F1, typename... Fs>
	struct t_ : public b_<F1>, public t_<Fs...> {

		using b_<F1>::operator();
		using t_<Fs...>::operator();

		template <typename Fa1, typename... Fas>
		t_(Fa1&& fa1, Fas&&... fas)
		: b_<F1>(std::forward<Fa1>(fa1)), t_<Fs...>(std::forward<Fas>(fas)...) {
		}
	};

	template <typename F>
	struct t_<F> : public b_<F> {

		using b_<F>::operator();

		template <typename Fa>
		t_(Fa&& fa)
		: b_<F>(std::forward<Fa>(fa)) {
		}
	};

} // namespace phd::overload_detail

#endif // PHD_OVERLOAD_OVERLOAD_DETAIL_HPP
