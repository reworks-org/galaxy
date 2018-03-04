///
/// Level.cpp
/// starlight
///
/// Created by reworks on 21/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/mapping//TMXMap.hpp"

#include "Level.hpp"

namespace sl
{
	Level::Level(const Rect<float, int>& bounds, const std::string& mapFile, float lineThickness)
		:m_bounds(bounds)
	{
		if (mapFile != "")
		{
			m_map = std::make_unique<TMXMap>(mapFile, lineThickness);
			m_map->processAllLayers(m_map->m_internalMap->ly_head);
		}
	}

	Level::~Level()
	{
	}

	const Rect<float, int>& Level::getBounds() const
	{
		return m_bounds;
	}
}