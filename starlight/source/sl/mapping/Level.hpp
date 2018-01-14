///
///  Level.hpp
///  starlight
///
///  Created by reworks on 21/11/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LEVEL_HPP_
#define STARLIGHT_LEVEL_HPP_

#include "sl/math/Rect.hpp"

namespace sl
{
	class Level
	{
	public:
		///
		/// Level constructor.
		///
		/// \param bounds Bounds of level.
		///
		Level(const Rect<float, int>& bounds);

		///
		/// Copy Constructor.
		///
		Level(const Level&) = default;

		///
		/// Move Constructor.
		///
		Level(Level&&) = default;

		///
		/// Destructor.
		///
		virtual ~Level() = default;

		///
		/// Return current map bounds.
		///
		/// \return Rect<float, int> CONST
		///
		const Rect<float, int>& getBounds() const;

		///
		/// Update the level.
		///
		virtual void update(const double dt) = 0;

	protected:
		Rect<float, int> m_bounds;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Level() = delete;
	};
}

#endif