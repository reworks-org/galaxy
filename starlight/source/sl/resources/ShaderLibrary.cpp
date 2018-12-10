///
/// ShaderLibrary.cpp
/// starlight
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "ShaderLibrary.hpp"

namespace sl
{
	ShaderLibrary::ShaderLibrary(const std::string& script)
	{
		// Load lua script from VFS.
		sol::state loader;
		std::string fullPath = Locator::world->m_scriptFolderPath + script;
		loader.script(Locator::virtualFS->openAsString(fullPath));
		sol::table shaders = loader.get<sol::table>("shaders");

		if (!shaders.empty())
		{
			// Iterate over each shader in the lua table and construct it in place.
			shaders.for_each([this](std::pair<sol::object, sol::object> pair)
			{
				m_resourceMap.emplace(entt::HashedString{ pair.first.as<const char*>() }, pair.second.as<sol::table>());
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