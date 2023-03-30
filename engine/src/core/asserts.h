#pragma once

#include "defines.h"

#define ASSERTION_ENABLED 1

#if ASSERTION_ENABLED

#ifdef _MSC_VER
	#include <intrin.h>
	#define debug_break() __debugbreak()
#else
	#define debug_break() __builtin_trap()
#endif

MACHINEAPI void report_assertion_failure(const char* expr, const char* msg, const char* file, i32 line);

#define ASSERT(expr)                                             \
{                                                                \
	if (!(expr)) {                                               \
		report_assertion_failure(#expr, "", __FILE__, __LINE__); \
		debug_break();                                           \
	}                                                            \
}                                                                \

#define ASSERT_MSG(expr, message)                                     \
{                                                                     \
	if (!(expr)) {                                                    \
		report_assertion_failure(#expr, message, __FILE__, __LINE__); \
		debug_break();                                                \
	}                                                                 \
}                                                                     \

#ifdef _DEBUG
	#define ASSERT_DEBUG(expr) ASSERT(expr)
#else
	#define ASSERT_DEBUG(expr)
#endif

#else

#define ASSERT(expr)
#define ASSERT_MSG(expr)
#define ASSERT_DEBUG(expr)

#endif