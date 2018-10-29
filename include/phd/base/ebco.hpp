#pragma once

#ifndef PHD_BASE_EBCO_HPP
#define PHD_BASE_EBCO_HPP

#include <utility>

namespace phd::base {

	template <typename T, typename = void>
	struct ebco {
		T value;

		ebco() = default;
		ebco(const ebco&) = default;
		ebco(ebco&&) = default;
		ebco& operator=(const ebco&) = default;
		ebco& operator=(ebco&&) = default;
		ebco(const T& v)
		: value(v){};
		ebco(T&& v)
		: value(std::move(v)){};
		ebco& operator=(const T& v) {
			value = v;
		}
		ebco& operator=(T&& v) {
			value = std::move(v);
		};
		template <typename Arg, typename... Args, typename = std::enable_if_t<!std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, ebco> && !std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, T>>>
		ebco(Arg&& arg, Args&&... args)
		: T(std::forward<Arg>(arg), std::forward<Args>(args)...){};

		T& get_value() {
			return value;
		}

		T const& get_value() const {
			return value;
		}
	};

	template <typename T>
	struct ebco<T, std::enable_if_t<std::is_class_v<T> && !std::is_final_v<T>>> : T {
		ebco() = default;
		ebco(const ebco&) = default;
		ebco(ebco&&) = default;
		ebco(const T& v)
		: T(v){};
		ebco(T&& v)
		: T(std::move(v)){};
		template <typename Arg, typename... Args, typename = std::enable_if_t<!std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, ebco> && !std::is_same_v<std::remove_reference_t<std::remove_cv_t<Arg>>, T>>>
		ebco(Arg&& arg, Args&&... args)
		: T(std::forward<Arg>(arg), std::forward<Args>(args)...){};

		ebco& operator=(const ebco&) = default;
		ebco& operator=(ebco&&) = default;
		ebco& operator=(const T& v) {
			static_cast<T&>(*this) = v;
		}
		ebco& operator=(T&& v) {
			static_cast<T&>(*this) = std::move(v);
		};

		T& get_value() {
			return static_cast<T&>(*this);
		}

		T const& get_value() const {
			return static_cast<T const&>(*this);
		}
	};

} // namespace phd::base

#endif // PHD_BASE_EBCO_HPP
