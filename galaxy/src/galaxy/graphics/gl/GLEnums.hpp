///
/// GLEnums.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_GLENUMS_HPP_
#define GALAXY_GRAPHICS_GL_GLENUMS_HPP_

#include <glad/glad.h>

namespace galaxy
{
	///
	/// OpenGL driver data buffer storage flags.
	///
	enum class GLDrawHint : GLenum
	{
		STATIC_DRAW  = GL_STATIC_DRAW,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		STREAM_DRAW  = GL_STREAM_DRAW
	};

	///
	/// Texture wrapping modes.
	//
	enum class GLTextureMode : int
	{
		///
		/// GL_REPEAT.
		///
		REPEAT = GL_REPEAT,

		///
		/// GL_MIRRORED_REPEAT.
		///
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,

		///
		/// GL_CLAMP_TO_EDGE.
		///
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,

		///
		/// GL_CLAMP_TO_BORDER.
		///
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

} // namespace galaxy

#endif
