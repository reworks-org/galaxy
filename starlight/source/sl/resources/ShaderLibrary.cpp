///
/// ShaderLibrary.cpp
/// starlight
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"
#include "sl/fs/VFS.hpp"

#include "ShaderLibrary.hpp"

namespace sl
{
	ShaderLibrary::ShaderLibrary(const std::string& script)
	{
		sol::state lua;
		lua.script(VFS::inst()->openAsString(script));
		sol::table shaders = lua.get<sol::table>("shaders");

		shaders.for_each([this](std::pair<sol::object, sol::object> pair)
		{
			m_resourceMap.emplace(pair.first.as<entt::HashedString>(), pair.second.as<sol::table>());
		});
	}

	ShaderLibrary::~ShaderLibrary()
	{
		clean();
	}

	void ShaderLibrary::clean()
	{
		m_resourceMap.clear();
	}
}