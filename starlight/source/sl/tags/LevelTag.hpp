///
///  LevelTag.hpp
///  starlight
///
///  Created by reworks on 21/11/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LEVELTAG_HPP_
#define STARLIGHT_LEVELTAG_HPP_

// we can then update the animations aswell.
// each layer should be a bitmap so we can instert it into layer render engine
// movesystem needs to grab tile information in order to make actions occur

// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c

namespace sl
{
	class LevelTag
	{
	public:
		///
		/// Level constructor.
		///
		/// \param script Lua script outlining level data.
		///
		LevelTag(const std::string& script);

		///
		/// Destructor.
		///
		virtual ~LevelTag();

		///
		/// Return current map bounds.
		///
		/// \return Rect<float, int> CONST
		///
		const Rect<float, int>& getBounds() const;

		///
		/// Update the level.
		///
		virtual void update() = 0;

		void addAnimatedTile(AnimatedTile& tile);

	protected:
		tmx_map* m_map;
		Rect<float, int> m_bounds;
		std::vector<AnimatedTile> m_animatedTiles;
	};
}

#endif