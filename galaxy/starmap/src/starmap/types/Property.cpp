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
	Property::Property()
	    : m_type {""}
	{
	}

	Property::Property(const nlohmann::json& json)
	    : m_type {""}
	{
		parse(json);
	}

	Property::~Property()
	{
		m_value.reset();
	}

	void Property::parse(const nlohmann::json& json)
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

	std::string Property::get_type() const
	{
		return m_type;
	}
} // namespace starmap