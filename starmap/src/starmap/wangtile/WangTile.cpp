///
/// WangTile.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "WangTile.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangTile::WangTile() noexcept
	{
	}

	WangTile::WangTile(const nlohmann::json& json)
	{
		parse(json);
	}

	void WangTile::parse(const nlohmann::json& json)
	{
	}
}