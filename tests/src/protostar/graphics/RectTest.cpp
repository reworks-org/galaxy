///
/// RectTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/graphics/Rect.hpp>

TEST(Rect, Construct)
{
	pr::Rect<int> r;

	EXPECT_EQ(r.m_x, 0);
	EXPECT_EQ(r.m_y, 0);
	EXPECT_EQ(r.m_width, 0);
	EXPECT_EQ(r.m_height, 0);
}

TEST(Rect, ArgConstruct)
{
	pr::Rect<int> r(1, 2, 3, 4);

	EXPECT_EQ(r.m_x, 1);
	EXPECT_EQ(r.m_y, 2);
	EXPECT_EQ(r.m_width, 3);
	EXPECT_EQ(r.m_height, 4);
}

TEST(Rect, ContainsPoint)
{
	pr::Rect<int> r(0, 0, 100, 100);
	
	EXPECT_TRUE(r.contains(10, 10));
}

TEST(Rect, NotContainsPoint)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_FALSE(r.contains(250, 250));
}

TEST(Rect, ContainsRect)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_TRUE(r.contains({10, 10, 10, 10}));
}

TEST(Rect, NotContainsRect)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_FALSE(r.contains({ 250, 250, 250, 250 }));
}

TEST(Rect, Overlap)
{
	pr::Rect<int> a(10, 10, 100, 100);
	pr::Rect<int> b(0, 0, 200, 200);

	EXPECT_TRUE(a.overlaps(b));
}

TEST(Rect, OverlapSame)
{
	pr::Rect<int> a(10, 10, 100, 100);
	pr::Rect<int> b(10, 10, 100, 100);
	
	EXPECT_TRUE(a.overlaps(b));
}

TEST(Rect, NotOverlap)
{
	pr::Rect<int> a(10, 10, 100, 100);
	pr::Rect<int> b(2, 2, 1, 1);

	EXPECT_FALSE(a.overlaps(b));
}