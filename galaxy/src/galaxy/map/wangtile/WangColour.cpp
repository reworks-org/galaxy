///
/// WangColour.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangColour.hpp"

namespace galaxy
{
	namespace map
	{
		WangColour::WangColour()
		    : m_colour {"00FFFFFF"}, m_name {""}, m_probability {0.0}, m_tile {0}
		{
		}

		WangColour::WangColour(const nlohmann::json& json)
		    : m_colour {"00FFFFFF"}, m_name {""}, m_probability {0.0}, m_tile {0}
		{
			parse(json);
		}

		void WangColour::parse(const nlohmann::json& json)
		{
			if (json.count("color") > 0)
			{
				m_colour = json.at("color");
			}

			if (json.count("name") > 0)
			{
				m_name = json.at("name");
			}

			if (json.count("probability") > 0)
			{
				m_probability = json.at("probability");
			}

			if (json.count("tile") > 0)
			{
				m_tile = json.at("tile");
			}
		}

		std::string WangColour::get_colour() const
		{
			return m_colour;
		}

		std::string WangColour::get_name() const
		{
			return m_name;
		}

		const double WangColour::get_probability() const
		{
			return m_probability;
		}

		const int WangColour::get_tile() const
		{
			return m_tile;
		}
	} // namespace map
} // namespace galaxy