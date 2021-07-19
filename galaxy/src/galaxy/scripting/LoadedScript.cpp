///
/// LoadedScript.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "LoadedScript.hpp"

namespace galaxy
{
	namespace lua
	{
		LoadedScript::LoadedScript() noexcept
			: m_filename {""}
			, m_code {""}
		{
		}

		LoadedScript::LoadedScript(std::string_view filename) noexcept
		{
			m_filename = static_cast<std::string>(filename);

			const auto code = SL_HANDLE.vfs()->open(m_filename);
			if (code == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load script: {0}.", m_filename);
			}
			else
			{
				m_code = code.value();
			}
		}
	} // namespace lua
} // namespace galaxy