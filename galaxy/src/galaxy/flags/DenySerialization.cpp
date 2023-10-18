///
/// DenySerialization.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/utils/Globals.hpp"

#include "DenySerialization.hpp"

namespace galaxy
{
	namespace flags
	{
		DenySerialization::DenySerialization()
		{
		}

		DenySerialization::DenySerialization(const nlohmann::json& json)
		{
			GALAXY_UNUSED(json);
		}

		DenySerialization::DenySerialization(DenySerialization* ptr)
		{
			GALAXY_UNUSED(ptr);
		}

		DenySerialization::DenySerialization(DenySerialization&& ds)
		{
			GALAXY_UNUSED(ds);
		}

		DenySerialization& DenySerialization::operator=(DenySerialization&& ds)
		{
			GALAXY_UNUSED(ds);
			return *this;
		}

		DenySerialization::~DenySerialization()
		{
		}
	} // namespace flags
} // namespace galaxy
