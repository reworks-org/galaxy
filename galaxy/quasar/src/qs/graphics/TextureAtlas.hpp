///
/// TextureAtlas.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTUREATLAS_HPP_
#define QUASAR_TEXTUREATLAS_HPP_

#include <filesystem>
#include <optional>
#include <unordered_map>

#include <protostar/math/RectPack.hpp>

#include "qs/texture/NineSlice.hpp"
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
		/// Stores information about a texture in the atlas.
		///
		struct TextureInfo
		{
			///
			/// Region of the texture in the atlas.
			///
			pr::Rect<float> m_region;

			///
			/// Original path of the texture.
			///
			std::filesystem::path m_path;

			///
			/// Nine-slice if the texture has been assigned one.
			///
			NineSlice m_nineslice;
		};

		///
		/// \brief Constructor.
		///
		/// Size defaults to 1024.
		///
		TextureAtlas();

		///
		/// Argument constructor.
		///
		/// \param size Power of 2 size of texture atlas. I.e. user 4096 for a 4096x4096 texture.
		///
		explicit TextureAtlas(const unsigned int size);

		///
		/// Copy constructor.
		///
		TextureAtlas(const TextureAtlas&) = delete;

		///
		/// Move constructor.
		///
		TextureAtlas(TextureAtlas&&) = delete;

		///
		/// Copy assignment operator.
		///
		TextureAtlas& operator=(const TextureAtlas&) = delete;

		///
		/// Move assignment operator.
		///
		TextureAtlas& operator=(TextureAtlas&&) = delete;

		///
		/// Destructor.
		///
		virtual ~TextureAtlas();

		///
		/// Add a texture to the atlas to be processed.
		///
		/// \param file Path of a file to add.
		///
		void add(std::string_view file);

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
		/// Sets a TextureAtlas texture as a 9 slice scaled texture.
		///
		/// \param name Name of texture to add nineslice to.
		/// \param slice Object containing grid information. Is std::move() stored so you can destroy original.
		///
		void add_nine_slice(std::string_view name, NineSlice& slice);

		///
		/// Dumps internal atlas. May take a while.
		///
		/// \param file Path (including filename) to save file to.
		///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
		///
		void save(std::string_view file);

		///
		/// Retrieve a texture quad defined in the atlas.
		///
		/// \param name String name corresponding to texture (filename without path and extension).
		///
		/// \return Const ref to the quad.
		///
		[[nodiscard]] std::optional<pr::Rect<float>> get_region(std::string_view name);

		///
		/// Get atlas texture.
		///
		/// \return Pointer to texture.
		///
		[[nodiscard]] qs::RenderTexture* get_atlas();

		///
		/// Get size of atlas.
		///
		/// \return Size as an integer.
		///
		[[nodiscard]] const int get_size() const;

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
		/// Texture name (id) and assossiated info.
		///
		std::unordered_map<std::string, TextureInfo> m_textures;
	};
} // namespace qs

#endif