

#pragma once

#ifndef PHD_OUT_PTR_DETAIL_IS_SPECIALIZATION_OF_HPP
#define PHD_OUT_PTR_DETAIL_IS_SPECIALIZATION_OF_HPP

#include <type_traits>

namespace phd {
namespace out_ptr {
namespace detail {

	namespace meta_detail {
		template <typename T, template <typename...> class Templ>
		struct is_specialization_of : std::false_type {
		};
		template <typename... T, template <typename...> class Templ>
		struct is_specialization_of<Templ<T...>, Templ> : std::true_type {
		};

	} // namespace meta_detail

	template <typename T, template <typename...> class Templ>
	using is_specialization_of = meta_detail::is_specialization_of<typename std::remove_cv<T>::type, Templ>;

}}} // namespace phd::out_ptr::detail

#endif // PHD_OUT_PTR_DETAIL_IS_SPECIALIZATION_OF_HPP
