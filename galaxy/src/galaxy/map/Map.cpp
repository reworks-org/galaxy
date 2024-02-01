///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/components/MapData.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/DenySerialization.hpp"
#include "galaxy/flags/Disabled.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/math/ZLib.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "Map.hpp"

namespace galaxy
{
	namespace map
	{
		Map::Map()
			: m_created {false}
			, m_map {nullptr}
			, m_registry {nullptr}
		{
		}

		Map::Map(const std::string& file)
			: m_created {false}
			, m_map {nullptr}
			, m_registry {nullptr}
		{
			GALAXY_UNUSED(load(file));
		}

		Map::~Map()
		{
			m_registry = nullptr;
		}

		bool Map::load(const std::string& file)
		{
			auto success = true;

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto data = fs.read(file);
			if (!data.empty())
			{
				tson::Tileson parser;
				m_map = parser.parse(data.data(), data.size(), nullptr);

				if (m_map->getStatus() == tson::ParseStatus::OK)
				{
					if (m_created)
					{
						destroy();
					}

					m_name = file;

					auto& colour           = m_map->getBackgroundColor();
					m_clear_colour.m_red   = colour.r;
					m_clear_colour.m_green = colour.g;
					m_clear_colour.m_blue  = colour.b;
					m_clear_colour.m_alpha = colour.a;
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to parse tile map '{0}' because '{1}'.", file, m_map->getStatusMessage());
					success = false;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from vfs.", file);
				success = false;
			}

			return success;
		}

		void Map::create(entt::registry& registry)
		{
			if (!m_created)
			{
				m_registry = &registry;

				auto level = 0;
				for (auto& layer : m_map->getLayers())
				{
					// todo - make runtime not load time
					if (layer.isVisible())
					{
						process_layer(layer, level);
					}
				}

				m_created = true;
			}
		}

		void Map::destroy()
		{
			if (m_created)
			{
				m_registry->destroy(m_entities.begin(), m_entities.end());
				m_entities.clear();

				m_created = false;
			}
		}

		void Map::enable()
		{
			for (const auto e : m_entities)
			{
				m_registry->remove<flags::Disabled>(e);
			}
		}

		void Map::disable()
		{
			for (const auto e : m_entities)
			{
				m_registry->emplace<flags::Disabled>(e);
			}
		}

		const std::string& Map::get_name() const
		{
			return m_name;
		}

		void Map::process_layer(tson::Layer& layer, int& level)
		{
			switch (layer.getType())
			{
				case tson::LayerType::TileLayer:
					process_tile_layer(layer, level);
					break;

				case tson::LayerType::ObjectGroup:
					process_object_layer(layer, level);
					break;

				case tson::LayerType::ImageLayer:
					process_image_layer(layer, level);
					break;

				case tson::LayerType::Group:
					for (auto& sub_layer : layer.getLayers())
					{
						process_layer(sub_layer, level);
					}
					break;
			}
		}

		void Map::process_tile_layer(tson::Layer& layer, int& level)
		{
			level++;

			const auto entity = m_registry->create();
			m_entities.push_back(entity);

			m_registry->emplace<flags::DenySerialization>(entity);
			m_registry->emplace<flags::Disabled>(entity);

			auto& tag = m_registry->emplace<components::Tag>(entity);
			tag.m_tag = layer.getName();
			if (tag.m_tag.empty())
			{
				tag.m_tag = "Unnamed Tile Layer";
			}

			auto& map = m_registry->emplace<components::MapData>(entity, layer.getTileObjects().size());

			auto& tf = m_registry->emplace<components::Transform>(entity);
			tf.set_pos(layer.getOffset().x, layer.getOffset().y);

			auto texture_id = 0U;
			auto width      = 0.0f;
			auto height     = 0.0f;

			for (auto& [pos, tile] : layer.getTileObjects())
			{
				auto       tileset  = tile.getTile()->getTileset();
				const auto animated = tile.getTile()->getAnimation().any();

				auto& atlas    = core::ServiceLocator<resource::TextureAtlas>::ref();
				auto  info_opt = atlas.query(tileset->getImage().filename().string());

				if (info_opt.has_value())
				{
					const auto& info = info_opt.value().get();

					// TODO: Fix not selecting correct atlas if map tilesets are spread out across atlas'.
					texture_id = info.m_handle;

					tson::Rect draw_rect;

					if (!animated)
					{
						draw_rect = tile.getDrawingRect();
					}
					else
					{
						const auto owner_id = tile.getTile()->getId() + tileset->getFirstgid() - 1;
						if (map.m_animations.count(owner_id) == 0)
						{
							map.m_animations[owner_id].m_anim    = &tile.getTile()->getAnimation();
							map.m_animations[owner_id].m_tileset = tileset;
						}

						const auto tile_id = tile.getTile()->getAnimation().getCurrentTileId() + tileset->getFirstgid() - 1;
						draw_rect          = tileset->getTile(tile_id)->getDrawingRect();
					}

					width  += draw_rect.width;
					height += draw_rect.height;

					components::Transform tile_transform;

					bool flippedHorizontally = tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Horizontally);
					bool flippedVertically   = tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Vertically);

					tile_transform.set_origin(draw_rect.width / 2.0f, draw_rect.height / 2.0f);

					const auto rotation = tile_transform.get_rotation();
					if (tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Diagonally))
					{
						tile_transform.rotate(90.0f);

						flippedHorizontally = flippedVertically;
						flippedVertically   = !tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Horizontally);
					}

					const auto& scale = tile_transform.get_scale();
					tile_transform.set_scale_horizontal(scale.x * (flippedHorizontally ? -1.0f : 1.0f));
					tile_transform.set_scale_vertical(scale.y * (flippedVertically ? -1.0f : 1.0f));
					tile_transform.set_pos(tile.getPosition().x, tile.getPosition().y);

					meta::vector<graphics::Vertex> vertices(4);

					const auto tile_matrix = tile_transform.get_transform();

					const auto off_x = info.m_region.x + draw_rect.x;
					const auto off_y = info.m_region.y + draw_rect.y;

					vertices[0].m_pos      = tile_matrix * glm::vec4 {0.0f, 0.0f, 0.0f, 1.0f};
					vertices[0].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
					vertices[0].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

					vertices[1].m_pos      = tile_matrix * glm::vec4 {tileset->getTileSize().x, 0.0f, 0.0f, 1.0f};
					vertices[1].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + draw_rect.width, info.m_sheet_width);
					vertices[1].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

					vertices[2].m_pos      = tile_matrix * glm::vec4 {tileset->getTileSize().x, tileset->getTileSize().y, 0.0f, 1.0f};
					vertices[2].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + draw_rect.width, info.m_sheet_width);
					vertices[2].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + draw_rect.height, info.m_sheet_height);

					vertices[3].m_pos      = tile_matrix * glm::vec4 {0.0f, tileset->getTileSize().y, 0.0f, 1.0f};
					vertices[3].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
					vertices[3].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + draw_rect.height, info.m_sheet_height);

					const auto index = map.m_spritebatch.push(vertices);
					if (animated)
					{
						map.m_animations[tile.getTile()->getId()].m_tiles_to_animate.emplace(index, vertices);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to read texture '{0}' from tilemap.");
				}
			}

			map.m_spritebatch.build();
			map.set_texture(texture_id, level);

			tf.set_origin(width / 2.0f, height / 2.0f);
		}

		void Map::process_object_layer(tson::Layer& layer, int& level)
		{
			level++;

			graphics::Colour colour;
			colour.m_red   = layer.getTintColor().r;
			colour.m_green = layer.getTintColor().g;
			colour.m_blue  = layer.getTintColor().b;

			for (auto& obj : layer.getObjects())
			{
				colour.m_alpha = layer.getTintColor().a;
				if (!obj.isVisible())
				{
					colour.m_alpha = 0;
				}

				const auto entity = m_registry->create();
				m_entities.push_back(entity);

				m_registry->emplace<flags::DenySerialization>(entity);
				m_registry->emplace<flags::Disabled>(entity);

				auto& tag = m_registry->emplace<components::Tag>(entity);
				tag.m_tag = obj.getName();
				if (tag.m_tag.empty())
				{
					tag.m_tag = "Unnamed Object";
				}

				auto& tf = m_registry->emplace<components::Transform>(entity);
				tf.set_pos(static_cast<float>(obj.getPosition().x), static_cast<float>(obj.getPosition().y));
				tf.set_rotation(obj.getRotation());

				switch (obj.getObjectType())
				{
					case tson::ObjectType::Object:
						{
							const tson::Tileset* tileset = layer.getMap()->getTilesetByGid(obj.getGid());

							const auto offset = get_tile_offset(obj.getGid(), layer.getMap(), tileset);
							auto&      image  = tileset->getFullImagePath();

							auto tx = layer.getMap()->getTileSize().x;
							auto ty = layer.getMap()->getTileSize().y;
							tf.set_origin(static_cast<float>(tx) / 2.0f, static_cast<float>(ty) / 2.0f);

							if (obj.hasFlipFlags(tson::TileFlipFlags::Horizontally))
							{
								tf.set_scale_horizontal(-tf.get_scale().x);
							}

							if (obj.hasFlipFlags(tson::TileFlipFlags::Vertically))
							{
								tf.set_scale_vertical(-tf.get_scale().y);
							}

							if (obj.hasFlipFlags(tson::TileFlipFlags::Diagonally))
							{
								tf.set_rotation(obj.getRotation() + 90.0f);
							}

							// Objects are drawn from bottom left, so we need to correct coords.
							tf.set_pos(static_cast<float>(obj.getPosition().x), static_cast<float>(obj.getPosition().y - obj.getSize().y));

							auto& sprite = m_registry->emplace<components::Sprite>(entity);
							sprite.create(image.filename().string(), {offset.x, offset.y, tx, ty}, level, layer.getOpacity());
						}
						break;

					case tson::ObjectType::Ellipse:
						{
							auto& primitive = m_registry->emplace<components::Primitive>(entity);

							components::Primitive::PrimitiveData data;
							data.fragments = GALAXY_DEFAULT_ELLIPSE_FRAGMENTS;
							data.radii.x   = obj.getSize().x / 2.0f;
							data.radii.y   = obj.getSize().y / 2.0f;

							primitive.create<math::Shape::ELLIPSE>(data, colour, level);
						}
						break;

					case tson::ObjectType::Rectangle:
						{
							auto& primitive = m_registry->emplace<components::Primitive>(entity);

							components::Primitive::PrimitiveData data;
							data.points.emplace_back(0.0f, 0.0f);
							data.points.emplace_back(obj.getSize().x, 0.0f);
							data.points.emplace_back(obj.getSize().x, obj.getSize().y);
							data.points.emplace_back(0.0f, obj.getSize().y);

							primitive.create<math::Shape::POLYGON>(data, colour, level);
						}
						break;

					case tson::ObjectType::Point:
						{
							auto& primitive = m_registry->emplace<components::Primitive>(entity);
							primitive.create<math::Shape::POINT>({}, colour, level);
						}
						break;

					case tson::ObjectType::Polygon:
						{
							auto& primitive = m_registry->emplace<components::Primitive>(entity);

							components::Primitive::PrimitiveData data;
							for (const auto& point : obj.getPolygons())
							{
								data.points.emplace_back(point.x, point.y);
							}

							primitive.create<math::Shape::POLYGON>(data, colour, level);
						}
						break;

					case tson::ObjectType::Polyline:
						{
							auto& primitive = m_registry->emplace<components::Primitive>(entity);

							components::Primitive::PrimitiveData data;
							for (const auto& point : obj.getPolylines())
							{
								data.points.emplace_back(point.x, point.y);
							}

							primitive.create<math::Shape::POLYLINE>(data, colour, level);
						}
						break;

					case tson::ObjectType::Text:
						{
							GALAXY_LOG(GALAXY_WARNING, "Tiled text Objects are not supported.");
						}
						break;

					case tson::ObjectType::Template:
						GALAXY_LOG(GALAXY_WARNING, "Tiled template objects are not supported.");
						break;
				}
			}
		}

		void Map::process_image_layer(const tson::Layer& layer, int& level)
		{
			level++;

			const auto entity = m_registry->create();
			m_entities.push_back(entity);

			m_registry->emplace<flags::Disabled>(entity);
			m_registry->emplace<flags::DenySerialization>(entity);

			auto& tag = m_registry->emplace<components::Tag>(entity);
			tag.m_tag = layer.getName();
			if (tag.m_tag.empty())
			{
				tag.m_tag = "Unnamed Image Layer";
			}

			const auto img = std::filesystem::path(layer.getImage()).filename().string();

			auto& sprite = m_registry->emplace<components::Sprite>(entity);
			sprite.create(img, level, layer.getOpacity());

			auto& tf = m_registry->emplace<components::Transform>(entity);
			tf.set_pos(layer.getOffset().x, layer.getOffset().y);
		}

		glm::ivec2 Map::get_tile_offset(const int tile_id, const tson::Map* map, const tson::Tileset* tileset)
		{
			const auto first   = tileset->getFirstgid();
			const auto columns = tileset->getColumns();
			const auto rows    = tileset->getTileCount() / columns;
			const auto last    = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

			if (tile_id >= first && tile_id <= last)
			{
				const auto base_pos = (tile_id - first);

				const int tile_mod_x  = base_pos % columns;
				const int current_row = base_pos / columns;
				const int offset_x    = (tile_mod_x != 0) ? (tile_mod_x * map->getTileSize().x) : (0 * map->getTileSize().x);
				const int offset_y    = (current_row < rows - 1) ? (current_row * map->getTileSize().y) : ((rows - 1) * map->getTileSize().y);

				return {offset_x, offset_y};
			}

			return {0, 0};
		}
	} // namespace map
} // namespace galaxy
