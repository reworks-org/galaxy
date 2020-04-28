///
/// Terrain.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TERRAIN_HPP_
#define STARMAP_TERRAIN_HPP_

#include <nlohmann/json_fwd.hpp>

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
		Terrain() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing terrain.
		///
		explicit Terrain(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~Terrain() noexcept;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing terrain array from root->tilset->terrain.
		///
		void parse(const nlohmann::json& json) noexcept;

		///
		/// Get terrain name.
		///
		/// \return Name as string.
		///
		const std::string& getName() const noexcept;

		///
		/// \brief Retrieve property.
		///
		/// You will need to provide the type when retrieving.
		///
		/// \param name Name of the property to retrieve.
		///
		/// \return Property cast as type.
		///
		template<typename T>
		const T getProperty(const std::string& name) noexcept;

		///
		/// Get id of the tile representing the terrain.
		///
		/// \return Const int.
		///
		const int getTile() const noexcept;

	private:
		///
		/// Name of terrain.
		///
		std::string m_name;

		///
		/// Map of Properties.
		///
		std::unordered_map<std::string, starmap::Property> m_properties;

		///
		/// Local ID of tile representing terrain.
		///
		int m_tile;
	};

	template<typename T>
	inline const T Terrain::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].get<T>();
	}
}

#endif