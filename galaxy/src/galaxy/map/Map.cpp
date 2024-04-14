///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

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
							parse_tile_layer(layer, render_layer);
							break;

						case ldtk::LayerType::AutoLayer:
							parse_auto_layer(layer);
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
			GALAXY_LOG(GALAXY_WARNING, "Entity layer is not supported at this time.");

			/*
			for (auto& ldtk_entity : layer.allEntities())
			{
				auto entity = m_registry.create();

				auto& tag = m_registry.m_entt.get<components::Tag>(entity);
				tag.m_tag = ldtk_entity.getName();

				auto& transform = m_registry.m_entt.emplace<components::Transform>(entity);
				transform.m_tf.set_pos(ldtk_entity.getPosition().x, ldtk_entity.getPosition().y);

				if (ldtk_entity.hasSprite())
				{
					auto& sprite = m_registry.m_entt.emplace<components::Sprite>(entity);
					// sprite.set_texture(ldtk_entity.getTexturePath());
				}
			}
			*/
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

			auto& transform = m_registry.m_entt.emplace<components::Transform>(entity);
			transform.m_tf.set_pos(layer.getOffset().x, layer.getOffset().y);

			auto& cmd                       = m_registry.m_entt.emplace<components::RenderCommand>(entity);
			cmd.m_command.count             = tilemap.m_batch.vao().count();
			cmd.m_command.instances         = 1;
			cmd.m_command.layer             = render_layer;
			cmd.m_command.mode              = GL_TRIANGLES;
			cmd.m_command.offset            = tilemap.m_batch.vao().offset();
			cmd.m_command.vao               = tilemap.m_batch.vao().id();
			cmd.m_command.uniforms.colour   = glm::vec4(1.0f, 1.0f, 1.0f, layer.getOpacity());
			cmd.m_command.uniforms.entity   = static_cast<int>(entt::to_integral(entity));
			cmd.m_command.uniforms.handle   = tilemap.get_texture()->handle();
			cmd.m_command.uniforms.point    = false;
			cmd.m_command.uniforms.textured = true;
		}

		void Map::parse_auto_layer(const ldtk::Layer& layer)
		{
			GALAXY_LOG(GALAXY_WARNING, "AutoLayer is not supported at this time.");
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
