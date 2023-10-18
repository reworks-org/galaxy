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

		Disabled::Disabled(Disabled* ptr)
		{
			GALAXY_UNUSED(ptr);
		}

		Disabled::Disabled(Disabled&& d)
		{
			GALAXY_UNUSED(d);
		}

		Disabled& Disabled::operator=(Disabled&& d)
		{
			GALAXY_UNUSED(d);
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
