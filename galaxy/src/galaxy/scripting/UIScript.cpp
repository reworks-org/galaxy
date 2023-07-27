///
/// UIScript.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "UIScript.hpp"

namespace galaxy
{
	namespace scripting
	{
		UIScript::UIScript()
			: m_active {false}
		{
		}

		UIScript::UIScript(const std::string& file, nk_context* ctx)
			: m_active {false}
		{
			load(file, ctx);
		}

		UIScript::~UIScript()
		{
		}

		void UIScript::load(const std::string& file, nk_context* ctx)
		{
			auto& fs    = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& state = core::ServiceLocator<sol::state>::ref();

			const auto info = fs.find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				auto result = state.load_file(file);
				if (result.valid())
				{
					m_self            = result.call();
					m_self["context"] = ctx;

					if (m_self.valid())
					{
						m_update = m_self["update"];

						if (m_update.valid())
						{
							m_active = true;
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Update function not present in ui script '{0}'.", file);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to validate ui script '{0}'. Make sure its in the correct format.", file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load ui script '{0}' because '{1}'.", file, magic_enum::enum_name(result.status()));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find ui script '{0}' because '{1}'.", file, magic_enum::enum_name(info.code));
			}
		}
	} // namespace scripting
} // namespace galaxy