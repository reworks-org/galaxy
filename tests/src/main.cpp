///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <mimalloc.h>
#include <mimalloc-new-delete.h>

int main(int argsc, char* argsv[])
{
	mi_version();

	testing::InitGoogleTest(&argsc, argsv);

	return RUN_ALL_TESTS();
}
