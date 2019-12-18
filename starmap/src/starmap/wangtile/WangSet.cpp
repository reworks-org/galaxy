///
/// WangSet.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "WangSet.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangSet::WangSet() noexcept
	{
	}

	WangSet::WangSet(const nlohmann::json& json)
	{
		parse(json);
	}

	void WangSet::parse(const nlohmann::json& json)
	{
	}
}