#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Signed int types
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Unsigned int types
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t usize;

// Floating point types
typedef float  f32;
typedef double f64;

// Boolean types
typedef int  b32;
typedef bool b8;

// Properly define static assertion
#if defined(__clang__) || defined(__gcc__)
	#define STATIC_ASSERT _Static_assert
#else
	#define STATIC_ASSERT static_assert
#endif

// Ensure all types are of the correct size.
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

// Platform detection
#if defined(_WIN32)
	#define PLATFORM_WINDOWS 1
	#ifndef _WIN64
		#error "64-bit is required!"
	#endif
#elif defined(__linux__) || defined(__gnu_linux__)
	#define PLATFORM_LINUX 1
#elif defined(__unix__)
	#define PLATFORM_UNIX 1
#elif defined(__APPLE__)
	#define PLATFORM_APPLE 1
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		// iOS Simulator
		#define PLATFORM_IOS 1
		#define PLATFORM_IOS_SIMULATOR 1
	#elif TARGET_OS_IPHONE
		#define PLATFORM_IOS 1
		// iOS device
	#elif TARGET_OS_MAC
		// Other kinds of Mac OS
	#else
		#error "Unknown Apple platform"
	#endif
#else
	#error "Unknown platform!"
#endif

#ifdef MACHINE_EXPORT
	// Exports
	#ifdef _MSC_VER
		#define MACHINEAPI __declspec(dllexport)
	#else
		#define MACHINEAPI __attribute__((visibility("default")))
	#endif
#else
	// Imports
	#ifdef _MSC_VER
		#define MACHINEAPI __declspec(dllimport)
	#else
		#define MACHINEAPI
	#endif
#endif
