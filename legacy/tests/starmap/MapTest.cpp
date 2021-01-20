///
/// MapTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <starmap/Map.hpp>

TEST(Starmap, DesertExampleMap)
{
	starmap::Map map;
	auto res = map.load("../../../../tests/tiled/desert.json");
	ASSERT_TRUE(res);

	res = map.parse();
	ASSERT_TRUE(res);
}