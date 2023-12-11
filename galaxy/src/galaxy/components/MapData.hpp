///
/// MapData.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_MAPDATA_HPP_
#define GALAXY_COMPONENTS_MAPDATA_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/map/TileAnimation.hpp"

namespace galaxy
{
	namespace systems
	{
		class RenderSystem;
	} // namespace systems

	namespace components
	{
		///
		/// Map Data component.
		///
		class MapData final : public graphics::Renderable
		{
			friend class systems::RenderSystem;

		  public:
			///
			/// Constructor.
			///
			MapData();

			///
			/// Constructor.
			///
			/// \param total_tiles Number of tiles required for spritebatch to store.
			///
			MapData(const int total_tiles);

			///
			/// Move constructor.
			///
			MapData(MapData&&);

			///
			/// Move assignment operator.
			///
			MapData& operator=(MapData&&);

			///
			/// Destructor.
			///
			virtual ~MapData();

			///
			/// Set texture and rendering layer.
			///
			/// \param texture OpenGL texture handle.
			/// \param layer Rendering layer to sort by.
			///
			void set_texture(const unsigned int texture, const int layer);

			///
			/// Number of instances to render.
			///
			/// \return Count for number of instances.
			///
			[[nodiscard]] int get_instances() const override;

			///
			/// Get OpenGL rendering mode.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_mode() const override;

			///
			/// Get vertex array object.
			///
			/// \return VAO handle.
			///
			[[nodiscard]] unsigned int get_vao() const override;

			///
			/// Gets OpenGL texture id.
			///
			/// \return unsigned int. 0 if no texture.
			///
			[[nodiscard]] unsigned int get_texture() const override;

			///
			/// Get index (element) buffer count.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_count() const override;

			///
			/// Get rendering layer.
			///
			/// \return Integer. 0 is valid as a layer. So are negatives.
			///
			[[nodiscard]] int get_layer() const override;

		  private:
			///
			/// Copy constructor.
			///
			MapData(const MapData&) = delete;

			///
			/// Copy assignment operator.
			///
			MapData& operator=(const MapData&) = delete;

		  public:
			///
			/// Tiled animation data.
			///
			robin_hood::unordered_flat_map<std::uint32_t, map::TileAnimation> m_animations;

			///
			/// Spritebatch for tileMapData.
			///
			graphics::SpriteBatch m_spritebatch;

			///
			/// OpenGL texture handle.
			///
			unsigned int m_texture;

			///
			/// Rendering layer.
			///
			int m_layer;
		};
	} // namespace components
} // namespace galaxy

#endif
