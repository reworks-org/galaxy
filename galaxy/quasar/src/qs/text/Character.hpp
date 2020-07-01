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
		~Character() noexcept = default;

		///
		/// Activate character context.
		///
		void bind() noexcept override;

		///
		/// Deactivate character context.
		///
		void unbind() noexcept override;

		///
		/// Get the offset to the next character.
		///
		/// \return Const unsigned int.
		///
		const unsigned int getAdvance() const noexcept;

		///
		/// Get texture region.
		///
		/// \return Const reference to float rectangle.
		///
		const protostar::Rect<float>& getRegion() const noexcept;

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
		/// Texture region.
		///
		protostar::Rect<float> m_region;
	};
}

#endif