///
/// Level.cpp
/// starlight
///
/// Created by reworks on 21/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/fs/VirtualFS.hpp"
#include "sl/mapping/TMXUtils.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "Level.hpp"

namespace sl
{
	Level::Level(const Rect<float, int>& bounds, bool loadMap, const std::string& mapFile)
		:m_bounds(bounds)
	{
		if (loadMap)
		{
			std::string data = Locator::m_virtualFS->openAsString(mapFile);
			m_map = tmx_load_buffer(data.c_str(), boost::numeric_cast<int>(data.size()));

			tmxutils::processAllLayers(m_map, m_map->ly_head);
		}
	}

	Level::~Level()
	{
		tmx_map_free(m_map);
	}

	const Rect<float, int>& Level::getBounds() const
	{
		return m_bounds;
	}
}