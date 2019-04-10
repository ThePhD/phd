#pragma once

#ifndef PHD_TEXT_CHAR8_T_HPP
#define PHD_TEXT_CHAR8_T_HPP

#if !defined(PHD_CHAR8_T_TYPE) || PHD_CHAR8_T_TYPE
#if !defined(__cpp_char8_t)
using char8_t = unsigned char;
#endif // char8_t
#endif // Allow someone to define their own char8_t

#endif // PHD_TEXT_CHAR8_T_HPP
