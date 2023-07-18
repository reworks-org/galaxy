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

		std::future<void> Language::load(std::string_view folder)
		{
			clear();

			return core::ServiceLocator<BS::thread_pool>::ref().submit([&]() {
				if (!folder.empty())
				{
					m_folder = folder;

					auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

					const auto files = fs.list_directory(m_folder);
					for (const auto& file : files)
					{
						const auto key = std::filesystem::path(file).stem().string();

						if (!m_languages.contains(key))
						{
							m_languages.emplace(key, sol::state {});
							m_languages[key].open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::utf8);
							m_languages[key].script_file(file);
						}
						else
						{
							GALAXY_LOG(GALAXY_WARNING, "Attemped to add duplicate language '{0}'.", key);
						}
					}
				}
			});
		}

		void Language::load_mem(const std::string& key, const std::string& lang_script)
		{
			if (!m_languages.contains(key))
			{
				m_languages.emplace(key, sol::state {});
				m_languages[key].open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::utf8);
				m_languages[key].script(lang_script);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attemped to add duplicate language '{0}'.", key);
			}
		}

		void Language::set(const std::string& lang)
		{
			if (m_languages.contains(lang))
			{
				m_lang_map.clear();
				m_cur_lang = lang;

				sol::table table = m_languages[m_cur_lang]["lang"];
				for (const auto& [key, value] : table)
				{
					m_lang_map.emplace(key.as<std::string>(), value.as<std::string>());
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Language '{0}' not found.", lang);
			}
		}

		const std::string& Language::translate(const std::string& key)
		{
			if (m_lang_map.contains(key))
			{
				return m_lang_map[key];
			}

			return key;
		}

		void Language::reload()
		{
			load(m_folder);
			set(m_cur_lang);
		}

		void Language::clear()
		{
			m_languages.clear();
			m_lang_map.clear();
		}
	} // namespace resource
} // namespace galaxy