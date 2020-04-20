///
/// Character.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CHARACTER_HPP_
#define QUASAR_CHARACTER_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	/// A character in a font.
	///
	class Character final
	{
	public:
		///
		/// Constructor.
		///
		Character() noexcept;
		
		///
		/// Argument constructor.
		///
		/// \param id OpenGL texture handle.
		/// \param w Width.
		/// \param h Height.
		/// \param bx Bearing x offset.
		/// \param by Bearing y offset.
		/// \param advance Offset to next character.
		///
		explicit Character(const unsigned int id, const int w, const int h, const int bx, const int by, const unsigned int advance) noexcept;

		///
		/// Default destructor.
		///
		~Character() noexcept = default;

	public:
		///
		/// OpenGL handle.
		///
		unsigned int m_id;

		///
		/// Width.
		///
		int m_width;

		///
		/// Height.
		///
		int m_height;

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