///
/// ProtectedArithmeticTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/async/ProtectedArithmetic.hpp>

TEST(ProtectedArithmetic, construct)
{
	pr::ProtectedAirthmetic<int> pa(2);

	auto res = pa.get();
	EXPECT_EQ(res, 2);
}

TEST(ProtectedArithmetic, set_get)
{
	pr::ProtectedAirthmetic<int> pa;
	pa.set(5);

	auto res = pa.get();
	EXPECT_EQ(res, 5);
}