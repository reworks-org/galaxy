///
/// Character.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CHARACTER_HPP_
#define QUASAR_CHARACTER_HPP_

#include <glm/vec2.hpp>

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
		friend class Font;
		friend class Text;

	public:
		///
		/// Constructor.
		///
		Character();

		///
		/// Copy constructor.
		///
		Character(const Character&) = delete;

		///
		/// Move constructor.
		///
		Character(Character&&);

		///
		/// Copy assignment operator.
		///
		Character& operator=(const Character&) = delete;

		///
		/// Move assignment operator.
		///
		Character& operator=(Character&&);

		///
		/// Destructor.
		///
		~Character();

	private:
		///
		/// OpenGL texture.
		///
		unsigned int m_gl_texture;

		///
		/// Character size.
		///
		glm::ivec2 m_size;

		///
		/// Character bearing.
		///
		glm::ivec2 m_bearing;

		///
		/// Character advance.
		///
		unsigned int m_advance;
	};
} // namespace qs

#endif