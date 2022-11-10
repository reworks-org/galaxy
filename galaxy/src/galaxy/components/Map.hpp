///
/// Map.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_MAP_HPP_
#define GALAXY_COMPONENTS_MAP_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/animation/TileAnim.hpp"
#include "galaxy/graphics/Renderable.hpp"

namespace sc
{
	namespace panel
	{
		class EntityEditor;
	} // namespace panel
} // namespace sc

namespace galaxy
{
	namespace systems
	{
		class RenderSystem;
	} // namespace systems

	namespace components
	{
		///
		/// Tilemap component.
		///
		class Map final : public graphics::Renderable
		{
			friend class sc::panel::EntityEditor;
			friend class systems::RenderSystem;

		public:
			///
			/// \brief Constructor.
			///
			/// Note this will by default cap out at 10,000 tiles.
			///
			Map() noexcept;

			///
			/// Constructor.
			///
			/// \param total_tiles Reserve a set amount of tiles to match map size.
			///
			Map(const int total_tiles) noexcept;

			///
			/// Move constructor.
			///
			Map(Map&&) noexcept;

			///
			/// Move assignment operator.
			///
			Map& operator=(Map&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Map() noexcept;

			///
			/// Configure render data.
			///
			/// \param texture_id Id of texture to use in texture atlas.
			/// \param layer Layer to render map on.
			///
			void configure(const unsigned int texture_id, const int layer) noexcept;

			///
			/// Get render layer.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_layer() const noexcept;

		public:
			///
			/// Map of all tiled animation data for this map.
			///
			robin_hood::unordered_flat_map<std::uint32_t, graphics::TileAnim> m_animations;

			///
			/// Spritebatch for tilemap.
			///
			graphics::SpriteBatch m_spritebatch;

		private:
			///
			/// Copy constructor.
			///
			Map(const Map&) = delete;

			///
			/// Copy assignment operator.
			///
			Map& operator=(const Map&) = delete;

			///
			/// Stubbed function. We don't need this here.
			///
			void configure() noexcept override;
		};
	} // namespace components
} // namespace galaxy

#endif