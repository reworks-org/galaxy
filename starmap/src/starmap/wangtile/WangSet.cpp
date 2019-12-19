///
/// WangSet.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangSet.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangSet::WangSet() noexcept
		:m_name(""), m_tileID(0)
	{
	}

	WangSet::WangSet(const nlohmann::json& json)
		:m_name(""), m_tileID(0)
	{
		parse(json);
	}

	WangSet::~WangSet()
	{
		m_cornerColours.clear();
		m_edgeColours.clear();
		m_properties.clear();
		m_tiles.clear();
	}

	void WangSet::parse(const nlohmann::json& json)
	{
		auto cornerArray = json.at("cornercolors");
		std::for_each(cornerArray.begin(), cornerArray.end(), [&](const nlohmann::json& corner)
		{
			m_cornerColours.emplace_back(corner);
		});

		auto edgeArray = json.at("edgecolors");
		std::for_each(edgeArray.begin(), edgeArray.end(), [&](const nlohmann::json& edge)
		{
			m_edgeColours.emplace_back(edge);
		});

		m_name = json.at("name");

		if (json.count("properties") > 0)
		{
			auto propArray = json.at("properties");
			std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
			{
				m_properties.emplace(property.at("name"), property);
			});
		}
		
		m_tileID = json.at("tile");
		
		auto tileArray = json.at("wangtiles");
		std::for_each(tileArray.begin(), tileArray.end(), [&](const nlohmann::json& tile)
		{	
			m_tiles.emplace_back(tile);
		});
	}

	const auto& WangSet::getCornerColours() const noexcept
	{
		return m_cornerColours;
	}

	const auto& WangSet::getEdgeColours() const noexcept
	{
		return m_edgeColours;
	}
	
	const std::string& WangSet::getName() const noexcept
	{
		return m_name;
	}

	const int WangSet::getTileID() const noexcept
	{
		return m_tileID;
	}

	const auto& WangSet::getTiles() const noexcept
	{
		return m_tiles;
	}
}