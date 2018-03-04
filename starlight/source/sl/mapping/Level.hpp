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
#include <memory>

#include "sl/math/Rect.hpp"

namespace sl
{
	class TMXMap;
	class Level
	{
	public:
		///
		/// Level constructor.
		///
		/// \param bounds Bounds of level.
		/// \param mapFile Map file name in the virtual file system.
		///
		Level(const Rect<float, int>& bounds, const std::string& mapFile = "", float lineThickness = 1.0f);

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
		std::unique_ptr<TMXMap> m_map;
		Rect<float, int> m_bounds;
	};
}

#endif