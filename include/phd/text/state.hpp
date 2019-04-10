#pragma once

#ifndef PHD_TEXT_STATE_HPP
#define PHD_TEXT_STATE_HPP

namespace phd {

	template <typename T>
	struct encoding_state {
		using type = typename T::state;
	};

	template <typename T>
	using encoding_state_t = typename encoding_state<T>::type;

} // namespace phd

#endif // PHD_TEXT_STATE_HPP
