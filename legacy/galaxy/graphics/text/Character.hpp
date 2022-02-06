///
/// Character.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_CHARACTER_HPP_
#define GALAXY_GRAPHICS_TEXT_CHARACTER_HPP_

#include <glm/vec2.hpp>

namespace galaxy
{
	namespace graphics
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
			/// Move constructor.
			///
			Character(Character&&) noexcept;

			///
			/// Move assignment operator.
			///
			Character& operator=(Character&&) noexcept;

			///
			/// Destructor.
			///
			~Character() noexcept;

		private:
			///
			/// Copy constructor.
			///
			Character(const Character&) = delete;

			///
			/// Copy assignment operator.
			///
			Character& operator=(const Character&) = delete;

		public:
			///
			/// OpenGL texture.
			///
			unsigned int m_texture;

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
	} // namespace graphics
} // namespace galaxy

#endif