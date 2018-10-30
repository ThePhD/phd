#pragma once
#ifndef PHD_META_CALLABLE_TRAITS_HPP
#define PHD_META_CALLABLE_TRAITS_HPP

#include <type_traits>

namespace phd::meta {

	namespace meta_detail {

		template <typename F>
		struct check_deducible_signature {
			struct nat {};
			template <class G>
			static auto test(int) -> decltype(&G::operator(), void());
			template <class>
			static auto test(...) -> nat;

			using type = std::is_void<decltype(test<F>(0))>;
		};
	} // namespace meta_detail

	template <typename F>
	struct has_deducible_signature : meta_detail::check_deducible_signature<F>::type {};

	template <typename F>
	inline constexpr bool has_deducible_signature_v = has_deducible_signature<F>::value;

	namespace meta_detail {

		template <bool marked_noexcept, bool has_c_variadic, typename T, typename R, typename... Args>
		struct basic_traits {
		public:
			typedef std::conditional_t<std::is_array_v<R>, std::add_lvalue_reference_t<T>, R> return_type;
			typedef std::remove_cv_t<std::remove_reference_t<T>> object_type;
			typedef T qualified_object_type;

			static constexpr const bool is_noexcept = marked_noexcept;
			static constexpr const bool is_member_function = !std::is_void_v<T>;
			static constexpr const bool has_c_var_arg = has_c_variadic;
		};

		template <typename Signature, bool b = has_deducible_signature<Signature>::value>
		struct fx_traits : basic_traits<false, false, void, void> {};

		// Free Functions
		template <typename R, typename... Args>
		struct fx_traits<R(Args...), false> : basic_traits<false, false, void, R, Args...> {
			typedef R (*function_pointer_type)(Args...);
		};

		template <typename R, typename... Args>
		struct fx_traits<R (*)(Args...), false> : basic_traits<false, false, void, R, Args...> {
			typedef R (*function_pointer_type)(Args...);
		};

		template <typename R, typename... Args>
		struct fx_traits<R(Args..., ...), false> : basic_traits<false, true, void, R, Args...> {
			typedef R (*function_pointer_type)(Args..., ...);
		};

		template <typename R, typename... Args>
		struct fx_traits<R (*)(Args..., ...), false> : basic_traits<false, true, void, R, Args...> {
			typedef R (*function_pointer_type)(Args..., ...);
		};

		// Member Functions
		/* C-Style Variadics */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...), false> : basic_traits<false, false, T, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...);
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...), false> : basic_traits<false, true, T, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...);
		};

		/* Const Volatile */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const, false> : basic_traits<false, false, T const, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const, false> : basic_traits<false, true, T const, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const volatile, false> : basic_traits<false, false, T const volatile, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const volatile;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const volatile, false> : basic_traits<false, true, T const volatile, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const volatile;
		};

		/* Member Function Qualifiers */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...)&, false> : basic_traits<false, false, T&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) &;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...)&, false> : basic_traits<false, true, T&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) &;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const&, false> : basic_traits<false, false, T const&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const&, false> : basic_traits<false, true, T const&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const volatile&, false> : basic_traits<false, false, T const volatile&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const volatile&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const volatile&, false> : basic_traits<false, true, T const volatile&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const volatile&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...)&&, false> : basic_traits<false, false, T&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) &&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...)&&, false> : basic_traits<false, true, T&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) &&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const&&, false> : basic_traits<false, false, T const&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const&&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const&&, false> : basic_traits<false, true, T const&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const&&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const volatile&&, false> : basic_traits<false, false, T const volatile&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const volatile&&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const volatile&&, false> : basic_traits<false, true, T const volatile&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const volatile&&;
		};

		template <typename R, typename... Args>
		struct fx_traits<R(Args...) noexcept, false> : basic_traits<true, false, void, R, Args...> {
			typedef R (*function_pointer_type)(Args...) noexcept;
		};

		template <typename R, typename... Args>
		struct fx_traits<R (*)(Args...) noexcept, false> : basic_traits<true, false, void, R, Args...> {
			typedef R (*function_pointer_type)(Args...) noexcept;
		};

		template <typename R, typename... Args>
		struct fx_traits<R(Args..., ...) noexcept, false> : basic_traits<true, true, void, R, Args...> {
			typedef R (*function_pointer_type)(Args..., ...) noexcept;
		};

		template <typename R, typename... Args>
		struct fx_traits<R (*)(Args..., ...) noexcept, false> : basic_traits<true, true, void, R, Args...> {
			typedef R (*function_pointer_type)(Args..., ...) noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) noexcept;
		};

		/* Const Volatile */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const noexcept, false> : basic_traits<true, false, T const, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const noexcept, false> : basic_traits<true, true, T const, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const volatile noexcept, false> : basic_traits<true, false, T const volatile, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const volatile noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const volatile noexcept, false> : basic_traits<true, true, T const volatile, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const volatile noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) & noexcept, false> : basic_traits<true, false, T&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) & noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) & noexcept, false> : basic_traits<true, true, T&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) & noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const& noexcept, false> : basic_traits<true, false, T const&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const& noexcept, false> : basic_traits<true, true, T const&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const volatile& noexcept, false> : basic_traits<true, false, T const volatile&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const volatile& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const volatile& noexcept, false> : basic_traits<true, true, T const volatile&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const volatile& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) && noexcept, false> : basic_traits<true, false, T&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) && noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) && noexcept, false> : basic_traits<true, true, T&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) && noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const&& noexcept, false> : basic_traits<true, false, T const&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const&& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const&& noexcept, false> : basic_traits<true, true, T const&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const&& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args...) const volatile&& noexcept, false> : basic_traits<true, false, T const volatile&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args...) const volatile&& noexcept;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (T::*)(Args..., ...) const volatile&& noexcept, false> : basic_traits<true, true, T const volatile&&, R, Args...> {
			typedef R (T::*function_pointer_type)(Args..., ...) const volatile&& noexcept;
		};

#if defined(_MSC_VER) && defined(_M_IX86)
		template <typename R, typename... Args>
		struct fx_traits<R __stdcall(Args...), false> : basic_traits<false, false, void, R, Args...> {
			typedef R(__stdcall* function_pointer_type)(Args...);
		};

		template <typename R, typename... Args>
		struct fx_traits<R(__stdcall*)(Args...), false> : basic_traits<false, false, void, R, Args...> {
			typedef R(__stdcall* function_pointer_type)(Args...);
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...), false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...);
		};

		/* Const Volatile */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const volatile, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const volatile;
		};

		/* Member Function Qualifiers */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...)&, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) &;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const&, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const volatile&, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const volatile&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...)&&, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) &&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const&&, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const&&;
		};

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const volatile&&, false> : basic_traits<false, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const volatile&&;
		};

#if defined(SOL_NOEXCEPT_FUNCTION_TYPE) && SOL_NOEXCEPT_FUNCTION_TYPE

		template <typename R, typename... Args>
		struct fx_traits<R __stdcall(Args...) noexcept, false> : basic_traits<true, false, void, R, Args...> {
			typedef R(__stdcall* function_pointer_type)(Args...) noexcept;
		};

		template <typename R, typename... Args>
		struct fx_traits<R(__stdcall*)(Args...) noexcept, false> : basic_traits<true, false, void, R, Args...> {
			typedef R(__stdcall* function_pointer_type)(Args...) noexcept;
		};

		/* __stdcall cannot be applied to functions with varargs*/
		/*template <typename R, typename... Args>
		struct fx_traits<__stdcall R(Args..., ...) noexcept, false> : basic_traits<true, true, void, R, Args...> {
			typedef R(__stdcall* function_pointer_type)(Args..., ...) noexcept;
		};
		template <typename R, typename... Args>
		struct fx_traits<R (__stdcall *)(Args..., ...) noexcept, false> : basic_traits<true, true, void, R, Args...> {
			typedef R(__stdcall* function_pointer_type)(Args..., ...) noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) noexcept;
		};*/

		/* Const Volatile */
		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) const noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) const noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const volatile noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const volatile noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) const volatile noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) const volatile noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) & noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) & noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) & noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) & noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const& noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const& noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) const& noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) const& noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const volatile& noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const volatile& noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) const volatile& noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) const volatile& noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) && noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) && noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) && noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) && noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const&& noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const&& noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) const&& noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) const&& noexcept;
		};*/

		template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args...) const volatile&& noexcept, false> : basic_traits<true, false, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args...) const volatile&& noexcept;
		};

		/* __stdcall does not work with varargs */
		/*template <typename T, typename R, typename... Args>
		struct fx_traits<R (__stdcall T::*)(Args..., ...) const volatile&& noexcept, false> : basic_traits<true, true, T, R, Args...> {
			typedef R (__stdcall T::*function_pointer_type)(Args..., ...) const volatile&& noexcept;
		};*/
#endif // noexcept is part of a function's type
#endif // __stdcall x86 VC++ bug

		template <typename Signature>
		struct fx_traits<Signature, true> : fx_traits<typename fx_traits<decltype(&Signature::operator())>::function_type, false> {};

		template <typename Signature, bool b = std::is_member_object_pointer_v<Signature>>
		struct callable_traits : fx_traits<std::decay_t<Signature>> {
		};

		template <typename R, typename T>
		struct callable_traits<R(T::*), true> {
			typedef std::conditional_t<std::is_array_v<R>, std::add_lvalue_reference_t<T>, R> return_type;
			typedef T object_type;

			static constexpr const bool is_noexcept = std::is_nothrow_assignable_v<R, R>;
			static constexpr const bool is_member_function = false;
			static constexpr const bool has_c_var_arg = false;
		};

	} // namespace meta_detail

	template <typename F>
	struct callable_traits : meta_detail::callable_traits<F> {};
} // namespace phd::meta

#endif // PHD_META_CALLABLE_TRAITS_HPP
