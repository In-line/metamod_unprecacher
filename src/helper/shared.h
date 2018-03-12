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

// Here we can put typedefs that shared by many files

#endif // SHARED_H
