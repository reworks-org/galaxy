///
/// ColourTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/graphics/Colour.hpp>

TEST(Colour, construct_rgb)
{
	pr::Colour colour(255, 126, 132);

	EXPECT_EQ(colour.m_red, 255);
	EXPECT_EQ(colour.m_green, 126);
	EXPECT_EQ(colour.m_blue, 132);
	EXPECT_EQ(colour.m_alpha, 255);
}

TEST(Colour, construct_rgba)
{
	pr::Colour colour(139, 226, 232, 178);

	EXPECT_EQ(colour.m_red, 139);
	EXPECT_EQ(colour.m_green, 226);
	EXPECT_EQ(colour.m_blue, 232);
	EXPECT_EQ(colour.m_alpha, 178);
}

TEST(Colour, get_as_normalized)
{
	pr::Colour colour(139, 226, 232, 178);
	auto norm = colour.get_normalized();

	EXPECT_EQ(norm[0], static_cast<float>(colour.m_red) / static_cast<float>(0xFF));
	EXPECT_EQ(norm[1], static_cast<float>(colour.m_green) / static_cast<float>(0xFF));
	EXPECT_EQ(norm[2], static_cast<float>(colour.m_blue) / static_cast<float>(0xFF));
	EXPECT_EQ(norm[3], static_cast<float>(colour.m_alpha) / static_cast<float>(0xFF));
}

TEST(Colour, normalize_bva)
{
	pr::Colour colour(255, 0, 255, 0);
	auto norm = colour.get_normalized();

	EXPECT_EQ(norm[0], 1.0f);
	EXPECT_EQ(norm[1], 0.0f);
	EXPECT_EQ(norm[2], 1.0f);
	EXPECT_EQ(norm[3], 0.0f);
}

TEST(Colour, get_as)
{
	pr::Colour colour(255, 0, 255, 0);
	auto res = colour.get_as<float>();

	const bool val = std::is_same<std::array<float, 4>, decltype(res)>::value;
	ASSERT_TRUE(val);

	EXPECT_EQ(res[0], 255.0f);
	EXPECT_EQ(res[1], 0.0f);
	EXPECT_EQ(res[2], 255.0f);
	EXPECT_EQ(res[3], 0.0f);
}