///
/// LuaConsole.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LUACONSOLE_HPP_
#define GALAXY_LUACONSOLE_HPP_

#include <vector>
#include <string>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Imgui Console for running lua code.
	///
	class LuaConsole final
	{
	public:
		///
		/// Constructor.
		///
		LuaConsole();

		///
		/// Copy constructor.
		///
		LuaConsole(const LuaConsole&) = default;

		///
		/// Move constructor.
		///
		LuaConsole(LuaConsole&&) = default;

		///
		/// Copy assignment operator.
		///
		LuaConsole& operator=(const LuaConsole&) = default;

		///
		/// Move assignment operator.
		///
		LuaConsole& operator=(LuaConsole&&) = default;

		///
		/// Default destructor.
		///
		~LuaConsole() = default;

		///
		/// Draw imgui widgets.
		///
		/// \param show Toggle to show the console.
		///
		void draw(bool* show);

	private:
		///
		/// Text buffer.
		///
		std::string m_buff;

		///
		/// List of inputs / outputs on console.
		///
		std::vector<std::string> m_history;
	};
} // namespace galaxy

#endif