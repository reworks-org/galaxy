///
/// TMXUtils.hpp
/// starlight
///
/// Created by reworks on 07/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TMXUTILS_HPP_
#define STARLIGHT_TMXUTILS_HPP_

#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>

#include "tmx/tmx.h"
#include "sl/core/World.hpp"
#include "sl/utils/Utils.hpp"
#include "sl/graphics/TextureAtlas.hpp"
#include "sl/components/SpriteComponent.hpp"
#include "sl/components/TransformComponent.hpp"

/// Sorry...can't stand american english...
#define ALLEGRO_COLOUR ALLEGRO_COLOR

///
/// Most of the following code is taken from:
/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
/// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
/// Code is licensed under the same license as the library (tmxlib):
/// BSD 2-Clause "Simplified" License
///
namespace sl
{
	namespace tmxutils
	{
		///
		/// The line thickness to draw objects with.
		/// Defaults to 1.0f.
		///
		static inline float LINE_THICKNESS = 1.0f;

		///
		/// Convert TMX colour to allegro colour.
		///
		static inline ALLEGRO_COLOUR intToColour(int colour, unsigned char opacity)
		{
			unsigned char r, g, b;

			r = (color >> 16) & 0xFF;
			g = (color >> 8) & 0xFF;
			b = (color) & 0xFF;

			return al_map_rgba(r, g, b, opacity);
		}

		///
		/// Draws a line. 
		///
		static inline void drawPolyline(double **points, double x, double y, int pointsc, ALLEGRO_COLOUR colour)
		{
			int i;
			for (i = 1; i<pointsc; i++) 
			{
				al_draw_line(x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1], colour, LINE_THICKNESS);
			}
		}

		///
		/// Draws a polygon.
		///
		static inline void drawPolygon(double **points, double x, double y, int pointsc, ALLEGRO_COLOUR colour)
		{
			drawPolyline(points, x, y, pointsc, color);
			if (pointsc > 2)
			{
				al_draw_line(x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1], colour, LINE_THICKNESS);
			}
		}

		///
		/// Draws tmx objects.
		///
		static inline void drawObjects(tmx_object_group *objgr)
		{
			ALLEGRO_COLOR color = intToColour(objgr->color);
			tmx_object *head = objgr->head;

			while (head)
			{
				if (head->visible)
				{
					if (head->obj_type == OT_SQUARE)
					{
						al_draw_rectangle(head->x, head->y, head->x + head->width, head->y + head->height, color, LINE_THICKNESS);
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
						al_draw_ellipse(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color, LINE_THICKNESS);
					}
				}

				head = head->next;
			}
		}

		///
		/// Extracts tile gid flags.
		///
		static inline int gidExtractFlags(unsigned int gid)
		{
			int res = 0;

			if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
			if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
			/* FIXME allegro has no diagonal flip */
			return res;
		}

		///
		/// Clear gid flags.
		///
		static inline unsigned int gidClearFlags(unsigned int gid)
		{
			return gid & TMX_FLIP_BITS_REMOVAL;
		}

		///
		/// Process the image layer(s).
		///
		static inline void processImageLayers(tmx_layer* layer)
		{
			float op = layer->opacity;

			entt::Entity entity = World::inst()->m_registry.create();
			World::inst()->m_registry.assign<SpriteComponent>(entity, utils::removeExtension(layer->content.image->source), op);
			World::inst()->m_registry.assign<TransformComponent>(entity, tmx_get_property(layer->properties, "layer")->value.integer, 0.0f, {layer->offsetx, layer->offsety, layer->content.image->width, layer->content.image->height});
			World::inst()->m_inUse.push_back(entity);
		}

		static inline void drawLayer(tmx_map *map, tmx_layer *layer)
		{
			unsigned long i, j;
			unsigned int gid, x, y, w, h, flags;
			float op;
			tmx_tileset *ts;
			tmx_image *im;
			ALLEGRO_BITMAP *tileset;
			op = layer->opacity;
			for (i = 0; i<map->height; i++)
			{
				for (j = 0; j<map->width; j++)
				{
					gid = gid_clear_flags(layer->content.gids[(i*map->width) + j]);
					if (map->tiles[gid] != NULL) {
						ts = map->tiles[gid]->tileset;
						im = map->tiles[gid]->image;
						x = map->tiles[gid]->ul_x;
						y = map->tiles[gid]->ul_y;
						w = ts->tile_width;
						h = ts->tile_height;
						if (im)
						{
							tileset = (ALLEGRO_BITMAP*)im->resource_image;
						}
						else
						{
							tileset = (ALLEGRO_BITMAP*)ts->image->resource_image;
						}

						flags = gid_extract_flags(layer->content.gids[(i*map->width) + j]);
						al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(op, op, op, op), x, y, w, h, j*ts->tile_width, i*ts->tile_height, flags);
					}
				}
			}
		}
	}
}