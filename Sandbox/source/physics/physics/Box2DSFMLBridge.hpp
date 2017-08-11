//
//  Box2DSFMLBridge.hpp
//  REngine3
//
//  Created by reworks on 12/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//
// Thanks to "SFML Blueprints" book.

#ifndef RENGINE3_BOX2DSFMLBRIDGE_HPP_
#define RENGINE3_BOX2DSFMLBRIDGE_HPP_

namespace re
{
	namespace b2
	{
		static const float PIXELS_PER_METERS = 32.0f;
		static const float PI = 3.14159265358979323846f;

		/*
		* IMPORTS: pixels
		* EXPORTS: meters
		* PURPOSE: Convert pixels to meters.
		*/
		template<typename T>
		T pixelsToMeters(T pixels)
		{
			return pixels / PIXELS_PER_METERS;
		}

		/*
		* IMPORTS: meters
		* EXPORTS: pixels
		* PURPOSE: convert meters to pixels.
		*/
		template<typename T>
		T metersToPixels(T meters)
		{
			return meters * PIXELS_PER_METERS;
		}

		/*
		* IMPORTS: degress
		* EXPORTS: radians
		* PURPOSE: convert degrees to radians.
		*/
		template<typename T>
		T degToRad(T deg)
		{
			return (PI * deg) / 180.0f;
		}

		/*
		* IMPORTS: radians
		* EXPORTS: degrees
		* PURPOSE: convert radians to degrees.
		*/
		template<typename T>
		T radToDeg(T rad)
		{
			return (180.0f * rad) / PI;
		}
	}
}

#endif
