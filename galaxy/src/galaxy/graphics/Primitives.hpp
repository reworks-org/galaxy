///
/// Primitives.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PRIMITIVES_HPP_
#define GALAXY_GRAPHICS_PRIMITIVES_HPP_

#include <glad/glad.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Primitive OpenGL renderable types.
		///
		enum class Primitives : unsigned int
		{
			///
			/// GL_POINTS
			///
			POINT = GL_POINTS,

			///
			/// GL_LINES
			///
			LINE = GL_LINES,

			///
			/// GL_LINE_LOOP
			///
			LINE_LOOP = GL_LINE_LOOP,

			///
			/// GL_LINE_STRIP
			///
			LINE_STRIP = GL_LINE_STRIP,

			///
			/// GL_TRIANGLES
			///
			TRIANGLE = GL_TRIANGLES,

			///
			/// GL_TRIANGLE_STRIP
			///
			TRIANGLE_STRIP = GL_TRIANGLE_STRIP,

			///
			/// GL_TRIANGLE_FAN
			///
			TRIANGLE_FAN = GL_TRIANGLE_FAN
		};

		///
		/// Convert a primitive enum to OpenGL draw type.
		///
		/// \param type Primitive enum.
		///
		/// \return OpenGL glDraw* mode.
		///
		[[nodiscard]] inline constexpr unsigned int primitive_to_gl(const Primitives type)
		{
			return static_cast<unsigned int>(type);
		}
	} // namespace graphics
} // namespace galaxy

#endif