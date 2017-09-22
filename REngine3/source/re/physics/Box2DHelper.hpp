//
//  Box2DHelper.hpp
//  REngine3
//
//  Created by reworks on 22/09/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_BOX2DHELPER_HPP_
#define RENGINE3_BOX2DHELPER_HPP_

// If 32 doesnt work, try 30 or 60.
#define RE_PIXELS_PER_METERS 32.0f
#define RE_PI 3.14159265358979323846f

namespace re
{
	///
	/// Convert pixels to meters.
	///
	/// \param pixels Pixels to convert.
	///
	/// \return Returns meters.
	///
	template<typename T>
	T pixelsToMeters(T pixels)
	{
		return pixels / RE_PIXELS_PER_METERS;
	}

	///
	/// Convert meters to pixels.
	///
	/// \param meters Meters to convert.
	///
	/// \return Returns pixels.
	///
	template<typename T>
	T metersToPixels(T meters)
	{
		return meters * RE_PIXELS_PER_METERS;
	}

	///
	/// Convert degrees to radians.
	///
	/// \param deg Degrees to convert.
	///
	/// \return Returns radians.
	///
	template<typename T>
	T degToRad(T deg)
	{
		return (RE_PI * deg) / 180.0f;
	}

	///
	/// Convert radians to degrees.
	///
	/// \param rad Radians to convert.
	///
	/// \return Returns degrees.
	///
	template<typename T>
	T radToDeg(T rad)
	{
		return (180.0f * rad) / RE_PI;
	}
}

#endif