#pragma once

#ifndef PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP
#define PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP

#include <type_traits>

namespace phd {

	template <typename To, typename From>
	using is_transcoding_compatible = std::integral_constant<bool, std::is_same_v<To, From> || std::is_same_v<typename To::decoding_code_point, typename From::encoding_code_point>>;

	template <typename To, typename From>
	constexpr bool is_transcoding_compatible_v = is_transcoding_compatible<To, From>::value;

} // namespace phd

#endif // PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP
