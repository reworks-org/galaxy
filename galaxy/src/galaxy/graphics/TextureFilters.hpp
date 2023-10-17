///
/// TextureFilters.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREFILTERS_HPP_
#define GALAXY_GRAPHICS_TEXTUREFILTERS_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// OpenGL Texture Filters.
		///
		enum class TextureFilters : short
		{
			///
			/// Nearest-neighbour.
			///
			MIN_NEAREST,

			///
			/// Attempts to smooth texture pattern.
			///
			MIN_TRILINEAR,

			///
			/// Nearest-neighbour.
			///
			MAG_NEAREST,

			///
			/// Attempts to smooth texture pattern.
			///
			MAG_TRILINEAR
		};
	} // namespace graphics
} // namespace galaxy

#endif
