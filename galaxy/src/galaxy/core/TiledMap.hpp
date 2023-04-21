///
/// TiledMap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_TILEDMAP_HPP_
#define GALAXY_CORE_TILEDMAP_HPP_

#include <entt/fwd.hpp>
#include <glm/vec2.hpp>
#include <tileson.hpp>

namespace galaxy
{
	namespace scene
	{
		class Scene;
	} // namespace scene

	namespace core
	{
		class TiledMap final
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param scene Pointer to scene this map belongs to.
			///
			TiledMap(scene::Scene* scene);

			///
			/// Destructor.
			///
			~TiledMap();

			///
			/// Load map into scene.
			///
			/// \param key Name of the map to load.
			///
			void load_map(const std::string& key);

			///
			/// Enable the currently active map's entities.
			///
			void enable_map();

			///
			/// Disable the currently active map's entities.
			///
			void disable_map();

			///
			/// Get name of map.
			///
			/// \return Const string ref.
			///
			const std::string& get_name() const;

		private:
			///
			/// Constructor.
			///
			TiledMap() = delete;

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
			/// Map name.
			///
			std::string m_name;

			///
			/// Pointer to scene this map belongs to.
			///
			scene::Scene* m_scene;

			///
			/// List of entities for this map.
			///
			meta::vector<entt::entity> m_map_entities;

			///
			/// Pointer to current map.
			///
			std::shared_ptr<tson::Map> m_map;
		};
	} // namespace core
} // namespace galaxy

#endif