///
/// Property.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Property.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Property::Property() noexcept
		:m_type("")
	{
	}

	Property::Property(const nlohmann::json& json) noexcept
		:m_type("")
	{
	}

	Property::~Property() noexcept
	{
		m_value.reset();
	}

	void Property::parse(const nlohmann::json& json) noexcept
	{
		if (json.count("type") > 0)
		{
			m_type = json.at("type");
		}

		if (json.count("value") > 0)
		{
			m_value = json.at("value");
		}
	}

	const std::string& Property::getType() const noexcept
	{
		return m_type;
	}
}