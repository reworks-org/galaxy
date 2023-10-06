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

		void Language::load_folder(std::string_view folder)
		{
			clear();
			if (!folder.empty())
			{
				auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

				sol::state lua;
				for (const auto& file : fs.list_directory(folder))
				{
					load(lua, file);
				}
			}
		}

		void Language::load(sol::state& lua, const std::string& file)
		{
			auto& fs  = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto info = fs.find(file);

			if (info.code == fs::FileCode::FOUND)
			{
				lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table);
				lua.script_file(info.string);

				std::string region = lua["region"];
				sol::table data    = lua["data"];

				if (!m_languages.contains(region))
				{
					m_languages[region] = {};

					for (const auto& [key, value] : data)
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
				GALAXY_LOG(GALAXY_ERROR, "Failed to find {0} because {1}.", file, magic_enum::enum_name(info.code));
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