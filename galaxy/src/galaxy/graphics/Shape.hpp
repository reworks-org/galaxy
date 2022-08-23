///
/// Shape.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPE_HPP_
#define GALAXY_GRAPHICS_SHAPE_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Basic shapes.
		///
		enum class Shape : short
		{
			///
			/// A circlular shape.
			///
			CIRCLE = 0,

			///
			/// An odd circle.
			///
			ELLIPSE = 1,

			///
			/// A line that stretches between only two points.
			///
			LINE = 2,

			///
			/// A single vertex point.
			///
			POINT = 3,

			///
			/// A line that stretches over a list of points.
			///
			POLYLINE = 4,

			///
			/// A closed loop line, i.e. a square.
			///
			POLYGON = 5
		};
	} // namespace graphics
} // namespace galaxy

#endif