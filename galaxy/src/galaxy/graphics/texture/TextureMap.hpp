///
/// TextureMap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_TEXTUREMAP_HPP_
#define GALAXY_GRAPHICS_TEXTURE_TEXTUREMAP_HPP_

#include "galaxy/graphics/texture/Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Holds a texture and the material type.
		///
		struct TextureMap final
		{
			///
			/// Different types of material textures.
			///
			enum class Type : short
			{
				DIFFUSE,
				SPECULAR,
				NORMAL,
				HEIGHT
			};

			///
			/// Texture data.
			///
			Texture m_texture;

			///
			/// Type of texture.
			///
			Type m_type;
		};
	} // namespace graphics
} // namespace galaxy

#endif