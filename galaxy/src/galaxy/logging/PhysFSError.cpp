///
/// PhysFSError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <physfs.h>

#include "galaxy/logging/Log.hpp"

#include "PhysFSError.hpp"

namespace galaxy
{
	namespace logging
	{
		bool physfs_check(const int code) noexcept
		{
			if (code == 0 || code == -1)
			{
				GALAXY_LOG(GALAXY_ERROR, "PHYSFS: {0}.", PHYSFS_getLastError());
				return false;
			}

			return true;
		}

		bool physfs_check(void* ptr) noexcept
		{
			if (!ptr)
			{
				GALAXY_LOG(GALAXY_ERROR, "PHYSFS: {0}.", PHYSFS_getLastError());
				return false;
			}

			return true;
		}
	} // namespace logging
} // namespace galaxy
