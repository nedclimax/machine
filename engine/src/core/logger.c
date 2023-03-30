#include "logger.h"
#include "asserts.h"

#include "platform/platform.h"

// TODO: temporary
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

b8 initialize_logging() {
	// TODO: create log file.
	return true;
}

void shutdown_logging() {
	// TODO: cleanup logging/write queued entries
}

void log_output(loglevel level, const char* fmt, ...) {
	const char* level_strings[] = {
		"[FATAL]: ",
		"[ERROR]: ",
		"[WARN]:  ",
		"[INFO]:  ",
		"[DEBUG]: ",
		"[TRACE]: ",
	};

	b8 is_error = level < LOG_LEVEL_WARN;
	const i32 msg_length = 32000;
	char out_message[msg_length];
	memset(out_message, 0, msg_length);

	va_list list;
	va_start(list, fmt);
	vsnprintf(out_message, msg_length, fmt, list);
	va_end(list);

	char final_out_message[msg_length];
	memset(final_out_message, 0, msg_length);

	sprintf(final_out_message, "%s%s\n", level_strings[level], out_message);

	if (is_error) {
		platform_console_write_error(final_out_message, level);
	} else {
		platform_console_write(final_out_message, level);
	}
}

MACHINEAPI void report_assertion_failure(const char* expr, const char* msg, const char* file, i32 line) {
	log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, line: %d", expr, msg, file, line);
}