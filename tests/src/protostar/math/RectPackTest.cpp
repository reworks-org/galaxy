///
/// RectPackTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/math/RectPack.hpp>

TEST(RectPack, Init)
{
	pr::RectPack<int> p;
	p.init(100, 100);

	EXPECT_EQ(p.getWidth(), 100);
	EXPECT_EQ(p.getHeight(), 100);
}

TEST(RectPack, CantFit)
{
	pr::RectPack<int> p;
	p.init(100, 100);

	auto res = p.pack(999, 999);

	EXPECT_EQ(res, std::nullopt);
}

TEST(RectPack, FillsWidthHeight)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(100, 100);

	ASSERT_TRUE(res != std::nullopt);

	EXPECT_EQ(res->m_width, 100);
	EXPECT_EQ(res->m_height, 100);
	EXPECT_EQ(p.getRects().size(), 0);
}

TEST(RectPack, FillsWidth)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(100, 10);

	ASSERT_TRUE(res != std::nullopt);
	ASSERT_EQ(p.getRects().size(), 1);

	EXPECT_EQ(res->m_width, 100);
	EXPECT_EQ(res->m_height, 10);

	auto space = p.getRects()[0];
	EXPECT_EQ(space.m_y, 100 + res->m_height);
	EXPECT_EQ(space.m_height, 100 - res->m_height);
}

TEST(RectPack, FillsHeight)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(10, 100);

	ASSERT_TRUE(res != std::nullopt);
	ASSERT_EQ(p.getRects().size(), 1);

	EXPECT_EQ(res->m_width, 10);
	EXPECT_EQ(res->m_height, 100);

	auto space = p.getRects()[0];
	EXPECT_EQ(space.m_x, 100 + res->m_width);
	EXPECT_EQ(space.m_width, 100 - res->m_width);
}

TEST(RectPack, Fits)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(10, 10);

	ASSERT_TRUE(res != std::nullopt);
	ASSERT_EQ(p.getRects().size(), 2);

	EXPECT_EQ(res->m_width, 10);
	EXPECT_EQ(res->m_height, 10);

	auto spaceA = p.getRects()[0];
	auto spaceB = p.getRects()[1];

	EXPECT_EQ(spaceA.m_y, 100 + res->m_height);
	EXPECT_EQ(spaceA.m_height, 100 - res->m_height);

	EXPECT_EQ(spaceB.m_width, 100 - res->m_width);
	EXPECT_EQ(spaceB.m_height, res->m_height);
}