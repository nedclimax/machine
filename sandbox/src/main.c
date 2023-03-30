#include <core/logger.h>
#include <core/asserts.h>

// TODO: test
#include <platform/platform.h>

#include <stdio.h>

int main(void) {
	LOG_FATAL("A test message: %f", 3.14f);
	LOG_ERROR("A test message: %f", 3.14f);
	LOG_WARN("A test message: %f", 3.14f);
	LOG_INFO("A test message: %f", 3.14f);
	LOG_DEBUG("A test message: %f", 3.14f);
	LOG_TRACE("A test message: %f", 3.14f);

	platform_state state;
	if (platform_startup(&state, "Machine Testing", 100, 100, 1280, 720)) {
		while (true) {
			platform_pump_messages(&state);
		}
	}
	platform_shutdown(&state);

	getchar();
	return 0;
}