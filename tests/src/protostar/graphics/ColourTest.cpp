///
/// ColourTest.cpp
/// tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>
#include <protostar/graphics/Colour.hpp>

TEST(Colour, ConstructRGB)
{
	pr::Colour colour(255, 126, 132);

	EXPECT_EQ(colour.m_red, 255);
	EXPECT_EQ(colour.m_green, 126);
	EXPECT_EQ(colour.m_blue, 132);
	EXPECT_EQ(colour.m_alpha, 255);
}

TEST(Colour, ConstructRGBA)
{
	pr::Colour colour(139, 226, 232, 178);

	EXPECT_EQ(colour.m_red, 139);
	EXPECT_EQ(colour.m_green, 226);
	EXPECT_EQ(colour.m_blue, 232);
	EXPECT_EQ(colour.m_alpha, 178);
}

TEST(Colour, Normalize)
{
	pr::Colour colour(139, 226, 232, 178);
	auto norm = colour.getNormalized();
	
	EXPECT_EQ(norm[0], static_cast<float>(colour.m_red) / static_cast<float>(0xFF));
	EXPECT_EQ(norm[1], static_cast<float>(colour.m_green) / static_cast<float>(0xFF));
	EXPECT_EQ(norm[2], static_cast<float>(colour.m_blue) / static_cast<float>(0xFF));
	EXPECT_EQ(norm[3], static_cast<float>(colour.m_alpha) / static_cast<float>(0xFF));
}

TEST(Colour, NormalizeEdge)
{
	pr::Colour colour(255, 0, 255, 0);
	auto norm = colour.getNormalized();

	EXPECT_EQ(norm[0], 1.0f);
	EXPECT_EQ(norm[1], 0.0f);
	EXPECT_EQ(norm[2], 1.0f);
	EXPECT_EQ(norm[3], 0.0f);
}

TEST(Colour, GetAs)
{
	pr::Colour colour(255, 0, 255, 0);
	auto res = colour.getAs<float>();

	auto val = std::is_same<decltype(res[0]), float>::value;
	EXPECT_TRUE(val);
	
	val = std::is_same<decltype(res[1]), float>::value;
	EXPECT_TRUE(val);

	val = std::is_same<decltype(res[2]), float>::value;
	EXPECT_TRUE(val);

	val = std::is_same<decltype(res[3]), float>::value;
	EXPECT_TRUE(val);
}