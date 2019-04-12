#pragma once

#ifndef PHD_TEXT_NORMALIZATION_HPP
#define PHD_TEXT_NORMALIZATION_HPP

namespace phd {

	namespace __abi_v0 {
		template <bool __is_compatibility>
		struct __nfc;
		template <bool __is_compatibility>
		struct __nfkd;
	} // namespace __abi_v0

	struct nfc;
	struct nfd;
	struct nfkc;
	struct nfkd;

} // namespace phd

#endif // PHD_TEXT_NORMALIZATION_HPP
