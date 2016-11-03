//
//  TMXMap.hpp
//  REngine3
//
//  Created by reworks on 30/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_TMXMAP_HPP_
#define RENGINE3_TMXMAP_HPP_

#include <map>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "re/mapping/TMXSmartPointer.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	class Entity;

	class TMXMap : public sf::Drawable, public sf::Transformable
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		TMXMap();

		/*
		* IMPORTS: map - The name of the map in .tmx format. Does not use VFS.
		* EXPORTS: none
		* PURPOSE: Sets up the map using tmx-parser.
		*/
		TMXMap(const std::string& map);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Destructor. Clean up map.
		*/
		~TMXMap();

		/*
		* IMPORTS: map - The path to the map in .tmx format. Does not use VFS.
		* EXPORTS: none
		* PURPOSE: Load a new map. Use this if you use the default constructor.
		*/
		void Load(const std::string& map);
		
		/*
		* IMPORTS: px and py are the players coords.
		* EXPORTS: tile gid.
		* PURPOSE: To get the gid (grid id) of a tile on the map.
		*/
		unsigned int CalculateGID(unsigned int px, unsigned int py);

		/*
		* IMPORTS: grid id of tile to update and the new texture top-left coords.
		* EXPORTS: none
		* PURPOSE: Update the data in the master array about a particular tile.
		*/
		void UpdateTile(unsigned int gid, int tx, int ty);

		/*
		* IMPORTS: delta time.
		* EXPORTS: none
		* PURPOSE: Update the map data e.g. animations, collisions and stuff.
		*/
		void Update(sf::Time dt);

	private:
	    /*
		* BEGIN TMX UTILITY FUNCTIONS
		* SEE: https://github.com/baylej/tmx/tree/master/examples
		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		sf::Color int_to_sf_colour(int colour);
		void draw_polyline(double **points, double x, double y, int pointsc, sf::Color col);
		void draw_polygon(double **points, double x, double y, int pointsc, sf::Color col);
		void draw_objects(tmx_object_group *objgr);
		unsigned int gid_clear_flags(unsigned int gid);
		void draw_layer(tmx_map *map, tmx_layer *layer);
		void draw_image_layer(tmx_image *img);
		void render_map(tmx_map *map);
		/*
		* END TMX UTILITY FUNCTIONS
		*/
	private:
		int m_width;
		int m_height;

		tmx::UPtr m_map;
		sf::RenderTexture m_batchTexture;
		sf::VertexArray m_tilemapVertices;
		std::map<unsigned int, std::pair<int, int>> m_tilesToUpdate;
		std::vector<unsigned int> m_animatedTiles;
	};
}

#endif