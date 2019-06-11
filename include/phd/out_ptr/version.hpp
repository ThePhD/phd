#pragma once

#ifndef PHD_OUT_PTR_OUT_PTR_VERSION_VERSION_HPP
#define PHD_OUT_PTR_OUT_PTR_VERSION_VERSION_HPP

#include <phd/version/version.hpp>

#if defined(PHD_DEBUG) && PHD_DEBUG == 1
#ifndef PHD_OUT_PTR_DEBUG
#define PHD_OUT_PTR_DEBUG 1
#endif // PHD_OUT_PTR_DEBUG
#endif // PHD_DEBUG, turn on other debug if not specified

#if defined(PHD_OUT_PTR_DEBUG) && PHD_OUT_PTR_DEBUG == 1

#ifndef PHD_OUT_PTR_CLEVER_SANITY_CHECK
#define PHD_OUT_PTR_CLEVER_SANITY_CHECK 1
#endif // PHD_OUT_PTR_CLEVER_SANITY_CHECK

#endif // PHD_OUT_PTR_DEBUG, force any undefined checks to be on

#endif // PHD_OUT_PTR_OUT_PTR_VERSION_VERSION_HPP
