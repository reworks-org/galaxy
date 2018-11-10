///
/// TestColour.cpp
/// Tests
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <qs/Colour.hpp>
#include <SDL2/SDL_pixels.h>

#include "gtest/gtest.h"

TEST(Colour, Constructors)
{
	// Default Constructor.
	// Should be black, opaque. i.e. 0.0f, 0.0f, 0.0f, 1.0f.
	qs::Colour defCol;

	ASSERT_EQ(defCol.m_red, 0.0f);
	ASSERT_EQ(defCol.m_green, 0.0f);
	ASSERT_EQ(defCol.m_blue, 0.0f);
	ASSERT_EQ(defCol.m_alpha, 1.0f);

	// Alternate constructor. Should be values constructed with.
	qs::Colour altCol(0.1f, 0.3f, 0.2f, 0.57f);

	ASSERT_EQ(altCol.m_red, 0.1f);
	ASSERT_EQ(altCol.m_green, 0.3f);
	ASSERT_EQ(altCol.m_blue, 0.2f);
	ASSERT_EQ(altCol.m_alpha, 0.57f);

	// Alternate constructor.
	// Should clamp numbers bigger than 1.0f and less than 0.0f to 0.0f-1.0f.
	qs::Colour altColClamp(-1.1f, 12.1f, -2, 4);

	ASSERT_EQ(altColClamp.m_red, 0.0f);
	ASSERT_EQ(altColClamp.m_green, 1.0f);
	ASSERT_EQ(altColClamp.m_blue, 0.0f);
	ASSERT_EQ(altColClamp.m_alpha, 1.0f);

	// SDL_Colour constructor.
	SDL_Colour white{ 255, 255, 255, 255 };
	qs::Colour sdlCol(white);

	ASSERT_EQ(sdlCol.m_red, 1.0f);
	ASSERT_EQ(sdlCol.m_green, 1.0f);
	ASSERT_EQ(sdlCol.m_blue, 1.0f);
	ASSERT_EQ(sdlCol.m_alpha, 1.0f);
}

TEST(Colour, ToSDL)
{
	// Getting an SDL_Colour from qs::Colour.
	// Values should be the same converted across the range.

	SDL_Colour base{ 255, 255, 255, 255 };
	qs::Colour qsCol(1.0f, 1.0f, 1.0f, 1.0f);

	SDL_Colour sdlCol = qsCol.asSDL();

	ASSERT_EQ(sdlCol.r, base.r);
	ASSERT_EQ(sdlCol.g, base.g);
	ASSERT_EQ(sdlCol.b, base.b);
	ASSERT_EQ(sdlCol.a, base.a);
}

TEST(Colour, Operators)
{
	// Test that the operators are doing comparisons properly.
	qs::Colour a;
	qs::Colour b;
	qs::Colour c(1.0f, 0.3f, 0.3f, 1.0f);

	ASSERT_TRUE(a == b);
	ASSERT_TRUE(b != c);
}