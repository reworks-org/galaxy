///
/// Language.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_LANGUAGE_HPP_
#define GALAXY_RESOURCE_LANGUAGE_HPP_

#include <robin_hood.h>
#include <sol/forward.hpp>

namespace galaxy
{
	namespace resource
	{
		///
		/// Handle different languages as a resource.
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
			/// \param folder Folder that contains lua language files.
			///
			void load(std::string_view folder);

			///
			/// Loads all language files in a folder into galaxy.
			///
			/// \param key Key to store language under.
			/// \param lang_script Lua script containing language definitions.
			///
			void load_mem(const std::string& key, const std::string& lang_script);

			///
			/// Sets the currently active language.
			///
			/// \param lang Language to set. I.e. "en_au". No extension required.
			///
			void set(const std::string& lang);

			///
			/// Translates a key into the active language's text.
			///
			/// \param key Language key to retrieve translation from.
			///
			/// \return Const string reference. If not found, returns key.
			///
			[[nodiscard]] const std::string& translate(const std::string& key) noexcept;

			///
			/// Clear all language data.
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
	} // namespace resource
} // namespace galaxy

#endif