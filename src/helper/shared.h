#pragma once
#ifndef SHARED_H
#define SHARED_H

#define FNAME std::string(__func__)

#ifndef _MSC_VER
	#define likely(x)      __builtin_expect(!!(x), 1)
	#define unlikely(x)    __builtin_expect(!!(x), 0)
	#define HOT            __attribute__((hot))
	#define COLD           __attribute__((cold))
#else
	#define likely(x)      (x)
	#define unlikely(x)    (x)
	#define HOT
	#define COLD
#endif

#if __has_include(<boost/smart_ptr/local_shared_ptr.hpp>)
	#include <boost/smart_ptr/local_shared_ptr.hpp>
	#include <boost/smart_ptr/make_local_shared.hpp>

	namespace up
	{
		template<typename T>
		using shared_ptr = boost::local_shared_ptr<T>;

		template<class T, class... Args>
		inline auto make_shared(Args&&... args)
		{
			return boost::make_local_shared<T>(std::forward<Args>(args)...);
		}
	}
#else
	#include <memory>

	namespace up
	{
		using std::shared_ptr;
		using std::make_shared;
	}
#endif

// Here we can put typedefs that shared by many files

#endif // SHARED_H
