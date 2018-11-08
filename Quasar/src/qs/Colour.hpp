///
/// Colour.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_COLOUR_HPP_
#define QUASAR_COLOUR_HPP_

#include <SDL2/SDL_pixels.h>

namespace qs
{
	///
	/// Represents a Colour. Uses clamped floats from 0.0 to 1.0.
	/// Can be constructed from an SDL_Colour.
	///
	struct Colour
	{
		///
		///
		///
		Colour();
		Colour(float r, float g, float b, float a);
		Colour(SDL_Colour colour);

		//operators 
		
		///
		/// Red value.
		///
		float m_r;

		///
		/// Green value.
		///
		float m_g;

		///
		/// Blue value.
		///
		float m_b;

		///
		/// Alpha value.
		///
		float m_a;
	};|
}

#endif