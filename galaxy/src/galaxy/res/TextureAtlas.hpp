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
#include "galaxy/graphics/texture/RenderTexture.hpp"
#include "galaxy/graphics/texture/Info.hpp"

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
			/// Constructor.
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
			/// \brief Add textures defined in a json file to atlas.
			///
			/// Uses galaxy::filesystem for parsed texture names.
			///
			/// \param file JSON filepath. Uses galaxy::filesystem to look in json folder.
			///
			void add_from_json(std::string_view file);

			///
			/// \brief Creates atlas from added files.
			///
			/// Does nothing if no files were added.
			///
			/// \param shader Shader to use when creating atlas.
			///
			void create(std::string_view shader);

			///
			/// Update atlas, including additional and previous calls to add().
			///
			/// \param shader Shader to use when creating atlas.
			///
			void update(std::string_view shader);

			///
			/// Dumps internal atlas. May take a while.
			///
			/// \param file Path (including filename) to save file to.
			///
			void save(std::string_view file);

			///
			/// Allows you to define a custom region on the texture atlas.
			///
			/// \param name ID for this new sub region.
			/// \param region New bounds for this sub region.
			///
			void add_custom_region(std::string_view name, const graphics::fRect& region);

			///
			/// Clear all data.
			///
			void clear();

			///
			/// \brief Retrieve a texture quad defined in the atlas.
			///
			/// Throws exception if it cant find texture.
			///
			/// \param name String name corresponding to texture (filename without path and extension).
			///
			/// \return Const ref to the quad.
			///
			[[nodiscard]] const graphics::fRect& get_region(std::string_view name);

			///
			/// Get texture data.
			///
			/// \return Reference to map holding texture data.
			///
			[[nodiscard]] AtlasTextureData& get_tex_data() noexcept;

			///
			/// Get atlas texture.
			///
			/// \return Pointer to texture.
			///
			[[nodiscard]] graphics::RenderTexture* get_atlas() noexcept;

			///
			/// Get size of atlas.
			///
			/// \return Size as an integer.
			///
			[[nodiscard]] const int get_size() const noexcept;

			///
			/// Get atlas OpenGL texture.
			///
			[[nodiscard]] const unsigned int gl_texture() const noexcept;

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
			/// Size of atlas.
			///
			int m_size;

			///
			/// Master texture.
			///
			graphics::RenderTexture m_texture;

			///
			/// Contains the rectangles outlining all the textures on the atlas.
			///
			algorithm::RectPack<int> m_packer;

			///
			/// Texture name (id) and assossiated info.
			///
			AtlasTextureData m_textures;
		};
	} // namespace res
} // namespace galaxy

#endif