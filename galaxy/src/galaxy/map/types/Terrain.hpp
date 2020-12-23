///
/// Terrain.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TERRAIN_HPP_
#define STARMAP_TERRAIN_HPP_

#include <nlohmann/json_fwd.hpp>
#include <robin_hood.h>

#include "starmap/types/Property.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Terrains help the transitions between tiles.
	///
	class Terrain final
	{
	public:
		///
		/// Default constructor.
		///
		Terrain();

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing terrain.
		///
		explicit Terrain(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~Terrain();

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing terrain array from root->tilset->terrain.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get terrain name.
		///
		/// \return Name as string.
		///
		[[nodiscard]] std::string get_name() const;

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
		/// Get id of the tile representing the terrain.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_tile() const;

	private:
		///
		/// Name of terrain.
		///
		std::string m_name;

		///
		/// Map of Properties.
		///
		robin_hood::unordered_map<std::string, starmap::Property> m_properties;

		///
		/// Local ID of tile representing terrain.
		///
		int m_tile;
	};

	template<tiled_property Type>
	inline const Type Terrain::get_property(std::string_view name)
	{
		const auto str = static_cast<std::string>(name);
		return m_properties[str].get<Type>();
	}
} // namespace starmap

#endif