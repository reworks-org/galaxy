///
/// TextureAtlas.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREATLAS_HPP_
#define GALAXY_GRAPHICS_TEXTUREATLAS_HPP_

#include <robin_hood.h>

#include "galaxy/algorithm/RectPack.hpp"
#include "galaxy/graphics/RenderTexture.hpp"

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
			math::Rect<float> m_region;

			///
			/// Original path of the texture.
			///
			std::string m_path;

			///
			/// Index of which atlas this sprite belongs to.
			///
			unsigned int m_index;
		};

		///
		/// A texture atlas is a collection of textures stiched together to use with a spritebatch.
		///
		class TextureAtlas final
		{
		public:
			///
			/// Shorthand for long type.
			///
			using TextureData = robin_hood::unordered_map<std::string, graphics::TextureInfo>;

			///
			/// Constructor.
			///
			TextureAtlas() noexcept;

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
			/// Add a texture file to the atlas.
			///
			/// \param file Texture file in the vfs to add to atlas.
			///
			/// \return Const boolean True if add was successful.
			///
			[[maybe_unused]] const bool add(std::string_view file);

			///
			/// Adds multiple files at once.
			///
			/// \param files Array structure of paths to textures.
			///
			void add_multi(std::span<std::string> files);

			///
			/// \brief Add textures defined in a json file to atlas.
			///
			/// Uses galaxy::filesystem for parsed texture names.
			///
			/// \param file JSON filepath. Uses galaxy::filesystem to look in json folder.
			///
			void add_json(std::string_view file);

			///
			/// \brief Allows you to define a custom region on a texture atlas.
			///
			/// Do not create these dynamically based on player input, they are not serialized.
			/// Use this for i.e. tilemapping.
			///
			/// \param key Key for this new sub region.
			/// \param region New bounds for this sub region.
			///
			void add_custom_region(std::string_view key, const math::Rect<float>& region);

			///
			/// Check if atlas contains a texture.
			///
			/// \return True if found.
			///
			[[nodiscard]] const bool contains(std::string_view key) noexcept;

			///
			/// Get texture info object.
			///
			/// \param key Texture to get.
			///
			/// \return Optional. Will be std::nullopt if not found.
			///
			[[nodiscard]] std::optional<TextureInfo> get_texture_info(std::string_view key) noexcept;

			///
			/// \brief Get pixel dimension of atlas.
			///
			/// Size is equal for width and height.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_size() const noexcept;

			///
			/// Get altas ID.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int get_id() const noexcept;

			///
			/// Get OpenGL texture id.
			///
			/// \return Const unsigned integer.
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
			/// Unique ID assigned to this texture atlas instance.
			///
			unsigned int m_id;

			///
			/// Get pixel dimension of atlas.
			/// Size is equal for width and height.
			///
			int m_size;

			///
			/// Rectangle packing algorithm structure.
			///
			algorithm::RectPack<int> m_packer;

			///
			/// Texture to combine to.
			///
			RenderTexture m_render_texture;

			///
			/// Texture name (id) and assossiated info.
			///
			TextureData m_textures;
		};
	} // namespace graphics
} // namespace galaxy

#endif