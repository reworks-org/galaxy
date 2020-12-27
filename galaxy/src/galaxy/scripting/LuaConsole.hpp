///
/// LuaConsole.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCRIPTING_LUACONSOLE_HPP_
#define GALAXY_SCRIPTING_LUACONSOLE_HPP_

#include <vector>
#include <string>

namespace galaxy
{
	namespace lua
	{
		///
		/// Imgui Console for running lua code.
		///
		class Console final
		{
		public:
			///
			/// Constructor.
			///
			Console();

			///
			/// Default destructor.
			///
			~Console() = default;

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
	} // namespace lua
} // namespace galaxy

#endif