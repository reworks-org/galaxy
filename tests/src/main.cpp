///
/// main.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <mimalloc.h>
#include <mimalloc-new-delete.h>

int main(int argc, char* argv[])
{
	mi_version();

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
