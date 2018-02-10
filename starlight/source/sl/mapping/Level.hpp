///
/// Level.hpp
/// starlight
///
/// Created by reworks on 21/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LEVEL_HPP_
#define STARLIGHT_LEVEL_HPP_

#include <string>

#include "sl/math/Rect.hpp"

typedef struct _tmx_map tmx_map;

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
		Level(const std::string& mapFile, const Rect<float, int>& bounds);

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
		virtual void update(const double dt) = 0;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Level() = delete;

	protected:
		tmx_map* m_map;
		Rect<float, int> m_bounds;
	};
}

#endif