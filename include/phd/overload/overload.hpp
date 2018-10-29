#pragma once
#ifndef PHD_OVERLOAD_OVERLOAD_HPP
#define PHD_OVERLOAD_OVERLOAD_HPP

#include <phd/meta/remove_cv_ref.hpp>
#include <phd/overload/overload_detail.hpp>

namespace phd {

	template <typename... Fs>
	auto overload(Fs&&... fs) {
		return overload_detail::t_<meta::remove_cv_ref_t<Fs>...>(std::forward<Fs>(fs)...);
	}

} // namespace phd

#endif // PHD_OVERLOAD_OVERLOAD_HPP
