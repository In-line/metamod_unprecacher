#ifndef EXTERNAL_API_H
#define EXTERNAL_API_H

// This is some "hacky" way to avoid multiple "zero as null" errors in metamod and hlsdk headers
#ifndef CXXTEST_RUNNING // cxxtest generated runner.cpp file for some reason creates error.
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
		#include <extdll.h>
		#include <meta_api.h>
	#pragma GCC diagnostic pop
#endif

// SDK defines its own max/min, but it creates conflicts.
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
