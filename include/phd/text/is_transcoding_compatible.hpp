#pragma once

#ifndef PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP
#define PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP

#include <phd/text/is_unicode_code_point.hpp>
#include <phd/text/code_point.hpp>
#include <phd/text/code_unit.hpp>

namespace phd {

	template <typename __To, typename __From>
	struct is_bitwise_transcoding_compatible : std::false_type {};

	template <typename __To, typename __From>
	constexpr inline bool is_bitwise_transcoding_compatible_v = is_bitwise_transcoding_compatible<__To, __From>::value;

	template <typename __From, typename __To>
	struct is_transcoding_compatible : std::integral_constant<bool, is_bitwise_transcoding_compatible_v<__From, __To> || std::is_same_v<__To, __From> || std::is_same_v<encoding_code_point_t<__To>, encoding_code_point_t<__From>>> {};

	template <typename __From, typename __To>
	constexpr bool is_transcoding_compatible_v = is_transcoding_compatible<__To, __From>::value;

} // namespace phd

#endif // PHD_TEXT_IS_TRANSCODING_COMPATIBLE_HPP
