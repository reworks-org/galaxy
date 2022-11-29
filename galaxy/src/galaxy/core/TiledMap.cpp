///
/// TiledMap.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/entt.hpp>

#include "galaxy/components/DrawShader.hpp"
#include "galaxy/components/Flag.hpp"
#include "galaxy/components/Primitive.hpp"
#include "galaxy/components/Map.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/flags/AllowSerialize.hpp"
#include "galaxy/flags/Enabled.hpp"
#include "galaxy/resource/Maps.hpp"
#include "galaxy/resource/TextureAtlas.hpp"
#include "galaxy/scene/layers/RuntimeLayer.hpp"

namespace galaxy
{
	namespace core
	{
		TiledMap::TiledMap(scene::RuntimeLayer* layer)
			: m_layer {layer}
		{
			if (m_layer == nullptr)
			{
				GALAXY_LOG(GALAXY_FATAL, "Runtime layer must not be a nullptr when passed to a tiled map.");
			}
		}

		TiledMap::~TiledMap() noexcept
		{
			for (const auto& entity : m_map_entities)
			{
				m_layer->world().m_registry.destroy(entity);
			}

			m_map_entities.clear();
			m_layer = nullptr;
		}

		void TiledMap::load_map(const std::string& key)
		{
			m_name = key;

			for (const auto& entity : m_map_entities)
			{
				m_layer->world().m_registry.destroy(entity);
			}

			m_map_entities.clear();

			auto& maps = core::ServiceLocator<resource::Maps>::ref();
			if (maps.has(key))
			{
				auto level = 0;

				m_map = maps.get(key);
				for (auto& layer : m_map->getLayers())
				{
					if (layer.isVisible())
					{
						process_layer(layer, level);
					}
				}
			}
		}

		void TiledMap::enable_map() noexcept
		{
			for (const auto& entity : m_map_entities)
			{
				auto flag = m_layer->world().m_registry.try_get<components::Flag>(entity);
				if (flag)
				{
					flag->set_flag<flags::Enabled>();
				}
			}
		}

		void TiledMap::disable_map() noexcept
		{
			for (const auto& entity : m_map_entities)
			{
				auto flag = m_layer->world().m_registry.try_get<components::Flag>(entity);
				if (flag)
				{
					flag->unset_flag<flags::Enabled>();
				}
			}
		}

		const std::string& TiledMap::get_name() const noexcept
		{
			return m_name;
		}

		void TiledMap::process_layer(tson::Layer& layer, int& level)
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

		void TiledMap::process_tile_layer(tson::Layer& layer, int& level)
		{
			level++;

			const auto entity = m_layer->world().m_registry.create();
			m_map_entities.push_back(entity);

			auto& flag = m_layer->world().m_registry.emplace<components::Flag>(entity);
			flag.unset_flag<flags::AllowSerialize>();
			flag.set_flag<flags::Enabled>();

			auto& tag = m_layer->world().m_registry.emplace<components::Tag>(entity);
			tag.m_tag = layer.getName();
			if (tag.m_tag.empty())
			{
				tag.m_tag = "Unnamed Tile Layer";
			}

			auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
			shader.set_shader("TiledMap");

			auto& map = m_layer->world().m_registry.emplace<components::Map>(entity, layer.getTileObjects().size());

			auto& tf = m_layer->world().m_registry.emplace<components::Transform>(entity);
			tf.set_pos(layer.getOffset().x, layer.getOffset().y);

			auto texture_id = 0U;
			auto width      = 0.0f;
			auto height     = 0.0f;

			for (auto& [pos, tile] : layer.getTileObjects())
			{
				auto tileset        = tile.getTile()->getTileset();
				const auto animated = tile.getTile()->getAnimation().any();

				auto& atlas   = core::ServiceLocator<resource::TextureAtlas>::ref();
				auto info_opt = atlas.query(tileset->getImage().stem().string());

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
						const auto owner_id = tile.getTile()->getId();
						if (map.m_animations.count(owner_id) == 0)
						{
							map.m_animations[owner_id]           = {};
							map.m_animations[owner_id].m_anim    = &tile.getTile()->getAnimation();
							map.m_animations[owner_id].m_tileset = tileset;
						}

						const auto tile_id = tile.getTile()->getAnimation().getCurrentTileId();
						draw_rect          = tileset->getTile(tile_id)->getDrawingRect();
					}

					width += draw_rect.width;
					height += draw_rect.height;

					components::Transform tile_transform;

					glm::vec2 scale = tile_transform.get_scale();
					float rotation  = tile_transform.get_rotation();
					tile_transform.set_origin(draw_rect.width / 2.0f, draw_rect.height / 2.0f);

					if (tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Horizontally))
					{
						scale.x = -scale.x;
					}

					if (tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Vertically))
					{
						scale.y = -scale.y;
					}

					if (tile.getTile()->hasFlipFlags(tson::TileFlipFlags::Diagonally))
					{
						rotation += 90.f;
					}

					tile_transform.set_scale_horizontal(scale.x);
					tile_transform.set_scale_vertical(scale.y);
					tile_transform.set_rotation(rotation);
					tile_transform.set_pos(tile.getPosition().x, tile.getPosition().y);

					std::vector<graphics::Vertex> vertices(4);

					const auto tile_matrix = tile_transform.get_transform();

					const auto off_x = info.m_region.m_x + draw_rect.x;
					const auto off_y = info.m_region.m_y + draw_rect.y;

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
			}

			map.m_spritebatch.commit();
			map.configure(texture_id, level);

			tf.set_origin(width / 2.0f, height / 2.0f);
		}

		void TiledMap::process_object_layer(tson::Layer& layer, int& level)
		{
			level++;

			graphics::Colour colour;
			colour.m_red   = layer.getTintColor().r;
			colour.m_green = layer.getTintColor().g;
			colour.m_blue  = layer.getTintColor().b;
			colour.m_alpha = layer.getTintColor().a;

			for (auto& obj : layer.getObjects())
			{
				if (obj.isVisible())
				{
					const auto entity = m_layer->world().m_registry.create();
					m_map_entities.push_back(entity);

					auto& flag = m_layer->world().m_registry.emplace<components::Flag>(entity);
					flag.unset_flag<flags::AllowSerialize>();
					flag.set_flag<flags::Enabled>();

					auto& tag = m_layer->world().m_registry.emplace<components::Tag>(entity);
					tag.m_tag = obj.getName();
					if (tag.m_tag.empty())
					{
						tag.m_tag = "Unnamed Object";
					}

					auto& tf = m_layer->world().m_registry.emplace<components::Transform>(entity);
					tf.set_pos(static_cast<float>(obj.getPosition().x), static_cast<float>(obj.getPosition().y));
					tf.set_rotation(obj.getRotation());

					switch (obj.getObjectType())
					{
						case tson::ObjectType::Object:
							{
								auto tileset      = layer.getMap()->getTilesetByGid(obj.getGid());
								const auto offset = get_tile_offset(obj.getGid(), layer.getMap(), tileset);

								auto& image = tileset->getImage();

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

								auto& sprite = m_layer->world().m_registry.emplace<components::Sprite>(entity);
								sprite.create(image.stem().string(), {offset.x, offset.y, tx, ty}, level);

								auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
								shader.set_shader("Sprite");
							}
							break;

						case tson::ObjectType::Ellipse:
							{
								auto& primitive = m_layer->world().m_registry.emplace<components::Primitive>(entity);

								components::Primitive::PrimitiveData data;
								data.m_fragments = GALAXY_DEFAULT_ELLIPSE_FRAGMENTS;
								data.m_radii.x   = obj.getSize().x / 2.0f;
								data.m_radii.y   = obj.getSize().y / 2.0f;

								primitive.create<graphics::Shape::ELLIPSE>(data, colour, level);

								auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
								shader.set_shader("Line");
							}
							break;

						case tson::ObjectType::Rectangle:
							{
								auto& primitive = m_layer->world().m_registry.emplace<components::Primitive>(entity);

								components::Primitive::PrimitiveData data;
								data.m_points.emplace_back(0.0f, 0.0f);
								data.m_points.emplace_back(obj.getSize().x, 0.0f);
								data.m_points.emplace_back(obj.getSize().x, obj.getSize().y);
								data.m_points.emplace_back(0.0f, obj.getSize().y);

								primitive.create<graphics::Shape::POLYGON>(data, colour, level);

								auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
								shader.set_shader("Line");
							}
							break;

						case tson::ObjectType::Point:
							{
								auto& primitive = m_layer->world().m_registry.emplace<components::Primitive>(entity);
								primitive.create<graphics::Shape::POINT>({}, colour, level);

								auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
								shader.set_shader("Point");
							}
							break;

						case tson::ObjectType::Polygon:
							{
								auto& primitive = m_layer->world().m_registry.emplace<components::Primitive>(entity);

								components::Primitive::PrimitiveData data;
								for (const auto& point : obj.getPolygons())
								{
									data.m_points.emplace_back(point.x, point.y);
								}

								primitive.create<graphics::Shape::POLYGON>(data, colour, level);

								auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
								shader.set_shader("Line");
							}
							break;

						case tson::ObjectType::Polyline:
							{
								auto& primitive = m_layer->world().m_registry.emplace<components::Primitive>(entity);

								components::Primitive::PrimitiveData data;
								for (const auto& point : obj.getPolylines())
								{
									data.m_points.emplace_back(point.x, point.y);
								}

								primitive.create<graphics::Shape::POLYLINE>(data, colour, level);

								auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
								shader.set_shader("Line");
							}
							break;

						case tson::ObjectType::Text:
							GALAXY_LOG(GALAXY_WARNING, "Tiled Text objects are not supported.");
							break;

						case tson::ObjectType::Template:
							GALAXY_LOG(GALAXY_WARNING, "Tiled Template objects are not supported.");
							break;
					}
				}
			}
		}

		void TiledMap::process_image_layer(tson::Layer& layer, int& level)
		{
			level++;

			const auto entity = m_layer->world().m_registry.create();
			m_map_entities.push_back(entity);

			auto& flag = m_layer->world().m_registry.emplace<components::Flag>(entity);
			flag.unset_flag<flags::AllowSerialize>();
			flag.set_flag<flags::Enabled>();

			auto& tag = m_layer->world().m_registry.emplace<components::Tag>(entity);
			tag.m_tag = layer.getName();
			if (tag.m_tag.empty())
			{
				tag.m_tag = "Unnamed Image Layer";
			}

			// Image must be in texture atlas.
			auto& sprite   = m_layer->world().m_registry.emplace<components::Sprite>(entity);
			const auto img = std::filesystem::path(layer.getImage()).stem().string();
			sprite.create(img, level, layer.getOpacity());

			auto& tf = m_layer->world().m_registry.emplace<components::Transform>(entity);
			tf.set_pos(layer.getOffset().x, layer.getOffset().y);

			auto& shader = m_layer->world().m_registry.emplace<components::DrawShader>(entity);
			shader.set_shader("Sprite");
		}

		glm::ivec2 TiledMap::get_tile_offset(const int tile_id, tson::Map* map, tson::Tileset* tileset) noexcept
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
	} // namespace core
} // namespace galaxy