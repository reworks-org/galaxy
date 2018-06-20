///
/// Box2DHelper.hpp
/// starlight
///
/// Created by reworks on 12/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_BOX2DHELPER_HPP_
#define STARLIGHT_BOX2DHELPER_HPP_

#include "sl/libs/Box2D/Box2D.h"

namespace sl
{
	///
	/// Integrates Box2D with the rest of the library.
	///
	class Box2DHelper final
	{
	public:
		///
		/// Default Constructor.
		/// Gravity defaults to 9.81.
		///
		Box2DHelper();

		///
		/// Constructor. Sets up Box2D systems.
		///
		/// \param gravity A gravity value for Box2D to use in simulations.
		///
		Box2DHelper(float32 gravity);

		///
		/// Destructor
		///
		~Box2DHelper();

		///
		/// Convert pixels to meters.
		///
		/// \param pixels Pixels to convert.
		///
		/// \return Returns meters.
		///
		template<typename T>
		static constexpr T pixelsToMeters(T pixels);

		///
		/// Convert meters to pixels.
		///
		/// \param meters Meters to convert.
		///
		/// \return Returns pixels.
		///
		template<typename T>
		static constexpr T metersToPixels(T meters);

		///
		/// Convert degrees to radians.
		///
		/// \param deg Degrees to convert.
		///
		/// \return Returns radians.
		///
		template<typename T>
		static constexpr T degToRad(T deg);

		///
		/// Convert radians to degrees.
		///
		/// \param rad Radians to convert.
		///
		/// \return Returns degrees.
		///
		template<typename T>
		static constexpr T radToDeg(T rad);

	public:
		///
		/// The Box2D world in a unique_ptr to ensure deletion when app closes.
		///
		std::unique_ptr<b2World> m_b2world;

		///
		/// Pixels per meter when converting from box2d to pixel units.
		/// If 32 doesnt work, try 30 or 60.
		///
		static constexpr double PIXELS_PER_METER = 32.0;

		///
		/// Value of PI to the 20th place.
		///
		static constexpr double PI = 3.14159265358979323846;
	};

	template<typename T>
	static constexpr T Box2DHelper::pixelsToMeters(T pixels)
	{
		return static_cast<T>(pixels) / static_cast<T>(PIXELS_PER_METER);
	}

	template<typename T>
	static constexpr T Box2DHelper::metersToPixels(T meters)
	{
		return static_cast<T>(meters) * static_cast<T>(PIXELS_PER_METER);
	}

	template<typename T>
	static constexpr T Box2DHelper::degToRad(T deg)
	{
		return (static_cast<T>(PI) * deg) / static_cast<T>(180.0);
	}

	template<typename T>
	static constexpr T Box2DHelper::radToDeg(T rad)
	{
		return (static_cast<T>(180.0) * rad) / static_cast<T>(PI);
	}
}

#endif