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

		// need to recheck drawing objects :x
		// may have to draw them to transparent bitmap :x
		// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
		// https://github.com/reworks/starlight/blob/d96aec7a85b4ef402926315e0286e5f98a024967/starlight/source/sl/mapping/TMXHelper.hpp
		// http://docs.mapeditor.org/en/latest/reference/tmx-map-format/

		tmx_map_free(map);
	}

	const Rect<float, int>& Level::getBounds() const
	{
		return m_bounds;
	}
}