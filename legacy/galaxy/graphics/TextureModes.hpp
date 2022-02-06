///
/// TextureModes.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREMODES_HPP_
#define GALAXY_GRAPHICS_TEXTUREMODES_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Texture wrapping modes.
		//
		enum class TextureModes : short
		{
			///
			/// GL_REPEAT.
			///
			REPEAT,

			///
			/// GL_MIRRORED_REPEAT.
			///
			MIRRORED_REPEAT,

			///
			/// GL_CLAMP_TO_EDGE.
			///
			CLAMP_TO_EDGE,

			///
			/// GL_CLAMP_TO_BORDER.
			///
			CLAMP_TO_BORDER
		};
	} // namespace graphics
} // namespace galaxy

#endif