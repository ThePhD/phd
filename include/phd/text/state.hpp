#pragma once

#ifndef PHD_TEXT_STATE_HPP
#define PHD_TEXT_STATE_HPP

namespace phd {

	template <typename __T>
	struct encoding_state {
		using type = typename __T::state;
	};

	template <typename __T>
	using encoding_state_t = typename encoding_state<__T>::type;

} // namespace phd

#endif // PHD_TEXT_STATE_HPP
