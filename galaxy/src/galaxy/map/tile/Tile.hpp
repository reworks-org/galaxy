///
/// Tile.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TILE_TILE_HPP_
#define GALAXY_MAP_TILE_TILE_HPP_

#include <optional>

#include "galaxy/map/layer/ObjectLayer.hpp"
#include "galaxy/map/types/Frame.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// A tile on a tileset.
		///
		class Tile final
		{
			friend class Tileset;

		public:
			///
			/// Constructor.
			///
			Tile() noexcept;

			///
			/// Parse constructor.
			///
			/// \param json JSON structure/array containing tile json.
			///
			explicit Tile(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Tile() noexcept;

			///
			/// Parses json structure to member values; etc.
			///
			/// \param json JSON structure containing tile json.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get animation frames.
			///
			/// \return Std::vector array.
			///
			[[nodiscard]] const std::vector<Frame>& get_animations() const noexcept;

			///
			/// Get local tile id.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_id() const noexcept;

			///
			/// Get image representing the tile.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_image() const noexcept;

			///
			/// Height of the tile.
			///
			/// \return Const int. In pixels.
			///
			[[nodiscard]] const int get_image_height() const noexcept;

			///
			/// Width of the tile.
			///
			/// \return Const int. In pixels.
			///
			[[nodiscard]] const int get_image_width() const noexcept;

			///
			/// Get the object group of the Tile.
			///
			/// \return Returns a std::optional. Make sure you check for std::nullopt if tile offset is not used!
			///
			[[nodiscard]] const std::optional<ObjectLayer>& get_object_group() const noexcept;

			///
			/// Chance this tile is chosen when competing with others in the editor.
			///
			/// \return Const double. This is a percentage. Will return -1.0 if not used.
			///
			[[nodiscard]] const double get_probability() const noexcept;

			///
			/// \brief Retrieve property.
			///
			/// You will need to provide the type when retrieving.
			///
			/// \param name Name of the property to retrieve.
			///
			/// \return Property cast as type.
			///
			template<tiled_property Type>
			[[nodiscard]] const Type& get_property(std::string_view name);

			///
			/// Get index of terrain for each corner of tile.
			///
			/// \return std::vector int array.
			///
			[[nodiscard]] const std::vector<int>& get_terrain_indices() const noexcept;

			///
			/// Get the type of tile.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_type() const noexcept;

		private:
			///
			/// Array of Frames.
			///
			std::vector<Frame> m_animation;

			///
			/// Local ID of the tile.
			///
			int m_id;

			///
			/// Image representing this tile (optional).
			///
			std::string m_image;

			///
			/// Height of the tile image in pixels.
			///
			int m_image_height;

			///
			/// Width of the tile image in pixels.
			///
			int m_image_width;

			///
			/// Layer with type objectgroup, when collision shapes are specified (optional).
			///
			std::optional<ObjectLayer> m_object_group;

			///
			/// Percentage chance this tile is chosen when competing with others in the editor (optional).
			///
			double m_probability;

			///
			/// Map of Properties.
			///
			robin_hood::unordered_flat_map<std::string, Property> m_properties;

			///
			/// Index of terrain for each corner of tile (optional).
			///
			std::vector<int> m_terrain_indices;

			///
			/// The type of the tile (optional).
			///
			std::string m_type;
		};

		template<tiled_property Type>
		inline const Type& Tile::get_property(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			return m_properties[str].get<Type>();
		}
	} // namespace map
} // namespace galaxy

#endif