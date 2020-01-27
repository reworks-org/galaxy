///
/// TextureAtlas.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TEXTUREATLAS_HPP_
#define GALAXY_TEXTUREATLAS_HPP_

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "galaxy/math/RectPack.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// A texture atlas containing all the textures and rasterized text in the game.
	/// This means rendering can be very efficient because only one texture has to be loaded and no switching takes place.
	///
	class TextureAtlas final
	{
	public:
		///
		/// Default constructor.
		///
		TextureAtlas() noexcept;

		///
		/// Default destructor.
		///
		~TextureAtlas() noexcept = default;

		///
		/// Add a texture to the atlas.
		///
		/// \param name Name of file in VFS.
		///
		void add(const std::string& file) noexcept;

		///
		/// Create texture atlas from a JSON file.
		///
		/// \param json Name of the JSON file in the VFS.
		///
		void addFromJSON(const std::string& json);

		///
		/// Dumps internal atlas. May take a while.
		///
		/// \param file Name to save file with, including extension you want.
		///
		void dumpAtlas(const std::string& file);

		///
		/// Return a subtextures rectangle.
		///
		/// \param texture Name of the texture in the atlas.
		///
		/// \return Rectangle.
		///
		protostar::Rect<int> getRect(const std::string& texture) noexcept;

		///
		/// Return a subtexture from the atlas.
		///
		/// \param texture Name of the texture in the atlas.
		///
		/// \return Sprite sub-texture.
		///
		sf::Sprite getTexture(const std::string& texture) noexcept;
	
	private:
		///
		/// Master texture.
		///
		sf::Texture m_atlas;

		///
		/// Contains the rectangles outlining all the textures on the atlas.
		///
		galaxy::RectPack m_packer;

		///
		/// Hashmap to quickly get a texture and its assossiated rectangle.
		///
		std::unordered_map<std::string, protostar::Rect<int>> m_rects;
	};
}

#endif