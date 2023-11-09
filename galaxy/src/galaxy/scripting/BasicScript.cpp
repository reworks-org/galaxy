///
/// BasicScript.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "BasicScript.hpp"

namespace galaxy
{
	namespace lua
	{
		BasicScript::BasicScript()
			: m_loaded {false}
		{
		}

		BasicScript::BasicScript(const std::string& file)
			: m_loaded {false}
		{
			load(file);
		}

		BasicScript::~BasicScript()
		{
		}

		void BasicScript::load(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto script = fs.read<meta::FSTextR>(file);
			if (!script.empty())
			{
				m_script = core::ServiceLocator<sol::state>::ref().load_file(script);
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
		}

		bool BasicScript::run()
		{
			if (m_loaded)
			{
				const auto result = m_script();
				return result.valid();
			}

			return false;
		}

		sol::protected_function_result BasicScript::run_and_return()
		{
			if (m_loaded)
			{
				return m_script();
			}

			return {};
		}
	} // namespace lua
} // namespace galaxy
