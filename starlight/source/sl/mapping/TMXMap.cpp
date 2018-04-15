///
/// TMXMap.cpp
/// starlight
///
/// Created by reworks on 07/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_primitives.h>

#include "sl/core/World.hpp"
#include "sl/utils/Time.hpp"
#include "sl/utils/Utils.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/graphics/RenderType.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/SpriteComponent.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/libs/boost/numeric/conversion/cast.hpp"

#include "TMXMap.hpp"

namespace sl
{
	TMXMap::TMXMap(const std::string& mapFile, float lineThickness)
	:m_lineThickness(lineThickness), m_internalMapData("")
	{
		//m_externalTileset = tmx_make_tileset_manager();
		m_internalMapData = Locator::m_virtualFS->openAsString(mapFile);
		m_internalMap = tmx_load_buffer(m_internalMapData.c_str(), boost::numeric_cast<int>(m_internalMapData.size()));

		if (!m_internalMap)
		{
			LOG_S(FATAL) << "Failed to load map! File: " << mapFile << " Internal Error: " << tmx_strerr();
		}
	}

	TMXMap::~TMXMap()
	{
		tmx_map_free(m_internalMap);
		//tmx_free_tileset_manager(m_externalTileset);
	}

	ALLEGRO_COLOUR TMXMap::intToColour(int colour, unsigned char opacity)
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		r = (colour >> 16) & 0xFF;
		g = (colour >> 8) & 0xFF;
		b = (colour) & 0xFF;

		return al_map_rgba(r, g, b, opacity);
	}

	void TMXMap::drawPolyline(double** points, double x, double y, int pointsc, ALLEGRO_COLOUR colour)
	{
		int i;
		for (i = 1; i<pointsc; i++)
		{
			al_draw_line(x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1], colour, m_lineThickness);
		}
	}

	void TMXMap::drawPolygon(double** points, double x, double y, int pointsc, ALLEGRO_COLOUR colour)
	{
		drawPolyline(points, x, y, pointsc, colour);
		if (pointsc > 2)
		{
			al_draw_line(x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1], colour, m_lineThickness);
		}
	}

	int TMXMap::gidExtractFlags(unsigned int gid)
	{
		int res = 0;

		if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
		if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
		/* FIXME allegro has no diagonal flip */
		return res;
	}

	unsigned int TMXMap::gidClearFlags(unsigned int gid)
	{
		return gid & TMX_FLIP_BITS_REMOVAL;
	}

	void TMXMap::processImageLayer(tmx_layer* layer)
	{
		float op = layer->opacity;

		entt::Entity entity = Locator::m_world->m_registry.create();
		Locator::m_world->m_registry.assign<SpriteComponent>(entity, utils::removeExtension(layer->content.image->source), op);
		Locator::m_world->m_registry.assign<TransformComponent>(entity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{ static_cast<float>(layer->offsetx), static_cast<float>(layer->offsety), boost::numeric_cast<int>(layer->content.image->width), boost::numeric_cast<int>(layer->content.image->height) });

		RenderComponent& rc = Locator::m_world->m_registry.assign<RenderComponent>(entity);
		rc.m_renderTypes.resize(1);
		rc.m_renderTypes[0] = RenderTypes::SPRITE;

		Locator::m_world->m_inUse.push_back(entity);
	}

	void TMXMap::processObjects(tmx_map* map, tmx_object_group* objgr)
	{
		static unsigned int poCounter = 0;
		unsigned int w = map->width * map->tile_width;
		unsigned int h = map->height * map->tile_height;

		ALLEGRO_COLOR color = intToColour(objgr->color, 255);
		tmx_object *head = objgr->head;

		entt::Entity entity = Locator::m_world->m_registry.create();
		Locator::m_world->m_registry.assign<TransformComponent>(entity, 9999, 0.0f, Rect<float, int>{ 0.0f, 0.0f, boost::numeric_cast<int>(w), boost::numeric_cast<int>(h) }); // We use a super large layer height to ensure this component is always on top.

		ALLEGRO_BITMAP* objects = al_create_bitmap(w, h);
		al_set_target_bitmap(objects);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));

		while (head)
		{
			if (head->visible)
			{
				if (head->obj_type == OT_SQUARE)
				{
					entt::Entity objentity = Locator::m_world->m_registry.create();

					sol::state templua;
					templua.script(Locator::m_virtualFS->openAsString(head->name));
					sol::table tempPO = templua.get<sol::table>("PhysicsObject");

					if (!tempPO.empty())
					{
						Locator::m_world->m_registry.assign<PhysicsComponent>(objentity, objentity, tempPO);
						Locator::m_world->m_inUse.push_back(objentity);
					}
					else
					{
						LOG_S(ERROR) << "Physics Object table was empty! Script Name: " << head->name;
					}

					al_draw_rectangle(head->x, head->y, head->x + head->width, head->y + head->height, color, m_lineThickness);
				}
				else if (head->obj_type == OT_POLYGON)
				{
					drawPolygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
				}
				else if (head->obj_type == OT_POLYLINE)
				{
					drawPolyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
				}
				else if (head->obj_type == OT_ELLIPSE)
				{
					al_draw_ellipse(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color, m_lineThickness);
				}
			}

			head = head->next;
		}

		al_flip_display();
		al_set_target_backbuffer(Locator::m_window->getDisplay());

		std::string id = "ObjectLayerNo" + std::to_string(time::getTimeSinceEpoch()) + std::to_string(poCounter);
		Locator::m_textureAtlas->addTextureToAtlas(id, objects);

		Locator::m_world->m_registry.assign<SpriteComponent>(entity, id, 1.0f);
		RenderComponent& rc = Locator::m_world->m_registry.assign<RenderComponent>(entity);
		rc.m_renderTypes.resize(1);
		rc.m_renderTypes[0] = RenderTypes::SPRITE;

		Locator::m_world->m_inUse.push_back(entity);

		al_destroy_bitmap(objects);
		++poCounter;
	}

	void TMXMap::processLayer(tmx_map* map, tmx_layer* layer)
	{
		unsigned long i = 0;
		unsigned long j = 0;
		unsigned int gid = 0;
		unsigned int x = 0;
		unsigned int y = 0;
		unsigned int w = 0;
		unsigned int h = 0;
		unsigned int flags = 0;

		tmx_tileset *ts = nullptr;
		tmx_image *im = nullptr;
		std::string identifier;
		ALLEGRO_BITMAP* tileset = nullptr;
		float op = layer->opacity;

		entt::Entity entity = Locator::m_world->m_registry.create();
		Locator::m_world->m_registry.assign<TransformComponent>(entity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{ static_cast<float>(layer->offsetx), static_cast<float>(layer->offsety), boost::numeric_cast<int>(map->width * map->tile_width), boost::numeric_cast<int>(map->height * map->tile_height) });

		ALLEGRO_BITMAP* tileLayer = al_create_bitmap(map->width * map->tile_width, map->height * map->tile_height);
		al_set_target_bitmap(tileLayer);
		al_clear_to_color(intToColour(map->backgroundcolor, op));
		al_hold_bitmap_drawing(true);

		for (i = 0; i<map->height; i++)
		{
			for (j = 0; j<map->width; j++)
			{
				gid = gidClearFlags(layer->content.gids[(i*map->width) + j]);
				if (map->tiles[gid] != NULL)
				{
					if (!(map->tiles[gid]->animation))
					{
						ts = map->tiles[gid]->tileset;
						im = map->tiles[gid]->image;
						x = map->tiles[gid]->ul_x;
						y = map->tiles[gid]->ul_y;
						w = ts->tile_width;
						h = ts->tile_height;

						if (im)
						{
							identifier = utils::removeExtension(im->source);
							tileset = Locator::m_textureAtlas->al_create_packed_sub_bitmap(identifier);
						}
						else
						{
							identifier = utils::removeExtension(ts->image->source);
							/*
							if (utils::getExtension(ts->image->source) == ".tsx")
							{
								std::string data = Locator::m_virtualFS->openAsString(ts->image->source);
								tmx_load_tileset_buffer(m_externalTilesets, data.c_str(), boost::numeric_cast<int>(data.size()), identifier);
							}
							*/
							tileset = Locator::m_textureAtlas->al_create_packed_sub_bitmap(identifier);
						}

						flags = gidExtractFlags(layer->content.gids[(i*map->width) + j]);
						al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(0.0f, 0.0f, 0.0f, op), x, y, w, h, j*ts->tile_width, i*ts->tile_height, flags);
						al_destroy_bitmap(tileset);
					}
					else
					{
						ts = map->tiles[gid]->tileset;
						im = map->tiles[gid]->image;
						w = ts->tile_width;
						h = ts->tile_height;

						if (im)
						{
							identifier = utils::removeExtension(im->source);
						}
						else
						{
							identifier = utils::removeExtension(ts->image->source);
						}

						entt::HashedString hs{ identifier.c_str() };
						Rect<int> pr = Locator::m_textureAtlas->get(hs);
						x = pr.m_x + map->tiles[gid]->ul_x;
						y = pr.m_y + map->tiles[gid]->ul_y;

						std::string id(layer->name);
						id += "AnimatedTile" + std::to_string(time::getTimeSinceEpoch());
						Locator::m_textureAtlas->addRectToAtlas(id, { boost::numeric_cast<int>(x), boost::numeric_cast<int>(y), boost::numeric_cast<int>(w), boost::numeric_cast<int>(h) });

						entt::Entity animatedEntity = Locator::m_world->m_registry.create();
						Locator::m_world->m_registry.assign<TransformComponent>(animatedEntity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, Rect<float, int>{boost::numeric_cast<float>(j*ts->tile_width), boost::numeric_cast<float>(i*ts->tile_height), boost::numeric_cast<int>(w), boost::numeric_cast<int>(h) });
						Locator::m_world->m_registry.assign<SpriteComponent>(animatedEntity, id, op);
						Locator::m_world->m_registry.assign<AnimationComponent>(animatedEntity, map, map->tiles[gid], pr.m_x, pr.m_y, boost::numeric_cast<int>(w), boost::numeric_cast<int>(h), layer->name);

						RenderComponent& rc = Locator::m_world->m_registry.assign<RenderComponent>(animatedEntity);
						rc.m_renderTypes.resize(1);
						rc.m_renderTypes[0] = RenderTypes::SPRITE;

						Locator::m_world->m_inUse.push_back(animatedEntity);
					}
				}
				else
				{
					LOG_S(ERROR) << "Map tiles are NULL! Internal Error: " << tmx_strerr();
				}
			}
		}

		al_hold_bitmap_drawing(false);
		al_flip_display();
		al_set_target_backbuffer(Locator::m_window->getDisplay());
		Locator::m_textureAtlas->addTextureToAtlas(layer->name, tileLayer);
		al_destroy_bitmap(tileLayer);

		Locator::m_world->m_registry.assign<SpriteComponent>(entity, layer->name, op);
		RenderComponent& rc = Locator::m_world->m_registry.assign<RenderComponent>(entity);
		rc.m_renderTypes.resize(1);
		rc.m_renderTypes[0] = RenderTypes::SPRITE;

		Locator::m_world->m_inUse.push_back(entity);
	}

	void TMXMap::processAllLayers(tmx_layer* layers)
	{
		while (layers)
		{
			if (layers->visible)
			{
				if (layers->type == L_GROUP)
				{
					processAllLayers(layers->content.group_head);
				}
				else if (layers->type == L_OBJGR)
				{
					processObjects(m_internalMap, layers->content.objgr);
				}
				else if (layers->type == L_IMAGE)
				{
					processImageLayer(layers);
				}
				else if (layers->type == L_LAYER)
				{
					processLayer(m_internalMap, layers);
				}
			}

			layers = layers->next;
		}
	}
}