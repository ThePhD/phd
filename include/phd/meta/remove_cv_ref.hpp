#pragma once
#ifndef PHD_META_REMOVE_CV_REF_HPP
#define PHD_META_REMOVE_CV_REF_HPP

#include <type_traits>

namespace phd::meta {
	template <typename T>
	using remove_cv_ref = std::remove_cv<std::remove_reference_t<T>>;

	template <typename T>
	using remove_cv_ref_t = typename remove_cv_ref<T>::type;
} // namespace phd::meta

#endif // PHD_META_REMOVE_CV_REF_HPP
