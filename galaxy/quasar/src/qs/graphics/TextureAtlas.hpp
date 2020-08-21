///
/// TextureAtlas.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTUREATLAS_HPP_
#define QUASAR_TEXTUREATLAS_HPP_

#include <optional>
#include <unordered_map>

#include <protostar/math/RectPack.hpp>

#include "qs/texture/RenderTexture.hpp"

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
		explicit TextureAtlas(const unsigned int size);

		///
		/// Destructor.
		///
		~TextureAtlas() noexcept;

		///
		/// Add a texture to the atlas to be processed.
		///
		/// \param file Path of a file to add.
		///
		void add(std::string_view file) noexcept;

		///
		/// \brief Creates atlas from added files.
		///
		/// Does nothing if no files were added.
		///
		/// \param renderer Renderer to use to draw textures.
		/// \param shader Shader to use when creating atlas.
		///
		void create(qs::Renderer& renderer, qs::Shader& shader);

		///
		/// Dumps internal atlas. May take a while.
		///
		/// \param file Path (including filename) to save file to.
		///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
		///
		void save(std::string_view file);

		///
		/// Custom quad definition on texture atlas.
		///
		/// \param name String name corresponding to texture (filename without path and extension).
		/// \param rect Quad definition.
		///
		void add_custom_quad(std::string_view name, const pr::Rect<float>& rect) noexcept;

		///
		/// Retrieve a texture quad defined in the atlas.
		///
		/// \param name String name corresponding to texture (filename without path and extension).
		///
		/// \return Const ref to the quad.
		///
		[[nodiscard]] std::optional<pr::Rect<float>> get_region(std::string_view name) noexcept;

		///
		/// Get atlas texture.
		///
		/// \return Pointer to texture.
		///
		[[nodiscard]] qs::RenderTexture* get_atlas() noexcept;

		///
		/// Get size of atlas.
		///
		/// \return Size as an integer.
		///
		[[nodiscard]] const int get_size() const noexcept;

	private:
		///
		/// Size of atlas.
		///
		unsigned int m_size;

		///
		/// Master texture.
		///
		qs::RenderTexture m_texture;

		///
		/// Contains the rectangles outlining all the textures on the atlas.
		///
		pr::RectPack<int> m_packer;

		///
		/// Stores list of texture files.
		///
		std::vector<std::string> m_textures;

		///
		/// ID map for textures -> rects.
		///
		std::unordered_map<std::string, pr::Rect<float>> m_regions;
	};
} // namespace qs

#endif