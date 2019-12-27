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
	struct Colour final
	{
		///
		/// \brief Default constructor.
		///
		/// Defaults to black, fully opaque.
		///
		Colour() noexcept;

		///
		/// Alternate constructor.
		///
		/// \param r Red colour. 0.0-1.0.
		/// \param g Green colour. 0.0-1.0.
		/// \param b Blue colour. 0.0-1.0.
		/// \param a Alpha. 0.0-1.0.
		///
		Colour(float r, float g, float b, float a) noexcept;

		///
		/// Construct qs::Colour from SDL_Colour.
		///
		/// \param colour Colour to construct from.
		///
		Colour(SDL_Colour colour) noexcept;

		///
		/// Returns the colour in SDL2 colour format.
		///
		/// \return Returns filled out SDL_Colour.
		///
		SDL_Colour asSDL() noexcept;
		
		///
		/// Red value. 0.0f-1.0f.
		///
		float m_red;

		///
		/// Green value. 0.0f-1.0f.
		///
		float m_green;

		///
		/// Blue value. 0.0f-1.0f.
		///
		float m_blue;

		///
		/// Alpha value. 0.0f-1.0f.
		///
		float m_alpha;
	};

	///
	/// qs::Colour == Operator overload.
	///
	inline bool operator==(const qs::Colour& a, const qs::Colour& b) noexcept
	{
		// Equality operator overloading.
		return ((a.m_red == b.m_red) && (a.m_green == b.m_green) && (a.m_blue == b.m_blue) && (a.m_alpha == b.m_alpha));
	}

	///
	/// qs::Colour != Operator overload.
	///
	inline bool operator!=(const qs::Colour& a, const qs::Colour& b) noexcept
	{
		// Not operator overloading.
		return ((a.m_red != b.m_red) || (a.m_green != b.m_green) || (a.m_blue != b.m_blue) || (a.m_alpha != b.m_alpha));
	}
}

#endif