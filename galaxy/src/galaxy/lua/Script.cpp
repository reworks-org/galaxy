///
/// Script.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/logging/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "Script.hpp"

namespace galaxy
{
	Script::Script() noexcept
	{
	}

	Script::Script(const std::string& file)
	{
		GALAXY_UNUSED(load(file));
	}

	Script::~Script() noexcept
	{
	}

	bool Script::load(const std::string& file)
	{
		auto& fs = entt::locator<VirtualFileSystem>::value();

		const auto script = fs.read(file);
		if (!script.empty())
		{
			m_script = entt::locator<sol::state>::value().load(script);

			if (!m_script.valid())
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load script '{0}' because '{1}'.", file, magic_enum::enum_name(m_script.status()));
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read script '{0}'.", file);
		}

		return m_script.valid();
	}

	sol::protected_function_result Script::run()
	{
		if (m_script.valid())
		{
			return m_script();
		}

		return {};
	}
} // namespace galaxy
