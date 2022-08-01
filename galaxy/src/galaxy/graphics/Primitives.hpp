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
	} // namespace graphics
} // namespace galaxy

#endif