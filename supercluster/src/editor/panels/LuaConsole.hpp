///
/// LuaConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_LUACONSOLE_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_LUACONSOLE_HPP_

#include <string>
#include <vector>

namespace sc
{
	namespace panel
	{
		///
		/// Imgui LuaConsole for running lua code.
		///
		class LuaConsole final
		{
		public:
			LuaConsole();

			void render();

		private:
			std::string m_buff;
			std::vector<std::string> m_history;
		};
	} // namespace panel
} // namespace sc

#endif