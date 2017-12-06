//
//  Level.hpp
//  rework
//
//  Created by reworks on 21/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LEVEL_HPP_
#define REWORK_LEVEL_HPP_

#include <vector>

#include "re/math/Rect.hpp"
#include "re/types/Renderable.hpp"
#include "re/mapping/TMXHelper.hpp"
#include "re/mapping/AnimatedTile.hpp"

// we can then update the animations aswell.
// each layer should be a bitmap so we can instert it into layer render engine
// movesystem needs to grab tile information in order to make actions occur

// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c

namespace re
{
	class Level : public Renderable
	{
	public:
		///
		/// Level constructor.
		///
		/// \param script Lua script outlining level data.
		///
		Level(const std::string& script);

		///
		/// Destructor.
		///
		virtual ~Level();

		///
		/// Return current map bounds.
		///
		/// \return Rect<float, int> CONST
		///
		const Rect<float, int>& getBounds() const;

		///
		/// Update the level.
		///
		void update() = 0;

		void addAnimatedTile(AnimatedTile& tile);

	protected:
		tmx_map* m_map;
		Rect<float, int> m_bounds;
		std::vector<AnimatedTile> m_animatedTiles;
	};
}

#endif