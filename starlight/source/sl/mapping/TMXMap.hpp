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

/// Non-Americanised spelling macro.
#define ALLEGRO_COLOUR ALLEGRO_COLOR

namespace sl
{
	///
	/// Wrapper around a tmx map from tmxlib parser.
	/// Some of the following code is taken from or based on:
	/// https://github.com/baylej/tmx/blob/master/examples/allegro/allegro.c
	/// https://github.com/baylej/tmx/blob/master/examples/dumper/dumper.c
	/// Code is licensed under the same license as the library (tmxlib):
	/// BSD 2-Clause "Simplified" License
	///
	/// Also please note that external tilesets (.tsx) is not currently supported.
	///
	class TMXMap final
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

	private:
		///
		/// Loop through and process all layers.
		///
		/// \param layers ly_head from tmx_map* file.
		///
		void processAllLayers(tmx_layer* layers);

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
		/// When loading texts, fonts are named FontFamily then Size in points.
		/// So when naming fonts make sure they are the same as the font family name specificed in the tmx, WITHOUT SPACES!
		/// So for example, a font family named "Sans Serif" and pixel size of 37 means your table name in fonts.lua should be:
		/// SansSerif27. You can get the font point size and pixel size from the tmx map. 
		/// The info box on the left when selecting text shows u font family and pixel size, and if you click onthe text it will show you font point size.
		/// And ofcourse, the layer must be named "collision" for physics data to be created.
		///
		void processObjects(tmx_map* map, tmx_layer* layer);

		///
		/// Process a tile layer.
		///
		void processLayer(tmx_map* map, tmx_layer* layer);

	public:
		///
		/// The internal tmx_map data structure this class wraps around.
		///
		tmx_map* m_internalMap;

	private:
		///
		/// Map data buffer. This is here to ensure the data tmx_map uses never goes out of scope until object is destroyed.
		/// A std::string is used over a char* array because a std::string can clean up after itself if something goes wrong.
		///
		std::string m_internalMapData;
		
		///
		/// The thickness of the object lines.
		///
		float m_lineThickness;
	};
}

#endif