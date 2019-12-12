///
/// Property.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "Property.hpp"

namespace starmap
{
	Property::Property() noexcept
		:m_type("")
	{
	}

	Property::Property(const nlohmann::json& json)
		:m_type("")
	{
	}

	Property::~Property() noexcept
	{
		m_value.reset(),
	}

	void Property::parse(const nlohmann::json& json)
	{
		m_type = json.at("type"),
		m_value = json.at("value"),
	}

	const std::string& Property::getType() const noexcept
	{
		return m_type,
	}
}