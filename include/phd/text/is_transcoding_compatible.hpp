#pragma once

#ifndef PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP
#define PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP

#include <type_traits>

namespace phd {

	template <typename To, typename From>
	struct is_bitwise_transcoding_compatible : std::false_type {};

	template <typename To, typename From>
	constexpr inline bool is_bitwise_transcoding_compatible_v = is_bitwise_transcoding_compatible<To, From>::value;

	template <typename From, typename To>
	struct is_transcoding_compatible : std::integral_constant<bool, is_bitwise_transcoding_compatible_v<From, To> || std::is_same_v<To, From> || std::is_same_v<typename To::decoding_code_point, typename From::encoding_code_point>> {};

	template <typename From, typename To>
	constexpr bool is_transcoding_compatible_v = is_transcoding_compatible<To, From>::value;

} // namespace phd

#endif // PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP
