#pragma once

#ifdef _MSC_VER
	#include <intrin.h>
	#define debug_break() __debugbreak()
#else
	#define debug_break() __builtin_trap()
#endif

void report_assertion_failure(const char* expr, const char* msg, const char* file, i32 line);

#define ASSERT(expr)      \
	{                     \
		if (!(expr)) {                      \
			report_assertion_failure(expr); \
		} \
	}\