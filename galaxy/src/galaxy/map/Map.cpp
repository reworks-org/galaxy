///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/Tag.hpp"
#include "galaxy/components/TileMap.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"

#include "Map.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(4244)
#endif

namespace galaxy
{
	namespace map
	{
		Map::Map()
		{
		}

		Map::Map(Map&& m)
		{
			this->m_name     = std::move(m.m_name);
			this->m_registry = std::move(m.m_registry);
		}

		Map& Map::operator=(Map&& m)
		{
			if (this != &m)
			{
				this->m_name     = std::move(m.m_name);
				this->m_registry = std::move(m.m_registry);
			}

			return *this;
		}

		Map::~Map()
		{
		}

		void Map::load(const ldtk::Level& level)
		{
			m_name = level.name;

			for (auto& layer : level.allLayers())
			{
				auto render_layer = 0;

				if (layer.isVisible())
				{
					switch (layer.getType())
					{
						case ldtk::LayerType::IntGrid:
							parse_intgrid_layer(layer);
							break;

						case ldtk::LayerType::Entities:
							parse_entitiy_layer(layer);
							break;

						case ldtk::LayerType::Tiles:
						case ldtk::LayerType::AutoLayer:
							parse_tile_layer(layer, render_layer);
							break;
					}
				}

				++render_layer;
			}
		}

		void Map::parse_intgrid_layer(const ldtk::Layer& layer)
		{
			GALAXY_LOG(GALAXY_WARNING, "IntGrid layer is not supported at this time.");
		}

		void Map::parse_entitiy_layer(const ldtk::Layer& layer)
		{
			for (const auto& entity : layer.allEntities())
			{
				auto        count = 0;
				const auto& id    = entity.getName();

				// name = identifier
				if (id == "COLLISION")
				{
					const auto entt = m_registry.create();

					auto& tag = m_registry.m_entt.get<components::Tag>(entt);
					tag.m_tag = id + std::to_string(count);
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Attempted to parse unknown entity '{0}'.", entity.getName());
				}

				count++;
			}
		}

		void Map::parse_tile_layer(const ldtk::Layer& layer, const int render_layer)
		{
			const auto entity = m_registry.create();
			m_registry.m_entt.emplace<flags::DenySerialization>(entity);
			m_registry.m_entt.remove<flags::Disabled>(entity);

			auto& tag = m_registry.m_entt.emplace<components::Tag>(entity);
			tag.m_tag = layer.getName();

			auto& tilemap = m_registry.m_entt.emplace<components::TileMap>(entity);
			tilemap.m_batch.init(static_cast<int>(layer.allTiles().size()));

			for (const auto& tile : layer.allTiles())
			{
				auto ldtk_verts = tile.getVertices();

				std::array<graphics::Vertex, 4> vertices;

				for (auto i = 0; i < ldtk_verts.size(); ++i)
				{
					vertices[i].m_pos.x    = ldtk_verts[i].pos.x;
					vertices[i].m_pos.y    = ldtk_verts[i].pos.y;
					vertices[i].m_texels.x = ldtk_verts[i].tex.x;
					vertices[i].m_texels.y = ldtk_verts[i].tex.y;

					// todo indices?
				}

				tilemap.m_batch.push(vertices);
			}

			auto file = std::filesystem::path(layer.getTileset().path).filename().string();
			tilemap.set_texture(file);
			tilemap.m_render_layer = render_layer;
			tilemap.m_tint         = glm::vec4(1.0f, 1.0f, 1.0f, layer.getOpacity());

			auto& transform = m_registry.m_entt.emplace<components::Transform>(entity);
			transform.m_tf.set_pos(layer.getOffset().x, layer.getOffset().y);
		}

		const std::string& Map::name() const
		{
			return m_name;
		}

	} // namespace map
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
