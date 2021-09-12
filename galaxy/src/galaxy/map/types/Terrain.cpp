///
/// Terrain.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Terrain.hpp"

namespace galaxy
{
	namespace map
	{
		Terrain::Terrain() noexcept
			: m_tile {0}
		{
		}

		Terrain::Terrain(const nlohmann::json& json)
			: m_tile {0}
		{
			parse(json);
		}

		Terrain::~Terrain() noexcept
		{
			m_properties.clear();
		}

		void Terrain::parse(const nlohmann::json& json)
		{
			if (json.count("name") > 0)
			{
				m_name = json.at("name");
			}

			if (json.count("properties") > 0)
			{
				const auto& prop_array = json.at("properties");
				for (const auto& prop : prop_array)
				{
					m_properties.emplace(prop.at("name"), prop);
				}
			}

			if (json.count("tile") > 0)
			{
				m_tile = json.at("tile");
			}
		}

		const std::string& Terrain::get_name() const noexcept
		{
			return m_name;
		}

		const int Terrain::get_tile() const noexcept
		{
			return m_tile;
		}
	} // namespace map
} // namespace galaxy