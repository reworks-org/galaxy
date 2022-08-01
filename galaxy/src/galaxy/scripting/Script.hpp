///
/// Script.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_SCRIPT_HPP_
#define GALAXY_SCRIPTING_SCRIPT_HPP_

#include <string_view>

namespace galaxy
{
	namespace lua
	{
		///
		/// Wraps information and utils around script code.
		///
		class Script final
		{
		public:
			///
			/// Constructor.
			///
			Script() noexcept = default;

			///
			/// Argument Constructor.
			///
			/// \param file File in VFS to load as a script.
			///
			Script(std::string_view file);

			///
			/// Destructor.
			///
			~Script() noexcept = default;

			///
			/// Load a script.
			///
			/// \param file File in VFS to load as a script.
			///
			void load(std::string_view file);

			///
			/// Run a script.
			///
			void run();

		private:
			///
			/// File that script is in.
			///
			std::string m_file;

			///
			/// Script code.
			///
			std::string m_code;
		};
	} // namespace lua
} // namespace galaxy

#endif