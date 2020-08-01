///
/// WangSet.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WANGSET_HPP_
#define STARMAP_WANGSET_HPP_

#include "starmap/Meta.hpp"
#include "starmap/types/Property.hpp"
#include "starmap/wangtile/WangTile.hpp"
#include "starmap/wangtile/WangColour.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Defines a list of corner colors and a list of edge colors, and any number of Wang tiles using these colors.
	///
	class WangSet final
	{
	public:
		///
		/// Default constructor.
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
		[[nodiscard]] const auto& get_corner_colours() const noexcept;

		///
		/// Get edge wang tile colours.
		///
		/// \return Std::vector array.
		///
		[[nodiscard]] const auto& get_edge_colours() const noexcept;

		///
		/// Get the name.
		///
		/// \return Const std::string reference.
		///
		[[nodiscard]] std::string get_name() const noexcept;

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
		[[nodiscard]] const Type get_property(std::string_view name) noexcept;

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
		[[nodiscard]] const auto& get_tiles() const noexcept;

	private:
		///
		/// Array of Wang corner colors.
		///
		std::vector<starmap::WangColour> m_corner_colours;

		///
		/// Array of Wang edge colors.
		///
		std::vector<starmap::WangTile> m_edge_colours;

		///
		/// Name of the Wang set.
		///
		std::string m_name;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;

		///
		/// Local ID of tile representing the Wang set.
		///
		int m_tile_id;

		///
		/// Array of Wang tiles.
		///
		std::vector<starmap::WangTile> m_tiles;
	};

	template<tiled_property Type>
	inline const Type WangSet::get_property(std::string_view name) noexcept
	{
		const auto str = static_cast<std::string>(name);
		return m_properties[str].get<Type>();
	}
} // namespace starmap

#endif