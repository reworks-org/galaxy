///
/// WangColour.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "WangColour.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangColour::WangColour() noexcept
	{
	}

	WangColour::WangColour(const nlohmann::json& json)
	{
		parse(json);
	}

	void WangColour::parse(const nlohmann::json& json)
	{
	}
}