///
/// RectTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/math/Rect.hpp>

TEST(Rect, ContainsPoint)
{
	galaxy::iRect a {0, 0, 100, 100};

	ASSERT_TRUE(a.contains(10, 10));
}

TEST(Rect, NotContainsPoint)
{
	galaxy::iRect a {0, 0, 100, 100};

	ASSERT_FALSE(a.contains(1000, 1000));
}

TEST(Rect, ContainsRect)
{
	galaxy::iRect a {0, 0, 100, 100};
	galaxy::iRect b {10, 10, 10, 10};

	ASSERT_TRUE(a.contains(b));
}

TEST(Rect, NotContainsRect)
{
	galaxy::iRect a {0, 0, 100, 100};
	galaxy::iRect b {1000, 1000, 10, 10};

	ASSERT_FALSE(a.contains(b));
}

TEST(Rect, Overlaps)
{
	galaxy::iRect a {10, 10, 100, 100};
	galaxy::iRect b {0, 0, 100, 100};

	ASSERT_TRUE(a.overlaps(b));
}

TEST(Rect, NotOverlaps)
{
	galaxy::iRect a {100, 100, 1, 1};
	galaxy::iRect b {0, 0, 10, 10};

	ASSERT_FALSE(a.overlaps(b));
}

TEST(Rect, Intersects)
{
	galaxy::iRect a {10, 10, 100, 100};
	galaxy::iRect b {0, 0, 100, 100};

	ASSERT_TRUE(a.intersects(b));
}

TEST(Rect, NotIntersects)
{
	galaxy::iRect a {100, 100, 1, 1};
	galaxy::iRect b {0, 0, 10, 10};

	ASSERT_FALSE(a.intersects(b));
}

TEST(Rect, SetTopLeft)
{
	galaxy::iRect a {0, 0, 10, 10};
	a.set_top_left({10, 10});

	ASSERT_EQ(a.x, 10);
	ASSERT_EQ(a.y, 10);
}

TEST(Rect, GetRight)
{
	galaxy::iRect a {1, 2, 3, 4};

	ASSERT_EQ(a.get_right(), 4);
}

TEST(Rect, GetBottom)
{
	galaxy::iRect a {1, 2, 3, 4};

	ASSERT_EQ(a.get_bottom(), 6);
}

TEST(Rect, GetTopLeft)
{
	galaxy::iRect a {1, 2, 3, 4};

	const auto top_left = a.get_top_left();
	ASSERT_EQ(top_left.x, 1);
	ASSERT_EQ(top_left.y, 2);
}

TEST(Rect, GetCenter)
{
	galaxy::iRect a {2, 4, 6, 8};

	const auto center = a.get_center();
	ASSERT_EQ(center.x, 4);
	ASSERT_EQ(center.y, 6);
}

TEST(Rect, GetSize)
{
	galaxy::iRect a {2, 4, 6, 8};

	const auto size = a.get_size();
	ASSERT_EQ(size.x, 6);
	ASSERT_EQ(size.y, 8);
}
