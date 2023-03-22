#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
	LOG_FATAL("A test message: %f", 3.14f);
	LOG_ERROR("A test message: %f", 3.14f);
	LOG_WARN("A test message: %f", 3.14f);
	LOG_INFO("A test message: %f", 3.14f);
	LOG_DEBUG("A test message: %f", 3.14f);
	LOG_TRACE("A test message: %f", 3.14f);

	ASSERT(0 == 1);
	return 0;
}