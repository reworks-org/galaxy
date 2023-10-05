///
/// BasicScripts.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "BasicScripts.hpp"

namespace galaxy
{
	namespace resource
	{
		std::shared_ptr<lua::BasicScript> BasicScriptLoader::operator()(const std::string& file)
		{
			return std::make_shared<lua::BasicScript>(file);
		}
	} // namespace resource
} // namespace galaxy