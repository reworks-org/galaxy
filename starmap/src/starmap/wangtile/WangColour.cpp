///
/// WangColour.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangColour.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangColour::WangColour() noexcept
		:m_colour("00FFFFFF"), m_name(""), m_probability(0.0), m_tile(0)
	{
	}

	WangColour::WangColour(const nlohmann::json& json)
		:m_colour("00FFFFFF"), m_name(""), m_probability(0.0), m_tile(0)
	{
		parse(json);
	}

	void WangColour::parse(const nlohmann::json& json)
	{
		m_colour = json.at("color");
		m_name = json.at("name");
		m_probability = json.at("probability");
		m_tile = json.at("tile");
	}

	const std::string& WangColour::getColour() const noexcept
	{
		return m_colour;
	}

	const std::string& WangColour::getName() const noexcept
	{
		return m_name;
	}

	const double WangColour::getProbability() const noexcept
	{
		return m_probability;
	}

	const int WangColour::getTile() const noexcept
	{
		return m_tile;
	}
}