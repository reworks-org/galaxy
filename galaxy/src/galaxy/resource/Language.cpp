///
/// Language.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <sol/sol.hpp>

#include "galaxy/error/Log.hpp"

#include "Language.hpp"

namespace galaxy
{
	namespace res
	{
		Language::Language() noexcept
		    : m_cur_lang {""}
		{
		}

		Language::~Language() noexcept
		{
			clear();
		}

		void Language::parse_language_folder(std::string_view lang_folder)
		{
			const auto path = std::filesystem::path(lang_folder);
			if (!std::filesystem::exists(path))
			{
				GALAXY_LOG(GALAXY_ERROR, "Unable to find folder specified: ", path.string());
			}
			else
			{
				for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(path, std::filesystem::directory_options::skip_permission_denied))
				{
					const auto& full_path = dir_entry.path();

					m_languages.emplace(full_path.stem().string(), sol::state {});
					m_languages[full_path.stem().string()].open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::utf8);
					m_languages[full_path.stem().string()].script_file(full_path.string());
				}
			}
		}

		void Language::set_language(std::string_view lang)
		{
			m_lang_map.clear();

			m_cur_lang = static_cast<std::string>(lang);
			if (m_languages.contains(m_cur_lang))
			{
				sol::table lua = m_languages[m_cur_lang]["lang"];
				for (const auto& [key, value] : lua)
				{
					m_lang_map.emplace(key.as<std::string>(), value.as<std::string>());
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Language not found: {0}.", lang);
			}
		}

		std::string Language::translate(std::string_view key) noexcept
		{
			const auto str = static_cast<std::string>(key);

			if (m_lang_map.contains(str))
			{
				return m_lang_map[str];
			}
			else
			{
				return str;
			}
		}

		void Language::reload()
		{
			set_language(m_cur_lang);
		}

		void Language::clear() noexcept
		{
			m_languages.clear();
			m_lang_map.clear();
		}
	} // namespace res
} // namespace galaxy