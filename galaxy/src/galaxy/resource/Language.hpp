///
/// Language.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_LANGUAGE_HPP_
#define GALAXY_RESOURCE_LANGUAGE_HPP_

#include <robin_hood.h>

namespace galaxy
{
	namespace res
	{
		///
		/// Class to allow app to easily switch languages.
		///
		class Language final
		{
		public:
			///
			/// Constructor.
			///
			Language() noexcept;

			///
			/// Destructor.
			///
			~Language() noexcept;

			///
			/// Loads all language files in a folder into galaxy.
			///
			/// \param lang_folder Folder that contains lua language files.
			///
			void parse_language_folder(std::string_view lang_folder);

			///
			/// Sets the currently active language.
			///
			/// \param lang Language to set. I.e. "en_au".
			///
			void set_language(std::string_view lang);

			///
			/// Translates a key into the active language's text.
			///
			/// \param key Language key to retrieve translation from.
			///
			/// \return String. RVO will automatically optimize this into a reference.
			///
			[[nodiscard]] std::string translate(std::string_view key) noexcept;

			///
			/// Reload current language.
			///
			void reload();

			///
			/// Clear langauge database.
			///
			void clear() noexcept;

		private:
			///
			/// Copy constructor.
			///
			Language(const Language&) = delete;

			///
			/// Move constructor.
			///
			Language(Language&&) = delete;

			///
			/// Copy assignment operator.
			///
			Language& operator=(const Language&) = delete;

			///
			/// Move assignment operator.
			///
			Language& operator=(Language&&) = delete;

		private:
			///
			/// Current language.
			///
			std::string m_cur_lang;

			///
			/// Language data is stored in a lua table.
			///
			robin_hood::unordered_flat_map<std::string, sol::state> m_languages;

			///
			/// Current language KVPs.
			///
			robin_hood::unordered_flat_map<std::string, std::string> m_lang_map;
		};
	} // namespace res
} // namespace galaxy

#endif