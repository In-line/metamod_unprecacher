#ifndef EXTERNAL_API_H // We do not need to include this file twice
#define EXTERNAL_API_H

// This is some "hacky" way to avoid multiple "zero as null" errors in metamod and hlsdk headers
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#include <extdll.h>
#include <meta_api.h>

#ifndef CXXTEST_RUNNING // If generated runner.cpp for some reason appears error.
#pragma GCC diagnostic error "-Wzero-as-null-pointer-constant"
#endif
// SDK defines its own max/min, but it makes conflicts.
#if defined (max) || (min)
#include <algorithm>

#ifdef max
#undef max
using std::max;
#endif

#ifdef min
#undef min
using std::min;
#endif

#endif

#endif // EXTERNAL_API_H
