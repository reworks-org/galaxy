///
/// TextureFilters.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREFILTERS_HPP_
#define GALAXY_GRAPHICS_TEXTUREFILTERS_HPP_

#include <glad/glad.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// OpenGL Texture Filters.
		///
		enum class Filters : GLint
		{
			///
			/// Produces blocky/pixellated texture patterns.
			/// Think 8-bit.
			///
			MIN_BILINEAR_MIPMAP = GL_NEAREST_MIPMAP_NEAREST,

			///
			/// Trilinear filtering for textures.
			///
			MIN_TRILINEAR_MIPMAP = GL_LINEAR_MIPMAP_LINEAR,

			///
			/// Produces blocky/pixellated texture patterns.
			/// Think 8-bit.
			///
			MAG_NEAREST = GL_NEAREST,

			///
			/// Attempts to smooth texture pattern. Less visible pixels.
			///
			MAG_LINEAR = GL_LINEAR
		};
	} // namespace graphics
} // namespace galaxy

#endif