///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <mimalloc.h>
#include <mimalloc-new-delete.h>

#include "tests/LoggingTests.hpp"
#include "tests/PlatformTests.hpp"

#include "TestConfig.hpp"

int main(int argsc, char* argsv[])
{
	mi_version();

	if (ENABLE_PLATFORM_TESTS)
	{
		platform_tests();
	}

	if (ENABLE_LOGGING_TESTS)
	{
		logging_tests();
	}

	return 0;
}
