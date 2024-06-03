///
/// Disabled.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/utils/Globals.hpp"

#include "Disabled.hpp"

namespace galaxy
{
	namespace flags
	{
		Disabled::Disabled()
		{
		}

		Disabled::Disabled(const nlohmann::json& json)
		{
			GALAXY_UNUSED(json);
		}

		Disabled::Disabled(Disabled&&)
		{
		}

		Disabled& Disabled::operator=(Disabled&&)
		{
			return *this;
		}

		Disabled::~Disabled()
		{
		}

		nlohmann::json Disabled::serialize()
		{
			return "{}"_json;
		}

		void Disabled::deserialize(const nlohmann::json& json)
		{
			GALAXY_UNUSED(json);
		}
	} // namespace flags
} // namespace galaxy
