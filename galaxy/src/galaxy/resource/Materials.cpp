///
/// Materials.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Materials.hpp"

namespace galaxy
{
	namespace resource
	{
		std::shared_ptr<physics::Material> MaterialLoader::operator()(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto data = fs.open(file);
			if (!data.empty())
			{
				return std::make_shared<physics::Material>(nlohmann::json::parse(file));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Loaded a material file but it was empty! File was {0}.", file);
				return std::make_shared<physics::Material>();
			}
		}
	} // namespace resource
} // namespace galaxy