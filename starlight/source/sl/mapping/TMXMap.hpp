///
/// TMXMap.hpp
/// starlight
///
/// Created by reworks on 07/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TMXMAP_HPP_
#define STARLIGHT_TMXMAP_HPP_

#include <string>
#include <allegro5/color.h>

#include "sl/libs/tmx/tmx.h"
#include "sl/libs/tmx/tsx.h"

/// American English? Ew.
#define ALLEGRO_COLOUR ALLEGRO_COLOR

namespace sl
{
	///
	/// Some of the following code is taken from or based on:
	/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
	/// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
	/// Code is licensed under the same license as the library (tmxlib):
	/// BSD 2-Clause "Simplified" License
	///
	/// Also please note that external tilesets (.tsx) is not currently supported.
	///
	class TMXMap
	{
	public:
		///
		/// Construct map.
		///
		/// \param mapFile The name of the .tmx in the VFS.
		/// \param lineThickness Thickness of object lines when drawn.
		///
		TMXMap(const std::string& mapFile, float lineThickness = 1.0f);

		///
		/// Cleanup memory.
		///
		~TMXMap();

		///
		/// Loop through and process all layers.
		///
		/// \param layers ly_head from tmx_map* file.
		///
		void processAllLayers(tmx_layer* layers);

	private:
		///
		/// Convert TMX colour to allegro colour.
		///
		ALLEGRO_COLOUR intToColour(int colour, unsigned char opacity);

		///
		/// Draws a line. 
		///
		void drawPolyline(double** points, double x, double y, int pointsc, ALLEGRO_COLOUR colour);

		///
		/// Draws a polygon.
		///
		void drawPolygon(double** points, double x, double y, int pointsc, ALLEGRO_COLOUR colour);

		///
		/// Extracts tile gid flags.
		///
		int gidExtractFlags(unsigned int gid);

		///
		/// Clear gid flags.
		///
		unsigned int gidClearFlags(unsigned int gid);

		///
		/// Process the image layer
		///
		void processImageLayer(tmx_layer* layer);

		///
		/// Process the object layer.
		/// Please note collisions only support objects of type "OT_SQUARE" (rectangles).
		/// When loading texts, fonts must be named: FontfamilySize e.g. Arial18
		///
		void processObjects(tmx_map* map, tmx_layer* layer);

		///
		/// Process a tile layer.
		///
		void processLayer(tmx_map* map, tmx_layer* layer);

	public:
		tmx_map* m_internalMap;

	private:
		std::string m_internalMapData;
		//tmx_tileset_manager* m_externalTilesets;

		float m_lineThickness;
	};
}

#endif