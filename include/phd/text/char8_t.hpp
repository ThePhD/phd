#pragma once

#ifndef PHD_TEXT_CHAR8_T_HPP
#define PHD_TEXT_CHAR8_T_HPP

namespace phd {

#if !defined(__cpp_char8_t)
	using char8_t = unsigned char;
#endif // char8_t

} // namespace phd

#endif // PHD_TEXT_CHAR8_T_HPP
