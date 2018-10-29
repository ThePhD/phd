#pragma once
#ifndef PHD_OVERLOAD_OVERLOAD_DETAIL_HPP
#define PHD_OVERLOAD_OVERLOAD_DETAIL_HPP

#include <phd/meta/remove_cv_ref.hpp>

namespace phd::overload_detail {

	template <typename F, typename = void>
	struct b_m__;

	template <typename T, typename R>
	struct b_m__<R T::*> {
		using F = R T::*;

		F f;

		template <typename Fa>
		b_m__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(T& self) const {
			return (self.*f);
		}

		R operator()(T* self) const {
			return this->operator()(*self);
		}

		R operator()(const T& self) const {
			return (self.*f);
		}

		R operator()(T const* self) const {
			return this->operator()(*self);
		}

		R operator()(T& self, const R& arg) const {
			return (self.*f) = arg;
		}

		R operator()(T& self, R&& arg) const {
			return (self.*f) = std::move(arg);
		}

		R operator()(T* self, const R& arg) const {
			return this->operator()(*self, arg);
		}

		R operator()(T* self, R&& arg) const {
			return this->operator()(*self, std::move(arg));
		}
	};

	template <typename T, typename R, typename... Args>
	struct b_m__<R (T::*)(Args...)> {
		using F = R (T::*)(Args...);

		F f;

		template <typename Fa>
		b_m__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(T& self, Args... args) const {
			return (self.*f)(std::forward<Args>(args)...);
		}

		R operator()(T* self, Args... args) const {
			return this->operator()(*self, std::forward<Args>(args)...);
		}
	};

	template <typename T, typename R, typename... Args>
	struct b_m__<R (T::*)(Args...) const> {
		using F = R (T::*)(Args...) const;

		F f;

		template <typename Fa>
		b_m__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(const T& self, Args... args) const {
			return (self.*f)(std::forward<Args>(args)...);
		}

		R operator()(T const* self, Args... args) const {
			return this->operator()(*self, std::forward<Args>(args)...);
		}
	};

	template <typename F, typename MFP, typename = void>
	struct b_f__;

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...)> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) const> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...)&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) const&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) &&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) const&&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) volatile> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) const volatile> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) volatile&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) const volatile&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) volatile&&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename F, typename R, typename T, typename... Args>
	struct b_f__<F, R (T::*)(Args...) const volatile&&> {
		F f;

		template <typename Fa>
		b_f__(Fa&& fa)
		: f(std::forward<Fa>(fa)) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
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
	struct b__<F, std::enable_if_t<std::is_final_v<meta::remove_cv_ref_t<F>>>> : b_f__<F, decltype(&F::operator())> {
		using base_t = b_f__<F, decltype(&F::operator())>;
		using base_t::operator();

		template <typename Fa>
		b__(Fa&& fa)
		: base_t(std::forward<Fa>(fa)) {
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

	template <typename R, typename... Args>
	struct b__<R(Args...)> {
		using F = R (*)(Args...);
		F f;

		b__(F f_)
		: f(f_) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename R, typename... Args>
	struct b__<R(Args...) noexcept> {
		using F = R (*)(Args...);
		F f;
		b__(F f_)
		: f(f_) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

#if defined(_MSC_VER) && _WIN32 && !defined(_M_X64)
	// VC++ __stdcall bug on x86 machines only
	template <typename R, typename... Args>
	struct b__<__stdcall R(Args...)> {
		using F = R (*)(Args...);
		F f;
		b__(F f_)
		: f(f_) {
		}

		R operator()(Args... args) const {
			return f(std::forward<Args>(args)...);
		}
	};

	template <typename R, typename... Args>
	struct b__<__stdcall R(Args...) noexcept> {
		using F = R()(Args...);
		F f;
		b__(F f_)
		: f(f_) {
		}

		R operator()(Args... args) const noexcept {
			return f(std::forward<Args>(args)...);
		}
	};

#endif // VC++ 32-bit

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
