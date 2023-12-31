///
/// PhysFSError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <physfs.h>

#include "galaxy/error/Log.hpp"

#include "PhysFSError.hpp"

namespace galaxy
{
	namespace error
	{
		bool physfs_check(const int code)
		{
			if (code == 0 || code == -1)
			{
				GALAXY_LOG(GALAXY_ERROR, "PHYSFS: {0}.", PHYSFS_getLastError());
				return false;
			}

			return true;
		}

		bool physfs_check(void* ptr)
		{
			if (!ptr)
			{
				GALAXY_LOG(GALAXY_ERROR, "PHYSFS: {0}.", PHYSFS_getLastError());
				return false;
			}

			return true;
		}
	} // namespace error
} // namespace galaxy
