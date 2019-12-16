///
/// Point.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_POINT_HPP_
#define STARMAP_POINT_HPP_

///
/// Core namespace.
///
namespace starmap
{
	///
	/// A point on a polygon or a polyline, relative to the position of the object.
	///
	class Point final
	{
	public:
		///
		/// Default constructor.
		///
		Point() noexcept;

		///
		/// Parameter constructor.
		///
		/// \param x X coordinate in pixels.
		/// \param y Y coordinate in pixels.
		///
		explicit Point(const double x, const double y) noexcept;

		///
		/// Copy constructor.
		///
		Point(const Point&) = default;

		///
		/// Move constructor.
		///
		Point(Point&&) = default;

		///
		/// Default destructor.
		///
		~Point() noexcept = default;

		///
		/// Set the X coord.
		///
		/// \param x X coordinate in pixels.
		///
		void setX(const double x) noexcept;

		///
		/// Set the Y coord.
		///
		/// \param y Y coordinate in pixels.
		///
		void setY(const double y) noexcept;

		///
		/// Get the x coord.
		///
		/// \return const double x coord in pixels.
		///
		const double getX() const noexcept;

		///
		/// Get the y coord.
		///
		/// \return const double y coord in pixels.
		///
		const double getY() const noexcept;

	private:
		///
		/// X coordinate in pixels.
		///
		double m_x;

		///
		/// Y coordinate in pixels.
		///
		double m_y;
	};
}

#endif