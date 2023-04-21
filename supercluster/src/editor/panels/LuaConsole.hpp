///
/// LuaConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_LUACONSOLE_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_LUACONSOLE_HPP_

#include <string>

#include "galaxy/meta/Memory.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class LuaConsole final
		{
		public:
			LuaConsole();
			~LuaConsole();

			void render();

		private:
			std::string m_buff;
			meta::vector<std::string> m_history;
		};
	} // namespace panel
} // namespace sc

#endif