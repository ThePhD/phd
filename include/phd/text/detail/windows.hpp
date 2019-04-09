#pragma once

#ifndef PHD_TEXT_DETAIL_WINDOWS_HPP
#define PHD_TEXT_DETAIL_WINDOWS_HPP

#ifdef _WIN32

#pragma push_macro("NOMINMAX")
#pragma push_macro("WIN32_LEAN_AND_MEAN")
#pragma push_macro("VC_EXTRALEAN")

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN 1

namespace phd { namespace windows {

	extern "C" {
#include <Windows.h>
#include <stringapiset.h>
	}

}} // namespace phd::windows

#pragma pop_macro("VC_EXTRALEAN")
#pragma pop_macro("WIN32_LEAN_AND_MEAN")
#pragma pop_macro("NOMINMAX")

#endif // Windows shit

#endif // PHD_TEXT_DETAIL_WINDOWS_HPP
