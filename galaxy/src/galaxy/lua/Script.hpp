///
/// Script.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_SCRIPT_HPP_
#define GALAXY_SCRIPTING_SCRIPT_HPP_

#include <sol/sol.hpp>

namespace galaxy
{
	namespace lua
	{
		///
		/// A basic script is usually only run once or run when something specific happens.
		///
		class Script final
		{
		public:
			///
			/// Constructor.
			///
			Script();

			///
			/// Argument Constructor.
			///
			/// \param file File in VFS to load as a script.
			///
			Script(const std::string& file);

			///
			/// Destructor.
			///
			~Script();

			///
			/// Load a script.
			///
			/// \param file File in VFS to load as a script.
			///
			[[nodiscard]]
			bool load(const std::string& file);

			///
			/// Run a lua script.
			///
			/// \return True if script executed successfully.
			///
			[[maybe_unused]]
			bool run();

			///
			/// Run a lua script and get a return value.
			///
			/// \return Value returned from script execution. You will need to extract from function result and check validity.
			///
			[[nodiscard]]
			sol::protected_function_result run_and_return();

		private:
			///
			/// Script loaded into sol3 memory.
			///
			sol::load_result m_script;

			///
			/// Flag to make sure script is loaded.
			///
			bool m_loaded;
		};
	} // namespace lua
} // namespace galaxy

#endif
