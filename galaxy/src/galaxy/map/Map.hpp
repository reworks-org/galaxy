///
/// Map.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_MAP_HPP_
#define GALAXY_MAP_MAP_HPP_

#include <entt/entity/registry.hpp>
#include <glm/vec2.hpp>
#include <tileson.hpp>

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// A "Tiled" orthographic map.
		///
		class Map final
		{
		  public:
			///
			/// Constructor.
			///
			Map();

			///
			/// Loading constructor.
			///
			/// \param file Map file in VFS to load.
			///
			Map(const std::string& file);

			///
			/// Destructor.
			///
			~Map();

			///
			/// Load and parse tiled map.
			///
			/// \param file Map file in VFS to load.
			///
			/// \return True if successful.
			///
			[[nodiscard]] bool load(const std::string& file);

			///
			/// Create entities for tilemap.
			///
			/// \param registry Registry to create entities from.
			///
			void create(entt::registry& registry);

			///
			/// Destroy map and entities.
			///
			void destroy();

			///
			/// Enable the currently active map's entities.
			///
			void enable();

			///
			/// Disable the currently active map's entities.
			///
			void disable();

			///
			/// Get map name.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_name() const;

		  private:
			///
			/// Recursive function to process tiled map layers.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_layer(tson::Layer& layer, int& level);

			///
			/// Recursive function to process tile layer.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_tile_layer(tson::Layer& layer, int& level);

			///
			/// Recursive function to process object layer.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_object_layer(tson::Layer& layer, int& level);

			///
			/// Recursive function to process image layer.
			///
			/// \param layer Layer to process.
			/// \param level Needs to be incremented for each call.
			///
			void process_image_layer(const tson::Layer& layer, int& level);

			///
			/// Fetch tile offset in tileset.
			///
			/// \param tile_id Tile GID to use.
			/// \param map Pointer to tson map.
			/// \param tileset Pointer to tson tileset.
			///
			/// \return Offset as a vec2 integer.
			///
			[[nodiscard]] glm::ivec2 get_tile_offset(const int tile_id, const tson::Map* map, const tson::Tileset* tileset);

		  private:
			///
			/// Flag to check if entities have been created.
			///
			bool m_created;

			///
			/// Map name.
			///
			std::string m_name;

			///
			/// Pointer to parsed map data.
			///
			std::unique_ptr<tson::Map> m_map;

			///
			/// List of entities created
			///
			meta::vector<entt::entity> m_entities;

			///
			/// Pointer to registry used to create the maps entities.
			///
			entt::registry* m_registry;

			///
			/// Background map colour.
			///
			graphics::Colour m_clear_colour;
		};

	} // namespace map
} // namespace galaxy

#endif
