///
/// ShaderLibrary.cpp
/// galaxy
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/VirtualFS.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "ShaderLibrary.hpp"

namespace galaxy
{
	ShaderLibrary::ShaderLibrary(const std::string& script)
	{
		std::string fullPath = Locator::world->m_scriptFolderPath + script;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));
		sol::table shaders = Locator::lua->get<sol::table>("shaders");

		if (!shaders.empty())
		{
			// Iterate over each shader in the lua table and construct it in place.
			shaders.for_each([this](std::pair<sol::object, sol::object> pair)
			{
				m_resourceMap.emplace(entt::HashedString(pair.first.as<const char*>()), pair.second.as<sol::table>());
			});
		}
		else
		{
			LOG_S(WARNING) << "Loaded empty shader table for script: " << script;
		}
	}

	ShaderLibrary::~ShaderLibrary()
	{
		// See clean().
		clean();
	}

	void ShaderLibrary::clean()
	{
		// Shader objects are wrapped in classes so destructors handle cleaning of object data.
		m_resourceMap.clear();
	}
}