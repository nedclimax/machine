#include "logger.h"

// TODO: temporary
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

b8 initialze_logging() {
	// TODO: create log file.
	return true;
}

void shutdown_logging() {
	// TODO: cleanup logging/write queued entries
}

MACHINEAPI void log_output(loglevel level, const char* fmt, ...) {
	const char* level_strings[] = {
		"[FATAL]: ",
		"[ERROR]: ",
		"[WARN]:  ",
		"[INFO]:  ",
		"[DEBUG]: ",
		"[TRACE]: ",
	};

	//b8 is_error = level < LOG_LEVEL_WARN;

	char out_message[32000];
	memset(out_message, 0, 32000);

	va_list list;
	va_start(list, fmt);
	vsnprintf(out_message, 32000, fmt, list);
	va_end(list);

	char final_out_message[32000];
	memset(final_out_message, 0, 32000);

	sprintf(final_out_message, "%s%s\n", level_strings[level], out_message);

	// TODO: platform-specific output
	puts(final_out_message);
}