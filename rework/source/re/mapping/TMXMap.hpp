//
//  TMXMap.hpp
//  rework
//
//  Created by reworks on 17/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TMXMAP_HPP_
#define REWORK_TMXMAP_HPP_

#include <vector>

#include "re/math/Rect.hpp"
#include "re/mapping/TMXHelper.hpp"

namespace re
{
	class TMXMap
	{
	public:
		///
		/// Constructor.
		///
		/// \param map TMX map file to load in vfs.
		///
		TMXMap(const std::string& map);
		
		///
		/// Destructor.
		///
		~TMXMap();

		///
		/// Return current map dimensions.
		///
		/// \return Rect<int> CONST
		///
		Rect<int>& getDimensions() const;

	private:
		tmx_map* m_map;
		Rect<int> m_dimensions;
		std::vector<ALLEGRO_BITMAP*> m_layers;
	};
}

#endif