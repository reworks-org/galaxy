///
/// Script.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Script.hpp"

namespace galaxy
{
	namespace lua
	{
		Script::Script(std::string_view file)
		{
			load(file);
		}

		void Script::load(std::string_view file)
		{
			m_file = static_cast<std::string>(file);

			auto& fs  = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto code_opt = fs.open(file);
			if (code_opt.has_value())
			{
				m_code = code_opt.value();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load script '{0}'.", file);
			}
		}

		void Script::run()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();
			lua.script(m_code);
		}
	} // namespace lua
} // namespace galaxy