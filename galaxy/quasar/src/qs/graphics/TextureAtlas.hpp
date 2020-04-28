///
/// TextureAtlas.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTUREATLAS_HPP_
#define QUASAR_TEXTUREATLAS_HPP_

#include <protostar/math/RectPack.hpp>

#include "qs/core/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward dec.
	///
	class Shader;
	class Renderer;

	///
	/// A texture atlas containing all the textures and rasterized text in the game.
	/// This means rendering can be very efficient because only one texture has to be loaded and no switching takes place.
	///
	class TextureAtlas final
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Size defaults to 1024.
		///
		TextureAtlas() noexcept;

		///
		/// Argument constructor.
		///
		/// \param size Power of 2 size of texture atlas. I.e. user 4096 for a 4096x4096 texture.
		///
		TextureAtlas(const int size) noexcept;

		///
		/// Destructor.
		///
		~TextureAtlas() noexcept;

		///
		/// Add a texture to the atlas to be processed.
		///
		/// \param file Path of a file to add.
		///
		void add(const std::string& file) noexcept;

		///
		/// \brief Creates atlas from added files.
		///
		/// Does nothing if no files were added.
		///
		/// \param window Target to reset framebuffer to.
		/// \param renderer Renderer to use to draw textures.
		/// \param shader Shader to use when creating atlas.
		///
		void create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept;

		///
		/// Dumps internal atlas. May take a while.
		///
		/// \param file Path (including filename) to save file to.
		///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
		///
		void save(const std::string& file) noexcept;

		///
		/// Gets internal integer ID of a texture on the atlas.
		///
		/// \param name String name corresponding to texture (filename without path and extension).
		///
		/// \return Const integer.
		///
		const int getID(const std::string& name) noexcept;

		///
		/// Get size of atlas.
		///
		/// \return Size as an integer.
		///
		const int getSize() const noexcept;

		///
		/// Get atlas sprite.
		///
		/// \return Const reference to sprite.
		///
		qs::Sprite& getSprite() noexcept;

		///
		/// Get atlas texture.
		///
		/// \return Const reference to texture.
		///
		qs::RenderTexture& getTexture() noexcept;

	private:
		///
		/// Size of atlas.
		///
		int m_size;

		///
		/// Master texture.
		///
		qs::RenderTexture m_texture;

		///
		/// Master sprite.
		///
		qs::Sprite m_sprite;

		///
		/// Contains the rectangles outlining all the textures on the atlas.
		///
		protostar::RectPack m_packer;

		///
		/// Stores list of texture files.
		///
		std::vector<std::string> m_textureFiles;

		///
		/// ID Map for textures on the atlas.
		///
		std::unordered_map<std::string, unsigned int> m_idMap;
	};
}

#endif