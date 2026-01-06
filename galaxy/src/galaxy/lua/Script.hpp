///
/// Script.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LUA_SCRIPT_HPP_
#define GALAXY_LUA_SCRIPT_HPP_

#include <sol/sol.hpp>

namespace galaxy
{
	///
	/// An encapsulation of a bunch of functions for easy usage of a script.
	///
	class Script final
	{
	public:
		///
		/// Constructor.
		///
		Script() noexcept;

		///
		/// Argument Constructor.
		///
		/// \param file File in VFS to load as a script.
		///
		Script(const std::string& file);

		///
		/// Destructor.
		///
		~Script() noexcept;

		///
		/// Load a script.
		///
		/// \param file File in VFS to load as a script.
		///
		[[nodiscard]]
		bool load(const std::string& file);

		///
		/// Run a lua script and get a return value.
		///
		/// \return Value returned from script execution. You will need to extract from function result and check validity.
		///
		[[maybe_unused]]
		sol::protected_function_result run();

	private:
		///
		/// Script loaded into sol3 memory.
		///
		sol::load_result m_script;
	};
} // namespace galaxy

#endif
