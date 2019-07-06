#pragma once

#ifndef PHD_OUT_PTR_DETAIL_VOIDPP_OP_HPP
#define PHD_OUT_PTR_DETAIL_VOIDPP_OP_HPP

namespace phd { namespace out_ptr { namespace detail {

	template <typename T, typename P>
	class voidpp_op {
	public:
		operator void**() const noexcept(noexcept(static_cast<P*>(std::declval<T&>()))) {
			const T& self = *static_cast<T const*>(this);
			return static_cast<void**>(static_cast<void*>(static_cast<P*>(self)));
		};
	};

	template <typename T>
	class voidpp_op<T, void*> {};

	template <typename T>
	class voidpp_op<T, const void*> {};

	template <typename T>
	class voidpp_op<T, const volatile void*> {};

}}} // namespace phd::out_ptr::detail

#endif // PHD_OUT_PTR_DETAIL_VOIDPP_OP_HPP
