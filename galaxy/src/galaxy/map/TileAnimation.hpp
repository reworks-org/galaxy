///
/// TileAnimation.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TILEANIMATION_HPP_
#define GALAXY_MAP_TILEANIMATION_HPP_

#include <robin_hood.h>
#include <tileson.hpp>

#include "galaxy/graphics/Vertex.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Animation data for a tileset.
		///
		class TileAnimation final
		{
		  public:
			///
			/// Constructor.
			///
			TileAnimation();

			///
			/// Move constructor.
			///
			TileAnimation(TileAnimation&&);

			///
			/// Move assignment operator.
			///
			TileAnimation& operator=(TileAnimation&&);

			///
			/// Destructor.
			//
			~TileAnimation();

		  private:
			///
			/// Copy constructor.
			///
			TileAnimation(const TileAnimation&) = delete;

			///
			/// Copy assignment operator.
			///
			TileAnimation& operator=(const TileAnimation&) = delete;

		  public:
			///
			/// Tileson animation data.
			///
			tson::Animation* m_anim;

			///
			/// Tileson tileset data.
			///
			tson::Tileset* m_tileset;

			///
			/// Map of tiles being animated.
			///
			robin_hood::unordered_flat_map<std::uint32_t, meta::vector<graphics::Vertex>> m_tiles_to_animate;
		};
	} // namespace map
} // namespace galaxy

#endif
