///
/// Enums.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_ENUMS_HPP_
#define GALAXY_GRAPHICS_GL_ENUMS_HPP_

#include <glad/glad.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// OpenGL driver data buffer storage flags.
		///
		enum class DrawHint : GLenum
		{
			STATIC_DRAW  = GL_STATIC_DRAW,
			DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
			STREAM_DRAW  = GL_STREAM_DRAW
		};

		///
		/// Stores the location of shader attribute binding point for the VAO.
		///
		enum class AttributeBinding : unsigned int
		{
			///
			/// Position data.
			///
			POSITION_POINT = 0u,

			///
			/// Texel data.
			///
			TEXEL_POINT = 1u,

			///
			/// Instance offset data.
			///
			OFFSET_POINT = 2u
		};

		///
		/// Stores the location of a buffer's binding point for the VAO.
		///
		enum class BufferBinding : unsigned int
		{
			///
			/// Normal VBO.
			///
			VERTEX_BUFFER_POINT = 0,

			///
			/// Instance offsets.
			///
			INSTANCE_BUFFER_POINT = 1
		};

		///
		/// Texture wrapping modes.
		//
		enum class TextureMode : int
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

		///
		/// Mipmap filtering.
		///
		enum class TextureFilter : short
		{
			///
			/// Nearest-neighbour.
			///
			NEAREST,

			///
			/// Trilinear.
			///
			TRILINEAR
		};
	} // namespace graphics
} // namespace galaxy

#endif
