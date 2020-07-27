///
/// Character.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CHARACTER_HPP_
#define QUASAR_CHARACTER_HPP_

#include <protostar/graphics/Rect.hpp>

#include "qs/core/VertexData.hpp"
#include "qs/texture/Texture.hpp"

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
		/// Allows font to access internals.
		///
		friend class Font;

	public:
		///
		/// Constructor.
		///
		Character();

		///
		/// Argument constructor.
		///
		/// \param bx Bearing x offset.
		/// \param by Bearing y offset.
		/// \param advance Offset to next character.
		///
		explicit Character(const int bx, const int by, const pr::positive_uint auto advance);

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
		[[nodiscard]] const unsigned int get_advance() const noexcept;

		///
		/// Get texture region.
		///
		/// \return Const reference to float rectangle.
		///
		[[nodiscard]] const pr::Rect<float>& get_region() const noexcept;

	private:
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

		///
		/// Texture region.
		///
		pr::Rect<float> m_region;
	};
} // namespace qs

#endif