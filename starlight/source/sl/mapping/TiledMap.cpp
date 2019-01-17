///
/// TiledMap.cpp
/// starlight
///
/// Created by reworks on 11/01/2019.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>
#include <allegro5/allegro_primitives.h>

#include "sl/utils/Time.hpp"
#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/components/AnimationComponent.hpp"

#include "TiledMap.hpp"

///
/// Controls how thick lines are for tmx objects.
///
#define LINE_THICKNESS 2.5

///
/// This namespace contains functions from the Allegro 5 example from libtmx.
/// They are used to assist in rendering a map with allegro.
/// This code is under the same license as libtmx.
///
///
namespace libtmx
{
	ALLEGRO_COLOR int_to_al_color(int color, unsigned char opacity) {
		unsigned char r, g, b;

		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = (color) & 0xFF;

		return al_map_rgba(r, g, b, opacity);
	}

	void draw_polyline(double **points, double x, double y, int pointsc, ALLEGRO_COLOR color) {
		int i;
		for (i = 1; i < pointsc; i++) {
			al_draw_line(x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1], color, LINE_THICKNESS);
		}
	}

	void draw_polygon(double **points, double x, double y, int pointsc, ALLEGRO_COLOR color) {
		draw_polyline(points, x, y, pointsc, color);
		if (pointsc > 2) {
			al_draw_line(x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1], color, LINE_THICKNESS);
		}
	}

	int gid_extract_flags(unsigned int gid) {
		int res = 0;

		if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
		if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;

		return res;
	}

	unsigned int gid_clear_flags(unsigned int gid) {
		return gid & TMX_FLIP_BITS_REMOVAL;
	}

	void image(tmx_layer* layer)
	{
		// On an image layer, we want to create an entity that represents the background to render.
		entt::DefaultRegistry::entity_type entity = sl::Locator::world->m_registry.create();

		// Assign Render component.
		sl::Locator::world->m_registry.assign<sl::RenderComponent>(entity, layer->opacity, std::filesystem::path(layer->content.image->source).stem().string());

		// Assign Transform component.
		sl::Locator::world->m_registry.assign<sl::TransformComponent>(entity, tmx_get_property(layer->properties, "renderLayer")->value.integer, 0.0f,
			sl::Rect<float, int>{static_cast<float>(layer->offsetx), static_cast<float>(layer->offsety), static_cast<int>(layer->content.image->width), static_cast<int>(layer->content.image->height)});

		// Assign Enabled component.
		sl::Locator::world->m_registry.assign<sl::EnabledComponent>(entity);
	}

	void object(tmx_map *map, tmx_layer *layer)
	{
		// Retrieve layer data.
		bool objectIsDrawn = false;
		unsigned int w = map->width * map->tile_width;
		unsigned int h = map->height * map->tile_height;
		tmx_object_group* objgr = layer->content.objgr;

		// Prep for layer contstruction.
		tmx_object *head = objgr->head;
		ALLEGRO_COLOR color = libtmx::int_to_al_color(objgr->color, 255);
		color.a = layer->opacity;

		// Create objects bitmap.
		ALLEGRO_BITMAP* objects = al_create_bitmap(w, h);
		al_set_target_bitmap(objects);

		// Prepare to draw texture to render.
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));

		// Iterate over object layer like this because it is a linked list.
		while (head)
		{
			if (head->visible)
			{
				if (head->obj_type == OT_SQUARE)
				{
					// Draw rectangle to object layer.
					al_draw_rectangle(head->x, head->y, head->x + head->width, head->y + head->height, color, LINE_THICKNESS);
					objectIsDrawn = true;
				}
				else if (head->obj_type == OT_POLYGON)
				{
					// Draw polygon object to object layer.
					libtmx::draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
					objectIsDrawn = true;
				}
				else if (head->obj_type == OT_POLYLINE)
				{
					// Draw polyline object to object layer.
					libtmx::draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
					objectIsDrawn = true;
				}
				else if (head->obj_type == OT_ELLIPSE)
				{
					// Draw ellipse object to object layer.
					al_draw_ellipse(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color, LINE_THICKNESS);
					objectIsDrawn = true;
				}
				else if (head->obj_type == OT_TEXT)
				{
					// TO DO.
				}
			}

			// Next in linked list.
			head = head->next;
		}

		// Finish drawing image and restore display.
		al_flip_display();
		al_set_target_backbuffer(sl::Locator::window->getDisplay());

		if (objectIsDrawn)
		{
			// Create entity.
			entt::DefaultRegistry::entity_type entity = sl::Locator::world->m_registry.create();

			// Assign enabled component.
			sl::Locator::world->m_registry.assign<sl::EnabledComponent>(entity);

			// Assign transform component.
			sl::Locator::world->m_registry.assign<sl::TransformComponent>(entity, tmx_get_property(layer->properties, "renderLayer")->value.integer, 0.0f, sl::Rect<float, int>{0.0f, 0.0f, static_cast<int>(w), static_cast<int>(h)});

			// Then add it to the texture atlas ensuring a unique id and assign object to render component.
			std::string id = "TmxObject" + std::to_string(sl::Time::getTimeSinceEpoch());
			sl::Locator::textureAtlas->addTexture(id, objects);
			sl::Locator::world->m_registry.assign<sl::RenderComponent>(entity, 1.0f, id);
		}

		// Cleanup.
		al_destroy_bitmap(objects);
	}

	void tileLayer(tmx_map *map, tmx_layer *layer)
	{
		// Set up variables for layer.
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
		std::string identifier = "";
		ALLEGRO_BITMAP* tileset = nullptr;
		float op = layer->opacity;

		// Set up drawing for the tilemap.
		ALLEGRO_BITMAP* tileLayer = al_create_bitmap(map->width * map->tile_width, map->height * map->tile_height);
		al_set_target_bitmap(tileLayer);
		al_clear_to_color(libtmx::int_to_al_color(map->backgroundcolor, 0));
		al_hold_bitmap_drawing(true);

		// Iterate over each tile in the tilemap.
		for (i = 0; i < map->height; i++)
		{
			for (j = 0; j < map->width; j++)
			{
				// Retrieve the tile grid id, clearing the flags, making sure the tile at that gid is not NULL.
				gid = libtmx::gid_clear_flags(layer->content.gids[(i*map->width) + j]);

				if (map->tiles[gid] != NULL)
				{
					tmx_tile* gidTile = map->tiles[gid];

					// Retrieve tileset, image and position information.
					ts = gidTile->tileset;
					im = gidTile->image;
					x = gidTile->ul_x;
					y = gidTile->ul_y;
					w = ts->tile_width;
					h = ts->tile_height;

					// The tile will have either an image or a tileset for tile texture source.
					if (im)
					{
						identifier = std::filesystem::path(im->source).stem().string();
						tileset = sl::Locator::textureAtlas->al_create_packed_sub_bitmap(identifier);
					}
					else
					{
						identifier = std::filesystem::path(ts->image->source).stem().string();
						tileset = sl::Locator::textureAtlas->al_create_packed_sub_bitmap(identifier);
					}

					// We draw the animated tiles differently.
					if (!(gidTile->animation))
					{
						// Retrieve any flags and pass to the allegro draw function, drawing the tile to the master tilemap layer image.
						flags = libtmx::gid_extract_flags(layer->content.gids[(i*map->width) + j]);
						al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(op, op, op, op), x, y, w, h, j*ts->tile_width, i*ts->tile_height, flags);
					}
					else
					{
						// We get that extact tile from the atlas by offsetting the postion of the tile texture in the atlas by
						// the position of the tile texture on the tilesheet.
						sl::Rect<int> pr = sl::Locator::textureAtlas->get(identifier.c_str());
						x = pr.m_x + gidTile->ul_x;
						y = pr.m_y + gidTile->ul_y;

						// Create unique atlas rectangle for that tile. No need to duplicate data.
						std::string id = "AnimatedTile" + std::to_string(sl::Time::getTimeSinceEpoch());
						sl::Locator::textureAtlas->addRectToAtlas(id, sl::Rect<int>{static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h)});

						// Create entity (ate = animated tile entity).
						entt::DefaultRegistry::entity_type ate = sl::Locator::world->m_registry.create();

						// Assign enabled component.
						sl::Locator::world->m_registry.assign<sl::EnabledComponent>(ate);

						// Assign transform component.
						sl::Locator::world->m_registry.assign<sl::TransformComponent>(ate, tmx_get_property(layer->properties, "renderLayer")->value.integer, 0.0f,
							sl::Rect<float, int>{static_cast<float>(j*ts->tile_width), static_cast<float>(i*ts->tile_height), static_cast<int>(w), static_cast<int>(h)});

						// Assign render component.
						sl::Locator::world->m_registry.assign<sl::RenderComponent>(ate, op, id);

						// Assign animated component.
						sl::Locator::world->m_registry.assign<sl::AnimationComponent>(ate, map, map->tiles[gid], pr.m_x, pr.m_y, w, h);
					}

					// Then destroy the used up tileset.
					al_destroy_bitmap(tileset);
				}
			}
		}

		// Then reset graphics.
		al_hold_bitmap_drawing(false);
		al_flip_display();
		al_set_target_backbuffer(sl::Locator::window->getDisplay());

		// Add new tilemap to atlas.
		std::string tlID = "TileLayer" + std::to_string(sl::Time::getTimeSinceEpoch());
		sl::Locator::textureAtlas->addTexture(tlID, tileLayer);
		al_destroy_bitmap(tileLayer);

		// Create entity for layer tilemap (tle = tile layer entity).
		entt::DefaultRegistry::entity_type tle = sl::Locator::world->m_registry.create();

		// Assign enabled component.
		sl::Locator::world->m_registry.assign<sl::EnabledComponent>(tle);

		// Assign transform component.
		sl::Locator::world->m_registry.assign<sl::TransformComponent>(tle, tmx_get_property(layer->properties, "renderLayer")->value.integer, 0.0f,
			sl::Rect<float, int>(layer->offsetx, layer->offsety, map->width * map->tile_width, map->height * map->tile_height));

		// Assign render component.
		sl::Locator::world->m_registry.assign<sl::RenderComponent>(tle, op, tlID);
	}

	void draw_all_layers(tmx_map *map, tmx_layer *layers) {
		while (layers) {
			if (layers->visible) {
				if (layers->type == L_GROUP) {
					draw_all_layers(map, layers->content.group_head);
				}
				else if (layers->type == L_OBJGR) {
					object(map, layers);
				}
				else if (layers->type == L_IMAGE) {
					image(layers);
				}
				else if (layers->type == L_LAYER) {
					tileLayer(map, layers);
				}
			}
			layers = layers->next;
		}
	}
}

namespace sl
{
	TiledMap::TiledMap()
		:m_map(nullptr), m_buffer("")
	{
	}

	TiledMap::TiledMap(const std::string& map)
		:m_map(nullptr), m_buffer("")
	{
		if (!load(map))
		{
			LOG_S(FATAL) << "Failed to construct map in Constructor: " << map;
		}
	}

	TiledMap::~TiledMap()
	{
		if (m_map)
		{
			tmx_map_free(m_map);
		}

		m_buffer.clear();
	}

	bool TiledMap::load(const std::string& map)
	{
		bool result = true;

		m_buffer = Locator::virtualFS->openAsString(map);
		if (!m_buffer.empty())
		{
			m_map = tmx_load_buffer(m_buffer.c_str(), static_cast<int>(m_buffer.length()));
			if (!m_map)
			{
				LOG_S(ERROR) << "Failed to load map: " << map << ". libtmx Error: " << tmx_strerr();
				result = false;
			}
			else
			{
				if (m_map->orient != tmx_map_orient::O_ORT)
				{
					LOG_S(WARNING) << "Map orientation is not orthographic! Other map types are not supported.";
				}
				else
				{
					libtmx::draw_all_layers(m_map, m_map->ly_head);
				}
			}
		}
		else
		{
			LOG_S(ERROR) << "Failed to load buffer from vfs for file: " << map;
			result = false;
		}

		return result;
	}

	tmx_map* TiledMap::operator()() const
	{
		return m_map;
	}
}