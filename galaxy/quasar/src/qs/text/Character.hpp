///
/// Character.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CHARACTER_HPP_
#define QUASAR_CHARACTER_HPP_

#include "qs/renderer/Renderable.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// A character in a font.
	///
	class Character final : public qs::Renderable
	{
	public:
		///
		/// Constructor.
		///
		Character() noexcept;
		
		///
		/// Argument constructor.
		///
		/// \param bx Bearing x offset.
		/// \param by Bearing y offset.
		/// \param advance Offset to next character.
		///
		explicit Character(const int bx, const int by, const unsigned int advance) noexcept;

		///
		/// Destructor.
		///
		~Character() noexcept;

	public:
		///
		/// Bearing X.
		///
		int m_bearingX;

		///
		/// Bearing Y.
		///
		int m_bearingY;

		///
		/// Offset to next character.
		///
		unsigned int m_advance;
	};
}

#endif