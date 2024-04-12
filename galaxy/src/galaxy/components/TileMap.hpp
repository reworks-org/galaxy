///
/// TileMap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TILEMAP_HPP_
#define GALAXY_COMPONENTS_TILEMAP_HPP_

#include "galaxy/graphics/gl/Texture2D.hpp"
#include "galaxy/graphics/VertexBatch.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Rendering data for a single level/map in a LDTK world.
		///
		class TileMap final
		{
		  public:
			///
			/// Constructor.
			///
			TileMap();

			///
			/// Move constructor.
			///
			TileMap(TileMap&&);

			///
			/// Move assignment operator.
			///
			TileMap& operator=(TileMap&&);

			///
			/// Destructor.
			///
			virtual ~TileMap();

			///
			/// Sets the texture and vertex data.
			///
			/// \param texture Texture file in VFS.
			///
			void set_texture(const std::string& texture);

			///
			/// Get texture.
			///
			/// \return Pointer. DO NOT STORE THIS.
			///
			[[nodiscard]] graphics::Texture2D* get_texture();

		  public:
			///
			/// Vertex data batch.
			///
			graphics::VertexBatch m_batch;

		  private:
			///
			/// Tileset.
			///
			graphics::Texture2D* m_texture;

		  private:
			///
			/// Copy assignment operator.
			///
			TileMap& operator=(const TileMap&) = delete;

			///
			/// Copy constructor.
			///
			TileMap(const TileMap&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif
