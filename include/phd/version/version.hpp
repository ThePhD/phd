#pragma once

#ifndef PHD_VERSION_VERSION_HPP
#define PHD_VERSION_VERSION_HPP

#ifdef _MSC_VER

#if defined(_DEBUG) && !defined(NDEBUG)
#ifndef PHD_DEBUG
#define PHD_DEBUG 1
#endif // General Debug Macro
#endif // Not Debug && VC++ debug macros

#elif defined(__GNUC__) || defined(__clang__)

#if !defined(NDEBUG) && !defined(__OPTIMIZE__)
#ifndef PHD_DEBUG
#define PHD_DEBUG 1
#endif // General Debug Macro
#endif // Not Debug && g++ optimizer flag

#endif // VC++ versus clang/g++

#ifndef PHD_CTAD
#if defined(__cpp_deduction_guides)
#define PHD_CTAD 1
#elif defined(_MSC_VER) && MSC_VER >= 1914
#define PHD_CTAD 1
#elif defined(__GNUC__) && (__GNUC__ >= 7) && __cplusplus >= 201703L
#define PHD_CTAD 1
#elif defined(__clang__) && (__clang__ >= 5) && __cplusplus >= 201703L
#define PHD_CTAD 1
#else
#define PHD_CTAD 0
#endif // Detect CTAD
#endif // Detect CTAD

#endif // PHD_VERSION_VERSION_HPP
