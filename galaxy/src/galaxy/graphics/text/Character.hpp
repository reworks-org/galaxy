///
/// Character.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_CHARACTER_HPP_
#define GALAXY_GRAPHICS_TEXT_CHARACTER_HPP_

#include <glm/vec2.hpp>

#include "galaxy/graphics/shapes/Rect.hpp"

namespace galaxy
{
	namespace graphics
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

			///
			/// Texture region.
			///
			graphics::Rect<float> m_region;
		};
	} // namespace graphics
} // namespace galaxy

#endif