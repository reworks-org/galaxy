///
/// ManagerTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/utility/UniqueID.hpp>

using TestUID = protostar::UniqueID<struct TestSpec_>;

class TestA {};
class TestB {};

TEST(UniqueID, Gen)
{
	constexpr auto a = TestUID::get<TestA>();
	constexpr auto b = TestUID::get<TestB>();
}