///
/// ProtectedArithmeticTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/async/ProtectedArithmetic.hpp>

TEST(ProtectedArithmetic, SetGet)
{
	pr::ProtectedAirthmetic<int> pa;
	pa.set(5);

	auto res = pa.get();
	EXPECT_EQ(res, 5);
}

TEST(ProtectedArithmetic, Construct)
{
	pr::ProtectedAirthmetic<int> pa(2);

	auto res = pa.get();
	EXPECT_EQ(res, 2);
}