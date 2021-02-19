///
/// WangSet.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_WANGTILE_WANGSET_HPP_
#define GALAXY_MAP_WANGTILE_WANGSET_HPP_

#include <robin_hood.h>

#include "galaxy/map/types/Property.hpp"
#include "galaxy/map/wangtile/WangTile.hpp"
#include "galaxy/map/wangtile/WangColour.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Defines a list of corner colors and a list of edge colors, and any number of Wang tiles using these colors.
		///
		class WangSet final
		{
		public:
			///
			/// Constructor.
			///
			WangSet() noexcept;

			///
			/// Parse constructor.
			///
			/// \param json JSON structure/array containing WangSet json.
			///
			explicit WangSet(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~WangSet() noexcept;

			///
			/// Parses json structure to member values; etc.
			///
			/// \param json JSON structure containing WangSet json.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get corner wang tile colours.
			///
			/// \return Std::vector array.
			///
			[[nodiscard]] const std::vector<WangColour>& get_corner_colours() const noexcept;

			///
			/// Get edge wang tile colours.
			///
			/// \return Std::vector array.
			///
			[[nodiscard]] const std::vector<WangTile>& get_edge_colours() const noexcept;

			///
			/// Get the name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

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
			[[nodiscard]] const Type get_property(std::string_view name);

			///
			/// Get local tile id.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_tile_id() const noexcept;

			///
			/// Get all wang tiles.
			///
			/// \return Std::vector array.
			///
			[[nodiscard]] const std::vector<WangTile>& get_tiles() const noexcept;

		private:
			///
			/// Array of Wang corner colors.
			///
			std::vector<WangColour> m_corner_colours;

			///
			/// Array of Wang edge colors.
			///
			std::vector<WangTile> m_edge_colours;

			///
			/// Name of the Wang set.
			///
			std::string m_name;

			///
			/// Map of Properties.
			///
			robin_hood::unordered_map<std::string, Property> m_properties;

			///
			/// Local ID of tile representing the Wang set.
			///
			int m_tile_id;

			///
			/// Array of Wang tiles.
			///
			std::vector<WangTile> m_tiles;
		};

		template<tiled_property Type>
		inline const Type WangSet::get_property(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			return m_properties[str].get<Type>();
		}
	} // namespace map
} // namespace galaxy

#endif