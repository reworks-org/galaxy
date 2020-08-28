///
/// RectPackTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/math/RectPack.hpp>

TEST(RectPack, init)
{
	pr::RectPack<int> p;
	p.init(100, 100);

	EXPECT_EQ(p.get_width(), 100);
	EXPECT_EQ(p.get_height(), 100);
}

TEST(RectPack, cant_fit)
{
	pr::RectPack<int> p;
	p.init(100, 100);

	auto res = p.pack(999, 999);

	EXPECT_EQ(res, std::nullopt);
}

TEST(RectPack, fills_width_height)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(100, 100);

	ASSERT_TRUE(res != std::nullopt);

	EXPECT_EQ(res->m_width, 100);
	EXPECT_EQ(res->m_height, 100);
	EXPECT_EQ(p.get_free_space().size(), 0);
}

TEST(RectPack, fills_width)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(100, 10);

	ASSERT_TRUE(res != std::nullopt);
	ASSERT_EQ(p.get_free_space().size(), 1);

	EXPECT_EQ(res->m_width, 100);
	EXPECT_EQ(res->m_height, 10);

	auto space = p.get_free_space()[0];
	EXPECT_EQ(space.m_y, 0 + res->m_height);
	EXPECT_EQ(space.m_height, 100 - res->m_height);
}

TEST(RectPack, fills_height)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(10, 100);

	ASSERT_TRUE(res != std::nullopt);
	ASSERT_EQ(p.get_free_space().size(), 1);

	EXPECT_EQ(res->m_width, 10);
	EXPECT_EQ(res->m_height, 100);

	auto space = p.get_free_space()[0];
	EXPECT_EQ(space.m_x, 0 + res->m_width);
	EXPECT_EQ(space.m_width, 100 - res->m_width);
}

TEST(RectPack, fits)
{
	pr::RectPack<int> p;
	p.init(100, 100);
	auto res = p.pack(10, 10);

	ASSERT_TRUE(res != std::nullopt);
	ASSERT_EQ(p.get_free_space().size(), 2);

	EXPECT_EQ(res->m_width, 10);
	EXPECT_EQ(res->m_height, 10);

	auto spaceA = p.get_free_space()[0];
	auto spaceB = p.get_free_space()[1];

	EXPECT_EQ(spaceA.m_y, 10);
	EXPECT_EQ(spaceA.m_height, 90);

	EXPECT_EQ(spaceB.m_width, 90);
	EXPECT_EQ(spaceB.m_height, res->m_height);
}