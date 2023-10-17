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
			m_state = &core::ServiceLocator<sol::state>::ref();
		}

		BasicScript::BasicScript(std::string_view file)
			: m_loaded {false}
		{
			m_state = &core::ServiceLocator<sol::state>::ref();

			load(file);
		}

		BasicScript::~BasicScript()
		{
			m_state = nullptr;
		}

		void BasicScript::load(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				m_script = m_state->load_file(info.string);
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
				GALAXY_LOG(GALAXY_ERROR, "Failed to find script '{0}' because '{1}'.", file, magic_enum::enum_name(info.code));
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
