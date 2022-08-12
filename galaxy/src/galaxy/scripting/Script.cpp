///
/// Script.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Script.hpp"

namespace galaxy
{
	namespace lua
	{
		Script::Script() noexcept
			: m_loaded {false}
		{
			m_state = &core::ServiceLocator<sol::state>::ref();
		}

		Script::Script(std::string_view file) noexcept
			: m_loaded {false}
		{
			m_state = &core::ServiceLocator<sol::state>::ref();

			load(file);
		}

		Script::~Script() noexcept
		{
			m_state = nullptr;
		}

		void Script::load(std::string_view file) noexcept
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(file);
			if (info.m_code == fs::FileCode::FOUND)
			{
				m_script = m_state->load_file(info.m_string);
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
				GALAXY_LOG(GALAXY_ERROR, "Failed to find script '{0}' because '{1}'.", file, magic_enum::enum_name(info.m_code));
			}
		}

		bool Script::run() noexcept
		{
			if (m_loaded)
			{
				const auto result = m_script();
				return result.valid();
			}

			return false;
		}

		sol::protected_function_result Script::run_and_return() noexcept
		{
			if (m_loaded)
			{
				return m_script();
			}

			return {};
		}
	} // namespace lua
} // namespace galaxy