///
/// TextureAtlas.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_TEXTUREATLAS_HPP_
#define GALAXY_RES_TEXTUREATLAS_HPP_

#include <robin_hood.h>

#include "galaxy/algorithm/RectPack.hpp"
#include "galaxy/graphics/texture/TextureInfo.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Shader;
		class Renderer;
	} // namespace graphics

	namespace res
	{
		///
		/// A texture atlas containing all the textures and rasterized text in the game.
		/// This means rendering can be very efficient because only one texture has to be loaded and no switching takes place.
		///
		class TextureAtlas final
		{
		public:
			///
			/// Shorthand for long type.
			///
			typedef robin_hood::unordered_map<std::string, graphics::TextureInfo> AtlasTextureData;

			///
			/// \brief Constructor.
			///
			/// Size defaults to 1024.
			///
			TextureAtlas();

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			TextureAtlas(std::string_view file);

			///
			/// Move constructor.
			///
			TextureAtlas(TextureAtlas&&) noexcept;

			///
			/// Move assignment operator.
			///
			TextureAtlas& operator=(TextureAtlas&&) noexcept;

			///
			/// Destructor.
			///
			~TextureAtlas() noexcept;

			///
			/// Add a texture to the atlas to be processed.
			///
			/// \param file Path of a file to add.
			///
			void add(std::string_view file);

			///
			/// \brief Add textures defined in a json file to atlas, then creates atlas.
			///
			/// Uses galaxy::filesystem for parsed texture names.
			///
			/// \param file JSON filepath. Uses galaxy::filesystem to look in json folder.
			///
			void build_from_json(std::string_view json);

			///
			/// \brief Creates atlas from added files.
			///
			/// Does nothing if no files were added.
			///
			/// \param renderer Renderer to use to draw textures.
			/// \param shader Shader to use when creating atlas.
			///
			void create(std::string_view shader);

			///
			/// Dumps internal atlas. May take a while.
			///
			/// \param file Path (including filename) to save file to.
			///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
			/// \param index Which internal texture atlas to dump. -1 (default) to dump all.
			///
			void save(std::string_view file, int index = -1);

			///
			/// \brief Retrieve a texture definition in the atlas.
			///
			/// Throws exception if it cant find texture.
			///
			/// \param name String name corresponding to texture (filename without path and extension).
			///
			/// \return Ref to info struct.
			///
			[[nodiscard]] graphics::TextureInfo& get_tex_info(std::string_view name);

			///
			/// Get atlas texture.
			///
			/// \param index Spritesheet index.
			///
			/// \return Pointer to texture.
			///
			[[nodiscard]] graphics::RenderTexture* const get_atlas(const std::size_t index);

			///
			/// Get texture data.
			///
			/// \return Reference to map holding texture data.
			///
			[[nodiscard]] AtlasTextureData& get_tex_data() noexcept;

			///
			/// Get size of atlas.
			///
			/// \return Size as an integer.
			///
			[[nodiscard]] const constexpr int get_size() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			TextureAtlas(const TextureAtlas&) = delete;

			///
			/// Copy assignment operator.
			///
			TextureAtlas& operator=(const TextureAtlas&) = delete;

		private:
			///
			/// Master texture.
			///
			std::vector<graphics::RenderTexture> m_rendertextures;

			///
			/// Contains the rectangles outlining all the textures on the atlas.
			///
			std::vector<algorithm::RectPack<int>> m_rect_packs;

			///
			/// Texture name (id) and assossiated info.
			///
			AtlasTextureData m_textures;
		};
	} // namespace res
} // namespace galaxy

#endif