///
/// Character.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CHARACTER_HPP_
#define QUASAR_CHARACTER_HPP_

#include <protostar/math/Rect.hpp>

#include "qs/texture/Texture.hpp"
#include "qs/core/VertexData.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// A character in a font.
	///
	class Character final : public qs::VertexData, public qs::Texture
	{
		///
		/// Allows font to access interior.
		///
		friend class Font;
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
		virtual ~Character() noexcept = default;

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

	private:
		///
		/// Texture region on a fontmap.
		///
		protostar::Rect<float> m_region;
	};
}

#endif