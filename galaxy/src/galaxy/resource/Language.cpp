///
/// Language.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Language.hpp"

namespace galaxy
{
	namespace resource
	{
		Language::Language()
		{
		}

		Language::~Language()
		{
			clear();
		}

		void Language::load_from_vfs()
		{
			clear();

			for (const auto& file : core::ServiceLocator<fs::VirtualFileSystem>::ref().list_assets(fs::AssetType::LANG))
			{
				sol::state lua;
				load(lua, file);
			}
		}

		void Language::load(sol::state& lua, const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto data = fs.read<meta::FSTextR>(file);
			if (!data.empty())
			{
				lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table);
				lua.script(data);

				std::string region    = lua["region"];
				sol::table  lang_data = lua["data"];

				if (!m_languages.contains(region))
				{
					m_languages[region] = {};

					for (const auto& [key, value] : lang_data)
					{
						m_languages[region].emplace(key.as<std::string>(), value.as<std::string>());
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Attemped to add duplicate language '{0}'.", region);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' in vfs.", file);
			}
		}

		void Language::set(const std::string& lang)
		{
			if (m_languages.contains(lang))
			{
				m_cur_lang = lang;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Language '{0}' not found.", lang);
			}
		}

		const std::string& Language::translate(const std::string& key)
		{
			if (m_languages[m_cur_lang].contains(key))
			{
				return m_languages[m_cur_lang][key];
			}

			return key;
		}

		const std::string& Language::operator[](const std::string& key)
		{
			return translate(key);
		}

		void Language::clear(const std::string& lang)
		{
			if (m_languages.contains(lang))
			{
				m_languages.erase(lang);
			}
		}

		void Language::clear()
		{
			m_languages.clear();
		}
	} // namespace resource
} // namespace galaxy
