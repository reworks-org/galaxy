///
/// LuaUtils.hpp
/// galaxy
///
/// Created by reworks on 02/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LUAUTILS_HPP_
#define GALAXY_LUAUTILS_HPP_

#include "galaxy/libs/sol2/forward.hpp"

galaxy
{
	///
	/// Class that contains useful functions for interacting with Lua.
	///
	class LuaUtils final
	{
		friend class Application;
	public:
		///
		/// Destructor.
		///
		virtual ~LuaUtils() noexcept = default;

		///
		/// This writes out a sol::table (or sol::state) into a text file.
		///
		/// \param table sol::table containing information to write out.
		/// \param file The file to write the data into.
		/// \param tableName The name of the table in the text file.
		///
		static void writeTableToFile(const sol::table& table, const std::string& file, const std::string& tableName);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		LuaUtils() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		LuaUtils(const LuaUtils&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		LuaUtils(LuaUtils&&) = delete;

		///
		/// Copy assignment operator.
		/// Deleted.
		///
		LuaUtils& operator=(const LuaUtils &) = delete;

		///
		/// Move assignment operator.
		/// Deleted.
		///
		LuaUtils& operator=(LuaUtils &&) = delete;

		///
		/// Registers galaxy libraries' usertypes with Sol2. Called for you by Application class.
		///
		static void registergalaxyUsertypes();
	};
}

#endif