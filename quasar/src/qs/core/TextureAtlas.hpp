///
/// TextureAtlas.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTUREATLAS_HPP_
#define QUASAR_TEXTUREATLAS_HPP_

#include <protostar/shapes/Rect.hpp>
#include <protostar/math/RectPack.hpp>

#include "qs/graphics/RenderTexture.hpp"
#include "qs/graphics/BatchSprite2D.hpp"

///
/// Core namespace.
///
namespace qs
{
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
		/// Size defaults to 512.
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
		/// \param shader Shader to use when rendering to texture. Must have a 'u_projection' uniform!
		///
		void create(qs::Shader& shader, qs::Window& window) noexcept;

		///
		/// Dumps internal atlas. May take a while.
		///
		/// \param file Name to save file with, including extension you want.
		///
		void dump(const std::string& file);

		///
		/// Get pointer to spritebatch. 
		///
		/// \return Returns a pointer to internal spritebatch.
		///
		qs::BatchSprite2D* getBatch() noexcept;

	private:
		///
		/// \brief Calc normalized texture point.
		///
		/// Private internal function.
		///
		/// \param p Point to transform.
		///
		/// \return Transformed float.
		///
		float cntp(const int p) noexcept;

	private:
		///
		/// Size of atlas.
		///
		int m_size;

		///
		/// Master texture.
		///
		qs::RenderTexture m_atlas;

		///
		/// Batch sprite.
		///
		qs::BatchSprite2D m_batchSprite;

		///
		/// Contains the rectangles outlining all the textures on the atlas.
		///
		protostar::RectPack m_packer;

		///
		/// Stores list of texture files.
		///
		std::vector<std::string> m_textureFiles;
	};
}

#endif