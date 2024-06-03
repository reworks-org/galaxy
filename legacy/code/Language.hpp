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
			Language();

			///
			/// Destructor.
			///
			~Language();

			///
			/// Loads all languages in the vfs.
			///
			void load_from_vfs();

			///
			/// Load a specific file with a provided lua state.
			///
			/// \param lua Lua state to load language with.
			/// \param file Language file to load.
			///
			void load(sol::state& lua, const std::string& file);

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
			[[nodiscard]] const std::string& translate(const std::string& key);

			///
			/// Overloaded operator[] to translate a key into the active language's text.
			///
			/// \param key Language key to retrieve translation from.
			///
			/// \return Const string reference. If not found, returns key.
			///
			[[nodiscard]] const std::string& operator[](const std::string& key);

			///
			/// Erase specific language.
			///
			/// \param lang Language to clear.
			///
			void clear(const std::string& lang);

			///
			/// Erase all language data.
			///
			void clear();

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
			/// Language data.
			///
			robin_hood::unordered_flat_map<std::string, robin_hood::unordered_flat_map<std::string, std::string>> m_languages;
		};
	} // namespace resource
} // namespace galaxy

#endif
