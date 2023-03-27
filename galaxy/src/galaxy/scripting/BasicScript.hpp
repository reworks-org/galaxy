///
/// BasicScript.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_BASICSCRIPT_HPP_
#define GALAXY_SCRIPTING_BASICSCRIPT_HPP_

#include <sol/sol.hpp>

namespace galaxy
{
	namespace lua
	{
		///
		/// A basic script is usually only run once or run when something specific happens.
		///
		class BasicScript final
		{
		public:
			///
			/// Constructor.
			///
			BasicScript();

			///
			/// Argument Constructor.
			///
			/// \param file File in VFS to load as a script.
			///
			BasicScript(std::string_view file);

			///
			/// Destructor.
			///
			~BasicScript();

			///
			/// Load a script.
			///
			/// \param file File in VFS to load as a script.
			///
			void load(std::string_view file);

			///
			/// Run a lua script.
			///
			/// \return True if script executed successfully.
			///
			[[maybe_unused]] bool run();

			///
			/// Run a lua script and get a return value.
			///
			/// \return Value returned from script execution. You will need to extract from function result and check validity.
			///
			[[nodiscard]] sol::protected_function_result run_and_return();

		private:
			///
			/// Pointer to global lua state.
			///
			sol::state* m_state;

			///
			/// BasicScript loaded into sol3 memory.
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