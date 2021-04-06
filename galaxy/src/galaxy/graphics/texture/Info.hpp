///
/// Info.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_INFO_HPP_
#define GALAXY_GRAPHICS_TEXTURE_INFO_HPP_

#include <string>

#include "galaxy/graphics/Rect.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Stores information about a texture in the atlas.
		///
		struct TextureInfo final
		{
			///
			/// Region of the texture in the atlas.
			///
			graphics::fRect m_region;

			///
			/// Original path of the texture.
			///
			std::string m_path;
		};
	} // namespace graphics
} // namespace galaxy

#endif