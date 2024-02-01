///
/// Script.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/Globals.hpp"

#include "Script.hpp"

namespace galaxy
{
	namespace lua
	{
		Script::Script()
			: m_loaded {false}
		{
		}

		Script::Script(const std::string& file)
			: m_loaded {false}
		{
			GALAXY_UNUSED(load(file));
		}

		Script::~Script()
		{
		}

		bool Script::load(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto script = fs.read(file);
			if (!script.empty())
			{
				m_script = core::ServiceLocator<sol::state>::ref().load(script);

				if (m_script.status() != sol::load_status::ok)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load script '{0}' because '{1}'.", file, magic_enum::enum_name(m_script.status()));
				}
				else
				{
					m_loaded = true;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read script '{0}'.", file);
			}

			return m_loaded;
		}

		bool Script::run()
		{
			if (m_loaded)
			{
				const auto result = m_script();
				return result.valid();
			}

			return false;
		}

		sol::protected_function_result Script::run_and_return()
		{
			if (m_loaded)
			{
				return m_script();
			}

			return {};
		}
	} // namespace lua
} // namespace galaxy
