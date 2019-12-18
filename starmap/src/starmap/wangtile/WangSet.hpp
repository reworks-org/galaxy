///
/// WangSet.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WANGSET_HPP_
#define STARMAP_WANGSET_HPP_

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
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing WangSet json.
		///
		explicit WangSet(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		WangSet(const WangSet&) = default;

		///
		/// Move constructor.
		///
		WangSet(WangSet&&) = default;

		///
		/// Destructor.
		///
		~WangSet();

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing WangSet json.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get corner wang tile colours.
		///
		/// \return Std::vector array.
		///
		const auto& getCornerColours() const noexcept;

		///
		/// Get edge wang tile colours.
		///
		/// \return Std::vector array.
		///
		const auto& getEdgeColours() const noexcept;

		///
		/// Get the name.
		///
		/// \return Const std::string reference.
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
		/// Get local tile id.
		///
		/// \return Const int.
		///
		const int getTileID() const noexcept;

		///
		/// Get all wang tiles.
		///
		/// \return Std::vector array.
		///
		const auto& getTiles() const noexcept;

	private:
		///
		/// Array of Wang corner colors.
		///
		std::vector<starmap::WangColour> m_cornerColours;

		///
		/// Array of Wang edge colors.
		///
		std::vector<starmap::WangTile> m_edgeColours;
			
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
		int m_tileID;

		///
		/// Array of Wang tiles.
		///
		std::vector<starmap::WangTile> m_tiles;
	};

	template<typename T>
	inline const T WangSet::getProperty(const std::string& name) noexcept
	{
		return m_properties[name].get<T>();
	}
}

#endif