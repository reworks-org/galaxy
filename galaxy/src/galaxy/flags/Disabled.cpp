///
/// Disabled.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/Settings.hpp"

#include "Disabled.hpp"

namespace galaxy
{
	namespace flags
	{
		Disabled::Disabled() noexcept
		{
		}

		Disabled::Disabled(const nlohmann::json& json)
		{
			GALAXY_UNUSED(json);
		}

		Disabled::Disabled(Disabled&& d) noexcept
		{
			GALAXY_UNUSED(d);
		}

		Disabled& Disabled::operator=(Disabled&& d) noexcept
		{
			GALAXY_UNUSED(d);
			return *this;
		}

		Disabled::~Disabled() noexcept
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
