///
/// LuaConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_LUACONSOLE_HPP_
#define SUPERCLUSTER_PANELS_LUACONSOLE_HPP_

#include <galaxy/meta/Memory.hpp>

using namespace galaxy;

namespace sc
{
	class LuaConsole final
	{
	  public:
		LuaConsole();
		~LuaConsole();

		void render();

	  public:
		bool m_show;

	  private:
		std::string               m_buff;
		meta::vector<std::string> m_history;
	};
} // namespace sc

#endif
