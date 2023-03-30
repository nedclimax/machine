#pragma once

#include "defines.h"

typedef struct {
	void* internal_state;
} platform_state;

MACHINEAPI b8 platform_startup(
	platform_state* plat_state,
	const char* app_name,
	i32 x, i32 y,
	i32 width,
	i32 height
);

// TODO: remove MACHINEAPI for platform functions

MACHINEAPI b8 platform_pump_messages(platform_state* plat_state);

MACHINEAPI void platform_shutdown(platform_state* plat_state);

MACHINEAPI void* platform_allocate(u64 size, b8 aligned);
MACHINEAPI void  platform_free(void* block, b8 aligned);
MACHINEAPI void* platform_zero_memory(void* block, u64 size);
MACHINEAPI void* platform_copy_memory(void* dest, const void* source, u64 size);
MACHINEAPI void* platform_set_memory(void* block, i32 value, u64 size);

MACHINEAPI void platform_console_write(const char* message, u8 color);
MACHINEAPI void platform_console_write_error(const char* message, u8 color);

MACHINEAPI f64 platform_get_absolute_time();

MACHINEAPI void platform_sleep(u64 ms);