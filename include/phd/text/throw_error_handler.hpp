#pragma once

#ifndef PHD_TEXT_THROW_ERROR_HANDLER_HPP
#define PHD_TEXT_THROW_ERROR_HANDLER_HPP

namespace phd {

	struct throw_encode_errors {
		template <typename Encoding, typename Stub>
		Stub operator()(const Encoding&, const Stub& error) const {
			throw error;
		}
	};

	struct throw_decode_errors {
		template <typename Encoding, typename Stub>
		Stub operator()(const Encoding&, const Stub& error) const {
			throw error;
		}
	};

} // namespace phd

#endif // PHD_TEXT_THROW_ERROR_HANDLER_HPP
