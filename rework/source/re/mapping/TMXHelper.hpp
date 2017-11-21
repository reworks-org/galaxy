//
//  TMXHelper.hpp
//  rework
//
//  Created by reworks on 17/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TMXHELPER_HPP_
#define REWORK_TMXHELPER_HPP_

#include <functional>

#include <allegro5/color.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/allegro_primitives.h>

#include "tmx/tmx.h"

namespace re
{
	namespace tmx
	{
		///
		/// Line thickness for objects.
		///
		static float LINE_THICKNESS = 2.5;

		///
		/// Sets up tmx loading / destruction functions for allegro images.
		///
		static inline void setUpLoaders()
		{
			tmx_img_load_func = al_img_loader;
			tmx_img_free_func = (void(*)(void*))al_destroy_bitmap;
		}

		///
		/// Wrapper around al_img_loader.
		///
		void* al_img_loader(const char* path)
		{
			return (void*)al_load_bitmap(path);
		}

		///
		/// Convert TMX colour to allegro colour. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline ALLEGRO_COLOR int_to_al_color(int color)
		{
			unsigned char r, g, b;

			r = (color >> 16) & 0xFF;
			g = (color >> 8) & 0xFF;
			b = (color) & 0xFF;

			return al_map_rgb(r, g, b);
		}

		///
		/// Draws a line. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline void draw_polyline(double **points, double x, double y, int pointsc, ALLEGRO_COLOR color)
		{
			int i;
			for (i = 1; i<pointsc; i++) {
				al_draw_line(x + points[i - 1][0], y + points[i - 1][1], x + points[i][0], y + points[i][1], color, LINE_THICKNESS);
			}
		}

		///
		/// Draws a polygon. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline void draw_polygon(double **points, double x, double y, int pointsc, ALLEGRO_COLOR color)
		{
			draw_polyline(points, x, y, pointsc, color);
			if (pointsc > 2) {
				al_draw_line(x + points[0][0], y + points[0][1], x + points[pointsc - 1][0], y + points[pointsc - 1][1], color, LINE_THICKNESS);
			}
		}

		///
		/// Draws tmx objects. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline void draw_objects(tmx_object_group *objgr)
		{
			ALLEGRO_COLOR color = int_to_al_color(objgr->color);
			tmx_object *head = objgr->head;
			while (head) {
				if (head->visible) {
					if (head->obj_type == OT_SQUARE) {
						al_draw_rectangle(head->x, head->y, head->x + head->width, head->y + head->height, color, LINE_THICKNESS);
					}
					else if (head->obj_type == OT_POLYGON) {
						draw_polygon(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
					}
					else if (head->obj_type == OT_POLYLINE) {
						draw_polyline(head->content.shape->points, head->x, head->y, head->content.shape->points_len, color);
					}
					else if (head->obj_type == OT_ELLIPSE) {
						al_draw_ellipse(head->x + head->width / 2.0, head->y + head->height / 2.0, head->width / 2.0, head->height / 2.0, color, LINE_THICKNESS);
					}
				}
				head = head->next;
			}
		}

		///
		/// Extracts tile gid flags. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline int gid_extract_flags(unsigned int gid)
		{
			int res = 0;

			if (gid & TMX_FLIPPED_HORIZONTALLY) res |= ALLEGRO_FLIP_HORIZONTAL;
			if (gid & TMX_FLIPPED_VERTICALLY)   res |= ALLEGRO_FLIP_VERTICAL;
			/* FIXME allegro has no diagonal flip */
			return res;
		}

		///
		/// Clear gid flags. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline unsigned int gid_clear_flags(unsigned int gid)
		{
			return gid & TMX_FLIP_BITS_REMOVAL;
		}

		///
		/// Draw a layer. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline void draw_layer(tmx_map *map, tmx_layer *layer)
		{
			unsigned long i, j;
			unsigned int gid, x, y, w, h, flags;
			float op;
			tmx_tileset *ts;
			tmx_image *im;
			ALLEGRO_BITMAP *tileset;
			op = layer->opacity;
			for (i = 0; i<map->height; i++) {
				for (j = 0; j<map->width; j++) {
					gid = gid_clear_flags(layer->content.gids[(i*map->width) + j]);
					if (map->tiles[gid] != NULL) {
						ts = map->tiles[gid]->tileset;
						im = map->tiles[gid]->image;
						x = map->tiles[gid]->ul_x;
						y = map->tiles[gid]->ul_y;
						w = ts->tile_width;
						h = ts->tile_height;
						if (im) {
							tileset = (ALLEGRO_BITMAP*)im->resource_image;
						}
						else {
							tileset = (ALLEGRO_BITMAP*)ts->image->resource_image;
						}
						flags = gid_extract_flags(layer->content.gids[(i*map->width) + j]);
						al_draw_tinted_bitmap_region(tileset, al_map_rgba_f(op, op, op, op), x, y, w, h, j*ts->tile_width, i*ts->tile_height, flags);
					}
				}
			}
		}

		///
		/// Draw all layer. Code by:
		/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
		/// Same license as tmx library.
		///
		static inline void draw_all_layer_types(tmx_map *map, tmx_layer *layers) {
			while (layers) {
				if (layers->visible) {
					if (layers->type == L_GROUP) {
						draw_all_layer_types(map, layers->content.group_head);
					}
					else if (layers->type == L_OBJGR) {
						draw_objects(layers->content.objgr);
					}
					else if (layers->type == L_IMAGE) {
						if (layers->opacity < 1.) {
							float op = layers->opacity;
							al_draw_tinted_bitmap((ALLEGRO_BITMAP*)layers->content.image->resource_image, al_map_rgba_f(op, op, op, op), 0, 0, 0);
						}
						al_draw_bitmap((ALLEGRO_BITMAP*)layers->content.image->resource_image, 0, 0, 0);
					}
					else if (layers->type == L_LAYER) {
						draw_layer(map, layers);
					}
				}
				layers = layers->next;
			}
		}
	}
}

#endif