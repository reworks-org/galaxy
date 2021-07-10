///
/// TextureBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_TEXTUREATLAS_HPP_
#define GALAXY_RESOURCE_TEXTUREATLAS_HPP_

#include <span>

#include "galaxy/graphics/TextureAtlas.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Holds all the different potential texture atlas'.
		///
		class TextureBook final
		{
			using AtlasMap = robin_hood::unordered_flat_map<unsigned int, graphics::TextureAtlas>;

		public:
			///
			/// Constructor.
			///
			TextureBook() noexcept = default;

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			TextureBook(std::string_view file);

			///
			/// Move constructor.
			///
			TextureBook(TextureBook&&) noexcept;

			///
			/// Move assignment operator.
			///
			TextureBook& operator=(TextureBook&&) noexcept;

			///
			/// Destructor.
			///
			~TextureBook() noexcept;

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
			/// \param index Texture Atlas to add this region to.
			/// \param key Key for this new sub region.
			/// \param region New bounds for this sub region.
			///
			void add_custom_region(unsigned int index, std::string_view key, const math::Rect<float>& region);

			///
			/// Search for a texture from an atlas.
			///
			/// \param key Texture key id.
			///
			/// \return Optional. Contains std::nullopt if it doesnt exist.
			///
			[[nodiscard]] std::optional<graphics::TextureInfo> search(std::string_view key);

			///
			/// Get texture info from atlas.
			///
			/// \param index Texture Atlas to add this region to.
			/// \param key Texture key id.
			///
			/// \return Optional. Contains std::nullopt if it doesnt exist.
			///
			[[nodiscard]] std::optional<graphics::TextureInfo> get(unsigned int index, std::string_view key);

			///
			/// Clear all data.
			///
			void clear() noexcept;

			///
			/// Get flat map of all atlas'.
			///
			/// \return Const reference to a robin hood flatmap.
			///
			[[nodiscard]] AtlasMap& get_all() noexcept;

		private:
			///
			/// Copy constructor.
			///
			TextureBook(const TextureBook&) = delete;

			///
			/// Copy assignment operator.
			///
			TextureBook& operator=(const TextureBook&) = delete;

		private:
			///
			/// Texture storage.
			///
			AtlasMap m_atlas;
		};
	} // namespace res
} // namespace galaxy

#endif