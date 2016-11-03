//
//  TMXMap.cpp
//  REngine3
//
//  Created by reworks on 30/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "re/utility/Log.hpp"
#include "re/entity/Entity.hpp"
#include "re/graphics/EllipseShape.hpp"
#include "re/component/PositionComponent.hpp"

#include "TMXMap.hpp"

namespace re
{

	void* sf_tex_loader(const char *path)
	{
		// We allocate on the heap because we need to retain this as long as the map is valid.
		sf::Texture* tex = new sf::Texture;
		tex->loadFromFile(path);
		return (void*)tex;
	}

	void sf_tex_deleter(void* address)
	{
		delete address;
	}

	sf::Color TMXMap::int_to_sf_colour(int colour)
	{
		unsigned char r, g, b;

		// Use bit operations to calculate rgb values.
		r = (colour >> 16) & 0xFF;
		g = (colour >> 8) & 0xFF;
		b = (colour) & 0xFF;

		return sf::Color(r, g, b, 255);
	}

	void TMXMap::draw_polyline(double **points, double x, double y, int pointsc, sf::Color col)
	{
		int i;
		for (i = 1; i<pointsc; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(x + points[i - 1][0], y + points[i - 1][1]), col),
				sf::Vertex(sf::Vector2f(x + points[i][0], y + points[i][1]), col)
			};

			m_batchTexture.draw(line, 2, sf::Lines);
		}
	}

	void TMXMap::draw_polygon(double **points, double x, double y, int pointsc, sf::Color col)
	{
		draw_polyline(points, x, y, pointsc, col);
		if (pointsc > 2)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(x + points[0][0], y + points[0][1]), col),
				sf::Vertex(sf::Vector2f(x + points[pointsc - 1][0], y + points[pointsc - 1][1]), col)
			};

			m_batchTexture.draw(line, 2, sf::Lines);
		}
	}

	void TMXMap::draw_objects(tmx_object_group *objgr)
	{
		sf::Color col = int_to_sf_colour(objgr->color);
		sf::RectangleShape rect;
		re::EllipseShape elip;

		tmx_object *head = objgr->head;

		while (head)
		{
			if (head->visible)
			{
				if (head->shape == S_SQUARE)
				{
					rect.setPosition(head->x, head->y);
					rect.setSize(sf::Vector2f(head->width, head->height));
					
					m_batchTexture.draw(rect);
				}
				else if (head->shape == S_POLYGON)
				{
					draw_polygon(head->points, head->x, head->y, head->points_len, col);
				}
				else if (head->shape == S_POLYLINE)
				{
					draw_polyline(head->points, head->x, head->y, head->points_len, col);
				}
				else if (head->shape == S_ELLIPSE)
				{
					// may need to change
					elip.setPosition(sf::Vector2f(head->x, head->y));
					elip.setRadius(sf::Vector2f(head->width / 2.0, head->height / 2.0));
					elip.setFillColor(col);

					m_batchTexture.draw(elip);
				}
			}

			head = head->next;
		}
	}

	unsigned int TMXMap::gid_clear_flags(unsigned int gid)
	{
		return gid & TMX_FLIP_BITS_REMOVAL;
	}

	void TMXMap::draw_layer(tmx_map *map, tmx_layer *layer)
	{
		unsigned long i, j;
		unsigned int gid;
		float op;

		sf::Texture* tileset;
		tmx_tileset *ts;
		tmx_image *im;

		op = layer->opacity;
		for (i = 0; i < map->height; i++)
		{
			for (j = 0; j < map->width; j++)
			{
				gid = gid_clear_flags(layer->content.gids[(i*map->width) + j]);
				if (map->tiles[gid] != NULL)
				{
					ts = map->tiles[gid]->tileset;
					im = map->tiles[gid]->image;
					
					sf::Vertex* quad = &m_tilemapVertices[gid];
					quad[0].position = sf::Vector2f(j*ts->tile_width, i*ts->tile_height);
					quad[1].position = sf::Vector2f(j*ts->tile_width + ts->tile_width, i*ts->tile_height);
					quad[2].position = sf::Vector2f(j*ts->tile_width + ts->tile_width, i*ts->tile_height + ts->tile_height);
					quad[3].position = sf::Vector2f(j*ts->tile_width, i*ts->tile_height + ts->tile_height);

					quad[0].texCoords = sf::Vector2f(map->tiles[gid]->ul_x, map->tiles[gid]->ul_y);
					quad[1].texCoords = sf::Vector2f(map->tiles[gid]->ul_x + ts->tile_width, map->tiles[gid]->ul_y);
					quad[2].texCoords = sf::Vector2f(map->tiles[gid]->ul_x + ts->tile_width, map->tiles[gid]->ul_y + ts->tile_height);
					quad[3].texCoords = sf::Vector2f(map->tiles[gid]->ul_x, map->tiles[gid]->ul_y + ts->tile_height);

					if (map->tiles[gid]->animation)
					{
						m_animatedTiles.push_back(gid);
					}

					if (im)
					{
						tileset = (sf::Texture*)im->resource_image;
					}
					else
					{
						tileset = (sf::Texture*)ts->image->resource_image;
					}

					m_batchTexture.draw(m_tilemapVertices, tileset);
				}
			}
		}
	}

	void TMXMap::draw_image_layer(tmx_image *img)
	{
		sf::Sprite s(*(sf::Texture*)img->resource_image);
		s.setPosition(0, 0);

		m_batchTexture.draw(s);
	}

	void TMXMap::render_map(tmx_map *map)
	{
		tmx_layer *layers = map->ly_head;
		
		m_batchTexture.clear(int_to_sf_colour(map->backgroundcolor));

		while (layers)
		{
			if (layers->visible)
			{
				if (layers->type == L_OBJGR)
				{
					draw_objects(layers->content.objgr);
				}
				else if (layers->type == L_IMAGE)
				{
					draw_image_layer(layers->content.image);
				}
				else if (layers->type == L_LAYER)
				{
					draw_layer(map, layers);
				}
			}

			layers = layers->next;
		}

		m_batchTexture.display();
	}

	TMXMap::TMXMap()
	:m_tilemapVertices(sf::Quads, 4)
	{
		tmx_img_load_func = sf_tex_loader;
		tmx_img_free_func = sf_tex_deleter;
	}

	TMXMap::TMXMap(const std::string& map)
	:m_tilemapVertices(sf::Quads, 4)
	{
		tmx_img_load_func = sf_tex_loader;
		tmx_img_free_func = sf_tex_deleter;

		m_map = tmx::MakeMap(map);
		if (!m_map.get())
		{
			RE_LOG(LogLevel::FATAL, tmx_strerr());
		}

		m_width = m_map->width * m_map->tile_width;
		m_height = m_map->height * m_map->tile_height;

		m_tilemapVertices.resize(m_width * m_height * 4);
		m_batchTexture.create(m_width, m_height);

		render_map(m_map.get());
	}

	TMXMap::~TMXMap()
	{
		m_tilemapVertices.clear();
		m_tilesToUpdate.clear();
	}

	void TMXMap::Load(const std::string& map)
	{
		m_map = tmx::MakeMap(map);
		if (!m_map.get())
		{
			RE_LOG(LogLevel::FATAL, tmx_strerr());
		}

		m_width = m_map->width * m_map->tile_width;
		m_height = m_map->height * m_map->tile_height;

		m_tilemapVertices.resize(m_width * m_height * 4);
		m_batchTexture.create(m_width, m_height);

		render_map(m_map.get());
	}

	unsigned int TMXMap::CalculateGID(unsigned int px, unsigned int py)
	{
		return gid_clear_flags(px + (py*m_map->width));
	}

	void TMXMap::UpdateTile(unsigned int gid, int tx, int ty)
	{
		m_tilesToUpdate.emplace(gid, std::make_pair(tx, ty));
	}

	void TMXMap::Update(sf::Time dt)
	{
		for (auto& vtgid : m_animatedTiles)
		{
			m_map->tiles[vtgid]->animation_timer += dt.asMilliseconds();

			if (m_map->tiles[vtgid]->animation_timer >= m_map->tiles[vtgid]->animation[m_map->tiles[vtgid]->current_animation].duration)
			{
				if (m_map->tiles[vtgid]->current_animation >= m_map->tiles[vtgid]->animation_len)
				{
					m_map->tiles[vtgid]->current_animation = 0;
				}

				int tx = m_map->tiles[m_map->tiles[vtgid]->animation[m_map->tiles[vtgid]->current_animation].tile_id + 1]->ul_x;
				int ty = m_map->tiles[m_map->tiles[vtgid]->animation[m_map->tiles[vtgid]->current_animation].tile_id + 1]->ul_y;
				tmx_tileset *ts = m_map->tiles[vtgid]->tileset;
				
				sf::Vertex* quad = &m_tilemapVertices[vtgid];
				quad[0].texCoords = sf::Vector2f(tx, ty);
				quad[1].texCoords = sf::Vector2f(tx + ts->tile_width, ty);
				quad[2].texCoords = sf::Vector2f(tx + ts->tile_width, ty + ts->tile_height);
				quad[3].texCoords = sf::Vector2f(tx, ty + ts->tile_height);

				m_map->tiles[vtgid]->current_animation++;
			}
		}

		for (auto& tile : m_tilesToUpdate)
		{
			sf::Vertex* quad = &m_tilemapVertices[tile.first];
			tmx_tileset *ts = m_map->tiles[tile.first]->tileset;

			int tx = tile.second.first;
			int ty = tile.second.second;

			quad[0].texCoords = sf::Vector2f(tx, ty);
			quad[1].texCoords = sf::Vector2f(tx + ts->tile_width, ty);
			quad[2].texCoords = sf::Vector2f(tx + ts->tile_width, ty + ts->tile_height);
			quad[3].texCoords = sf::Vector2f(tx, ty + ts->tile_height);
		}

		m_tilesToUpdate.clear();
	}

	void TMXMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Sprite sprite(m_batchTexture.getTexture());

		states.shader = nullptr;
		states.texture = nullptr;
		states.transform *= getTransform();

		target.draw(sprite, states);
	}
}

// I think the issue is objects are getting destroyed. better fix that :3