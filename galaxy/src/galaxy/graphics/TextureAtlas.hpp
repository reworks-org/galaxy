///
/// TextureAtlas.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREATLAS_HPP_
#define GALAXY_GRAPHICS_TEXTUREATLAS_HPP_

#include <filesystem>
#include <optional>

#include <robin_hood.h>

#include "galaxy/algorithm/RectPack.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
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
				graphics::Rect<float> m_region;

				///
				/// Original path of the texture.
				///
				std::filesystem::path m_path;
			};

			///
			/// Shorthand for long type.
			///
			typedef robin_hood::unordered_map<std::string, TextureInfo> AtlasTextureData;

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
			TextureAtlas(TextureAtlas&&);

			///
			/// Copy assignment operator.
			///
			TextureAtlas& operator=(const TextureAtlas&) = delete;

			///
			/// Move assignment operator.
			///
			TextureAtlas& operator=(TextureAtlas&&);

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
			void create(Renderer& renderer, Shader& shader);

			///
			/// Update atlas, including additional and previous calls to add().
			///
			/// \param renderer Renderer to use to draw textures.
			/// \param shader Shader to use when creating atlas.
			///
			void update(Renderer& renderer, Shader& shader);

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
			[[nodiscard]] std::optional<graphics::Rect<float>> get_region(std::string_view name);

			///
			/// Get atlas texture.
			///
			/// \return Pointer to texture.
			///
			[[nodiscard]] RenderTexture* get_atlas();

			///
			/// Get texture data.
			///
			/// \return Reference to map holding texture data.
			///
			[[nodiscard]] AtlasTextureData& get_tex_data();

			///
			/// Get size of atlas.
			///
			/// \return Size as an integer.
			///
			[[nodiscard]] const int get_size() const;

			///
			/// Get atlas OpenGL texture.
			///
			[[nodiscard]] const unsigned int gl_texture() const;

		private:
			///
			/// Size of atlas.
			///
			unsigned int m_size;

			///
			/// Master texture.
			///
			RenderTexture m_texture;

			///
			/// Contains the rectangles outlining all the textures on the atlas.
			///
			algorithm::RectPack<int> m_packer;

			///
			/// Texture name (id) and assossiated info.
			///
			AtlasTextureData m_textures;
		};
	} // namespace graphics
} // namespace galaxy

#endif