///
/// LoadedScript.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LOADEDSCRIPT_HPP_
#define GALAXY_SCRIPTING_LOADEDSCRIPT_HPP_

#include <string_view>

namespace galaxy
{
	namespace lua
	{
		///
		/// Contains a filename and the loaded data assosiated with that script.
		///
		struct LoadedScript final
		{
			///
			/// Constructor.
			///
			LoadedScript() noexcept;

			///
			/// \brief Argument constructor.
			///
			/// Will load code from filename into m_code for you.
			///
			/// \param filename File name.
			///
			LoadedScript(std::string_view filename) noexcept;

			///
			/// Destructor.
			///
			~LoadedScript() noexcept = default;

			///
			/// File name.
			///
			std::string m_filename;

			///
			/// Script code.
			///
			std::string m_code;
		};
	} // namespace lua
} // namespace galaxy

#endif