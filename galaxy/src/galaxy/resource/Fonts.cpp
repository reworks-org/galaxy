///
/// Fonts.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/algorithm/FNV1a.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/embedded/RobotoLight.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Fonts.hpp"

namespace galaxy
{
	namespace resource
	{
		void FontLoader::build(robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<graphics::Font>>& cache)
		{
			cache.emplace(algorithm::fnv1a_64("RobotoLight"), std::make_shared<graphics::Font>(embedded::roboto_light, embedded::roboto_light_len));

			for (auto&& [key, font] : cache)
			{
				font->build();
			}
		}

		std::shared_ptr<graphics::Font> FontLoader::operator()(const std::string& file)
		{
			auto font = std::make_shared<graphics::Font>();
			if (!font->load(file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load {0}.", file);
			}

			return font;
		}
	} // namespace resource
} // namespace galaxy
