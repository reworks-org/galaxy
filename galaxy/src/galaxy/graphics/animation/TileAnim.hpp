///
/// TileAnim.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TILEANIM_HPP_
#define GALAXY_GRAPHICS_TILEANIM_HPP_

#include <robin_hood.h>
#include <tileson.hpp>

#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Map animation data.
		///
		class TileAnim final
		{
		public:
			///
			/// Constructor.
			///
			TileAnim() noexcept;

			///
			/// Move constructor.
			///
			TileAnim(TileAnim&&) noexcept;

			///
			/// Move assignment operator.
			///
			TileAnim& operator=(TileAnim&&) noexcept;

			///
			/// Destructor.
			///
			~TileAnim() noexcept;

		private:
			///
			/// Copy constructor.
			///
			TileAnim(const TileAnim&) = delete;

			///
			/// Copy assignment operator.
			///
			TileAnim& operator=(const TileAnim&) = delete;

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
			robin_hood::unordered_flat_map<std::uint32_t, std::vector<graphics::Vertex>> m_tiles_to_animate;
		};
	} // namespace graphics
} // namespace galaxy

#endif