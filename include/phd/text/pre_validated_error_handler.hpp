#pragma once

#ifndef PHD_TEXT_PRE_VALIDATED_ERROR_HANDLER_HPP
#define PHD_TEXT_PRE_VALIDATED_ERROR_HANDLER_HPP

namespace phd {

	struct pre_validated {
		template <typename Encoding, typename Stub>
		Stub operator()(const Encoding&, const Stub& error) const {
			// what have you done?
			return error;
		}
	};

} // namespace phd

#endif // PHD_TEXT_PRE_VALIDATED_ERROR_HANDLER_HPP
