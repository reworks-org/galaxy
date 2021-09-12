///
/// Property.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Property.hpp"

namespace galaxy
{
	namespace map
	{
		Property::Property(const nlohmann::json& json)
		{
			parse(json);
		}

		void Property::parse(const nlohmann::json& json)
		{
			if (json.count("name") > 0)
			{
				m_name = json.at("name");
			}

			if (json.count("type") > 0)
			{
				m_type = json.at("type");
			}

			if (json.count("value") > 0)
			{
				if (m_type == "string" || m_type == "file")
				{
					m_value.emplace<std::string>(json.at("value"));
				}
				else if (m_type == "int")
				{
					m_value.emplace<int>(json.at("value"));
				}
				else if (m_type == "float")
				{
					m_value.emplace<float>(json.at("value"));
				}
				else if (m_type == "bool")
				{
					m_value.emplace<bool>(json.at("value"));
				}
				else if (m_type == "color")
				{
					m_value = map::parse_hex_colour(json.at("value"));
				}
			}
		}

		const std::string& Property::get_name() const noexcept
		{
			return m_name;
		}

		const std::string& Property::get_type() const noexcept
		{
			return m_type;
		}
	} // namespace map
} // namespace galaxy