///
/// Level.cpp
/// starlight
///
/// Created by reworks on 21/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "tmx/tmx.h"
#include "sl/fs/VFS.hpp"

#include "Level.hpp"

namespace sl
{
	Level::Level(const std::string& mapFile, const Rect<float, int>& bounds)
		:m_bounds(bounds)
	{
		tmx_map* map;
		std::string data = VFS::inst()->openAsString(mapFile);
		tmx_load_buffer(data.c_str(), data.length());
	}

	const Rect<float, int>& Level::getBounds() const
	{
		return m_bounds;
	}
}