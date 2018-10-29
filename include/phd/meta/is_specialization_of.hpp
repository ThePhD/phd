#pragma once

#ifndef PHD_META_IS_SPECIALIZATION_OF_HPP
#define PHD_META_IS_SPECIALIZATION_OF_HPP

#include <type_traits>

namespace phd::meta {

	namespace meta_detail {
		template <typename T, template <typename...> class Templ>
		struct is_specialization_of : std::false_type {};
		template <typename... T, template <typename...> class Templ>
		struct is_specialization_of<Templ<T...>, Templ> : std::true_type {};
	} // namespace meta_detail

	template <typename T, template <typename...> class Templ>
	using is_specialization_of = meta_detail::is_specialization_of<std::remove_cv_t<T>, Templ>;

} // namespace phd::meta

#endif // PHD_META_IS_SPECIALIZATION_OF_HPP