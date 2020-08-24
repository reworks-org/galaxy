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
		/// Default constructor.
		///
		LuaConsole() noexcept = default;

		///
		/// Copy constructor.
		///
		LuaConsole(const LuaConsole&) noexcept = default;

		///
		/// Move constructor.
		///
		LuaConsole(LuaConsole&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		LuaConsole& operator=(const LuaConsole&) noexcept = default;

		///
		/// Move assignment operator.
		///
		LuaConsole& operator=(LuaConsole&&) noexcept = default;

		///
		/// Default destructor.
		///
		~LuaConsole() noexcept = default;

		///
		/// Draw imgui widgets.
		///
		/// \param show Toggle to show the console.
		///
		void draw(bool* show);

	private:
		///
		/// List of inputs / outputs on console.
		///
		std::vector<std::string> m_history;
	};
} // namespace galaxy

#endif