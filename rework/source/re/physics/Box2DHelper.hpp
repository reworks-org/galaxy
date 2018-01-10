///
///  Box2DHelper.hpp
///  rework
///
///  Created by reworks on 22/09/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_BOX2DHELPER_HPP_
#define REWORK_BOX2DHELPER_HPP_

namespace re
{
	namespace b2
	{
		/// If 32 doesnt work, try 30 or 60.
		static constexpr double PIXELS_PER_METER = 32.0;
		static constexpr double PI = 3.14159265358979323846;

		///
		/// Convert pixels to meters.
		///
		/// \param pixels Pixels to convert.
		///
		/// \return Returns meters.
		///
		template<typename T>
		constexpr T pixelsToMeters(T pixels)
		{
			return static_cast<T>(pixels) / static_cast<T>(RE_PIXELS_PER_METERS);
		}

		///
		/// Convert meters to pixels.
		///
		/// \param meters Meters to convert.
		///
		/// \return Returns pixels.
		///
		template<typename T>
		constexpr T metersToPixels(T meters)
		{
			return static_cast<T>(meters) * static_cast<T>(RE_PIXELS_PER_METERS);
		}

		///
		/// Convert degrees to radians.
		///
		/// \param deg Degrees to convert.
		///
		/// \return Returns radians.
		///
		template<typename T>
		constexpr T degToRad(T deg)
		{
			return (static_cast<T>(RE_PI) * deg) / static_cast<T>(180.0);
		}

		///
		/// Convert radians to degrees.
		///
		/// \param rad Radians to convert.
		///
		/// \return Returns degrees.
		///
		template<typename T>
		constexpr T radToDeg(T rad)
		{
			return (static_cast<T>(180.0) * rad) / static_cast<T>(RE_PI);
		}
	}
}

#endif