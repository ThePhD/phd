#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_DETAIL_VOIDPP_OP_HPP
#define PHD_OUT_PTR_OUT_PTR_DETAIL_VOIDPP_OP_HPP

namespace phd::detail {

	template <typename T, typename P>
	struct voidpp_op {
		operator void*&() noexcept(noexcept(static_cast<P*>(std::declval<T&>()))) {
			T& self = *static_cast<T*>(static_cast<void*>(this));
			return *static_cast<void**>(static_cast<void*>(static_cast<P*>(self)));
		};
		operator void**() noexcept(noexcept(static_cast<P*>(std::declval<T&>()))) {
			T& self = *static_cast<T*>(static_cast<void*>(this));
			return static_cast<void**>(static_cast<void*>(static_cast<P*>(self)));
		};
	};

	template <typename T>
	struct voidpp_op<T, void*> {};

	template <typename T>
	struct voidpp_op<T, const void*> {};

	template <typename T>
	struct voidpp_op<T, const volatile void*> {};

} // namespace phd::detail

#endif // PHD_OUT_PTR_OUT_PTR_DETAIL_VOIDPP_OP_HPP
