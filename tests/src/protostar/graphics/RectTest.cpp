///
/// RectTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/graphics/Rect.hpp>

TEST(Rect, default_construct)
{
	pr::Rect<int> r;

	EXPECT_EQ(r.m_x, 0);
	EXPECT_EQ(r.m_y, 0);
	EXPECT_EQ(r.m_width, 0);
	EXPECT_EQ(r.m_height, 0);
}

TEST(Rect, arg_construct)
{
	pr::Rect<int> r(1, 2, 3, 4);

	EXPECT_EQ(r.m_x, 1);
	EXPECT_EQ(r.m_y, 2);
	EXPECT_EQ(r.m_width, 3);
	EXPECT_EQ(r.m_height, 4);
}

TEST(Rect, contains_point)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_TRUE(r.contains(10, 10));
}

TEST(Rect, not_contains_point)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_FALSE(r.contains(250, 250));
}

TEST(Rect, contains_rect)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_TRUE(r.contains({10, 10, 10, 10}));
}

TEST(Rect, not_contains_rect)
{
	pr::Rect<int> r(0, 0, 100, 100);

	EXPECT_FALSE(r.contains({250, 250, 250, 250}));
}

TEST(Rect, overlaps)
{
	pr::Rect<int> a(10, 10, 100, 100);
	pr::Rect<int> b(0, 0, 200, 200);

	EXPECT_TRUE(a.overlaps(b));
}

TEST(Rect, overlap_same_sized_spaces)
{
	pr::Rect<int> a(10, 10, 100, 100);
	pr::Rect<int> b(10, 10, 100, 100);

	EXPECT_TRUE(a.overlaps(b));
}

TEST(Rect, no_overlap)
{
	pr::Rect<int> a(10, 10, 100, 100);
	pr::Rect<int> b(2, 2, 1, 1);

	EXPECT_FALSE(a.overlaps(b));
}