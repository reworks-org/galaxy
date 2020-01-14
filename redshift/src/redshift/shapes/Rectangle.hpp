///
/// Rectangle.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_RECTANGLE_HPP_
#define REDSHIFT_RECTANGLE_HPP_

///
/// Core namespace.
///
namespace rs
{
	///
	/// Basic Rectangle bounding box data structure.
	///
	class Rectangle
	{
	public:
		///
		/// Default constructor.
		///
		Rectangle() noexcept;

		///
		/// Argument constructor.
		///
		/// \param topLeft Represents the top left corner of a rectangle.
		/// \param bottomRight Represents the bottom right corner of a rectangle.
		///
		explicit Rectangle(const double topLeft, const double bottomRight) noexcept;

		///
		/// Default destructor.
		///
		~Rectangle() = default;

		///
		/// Set top left value.
		///
		/// \param topLeft Represents the top left corner of a rectangle.
		///
		void setTopLeft(const double topLeft) noexcept;

		///
		/// Set bottom right value.
		///
		/// \param bottomRight Represents the bottom right corner of a rectangle.
		///
		void setBottomRight(const double bottomRight) noexcept;

		///
		/// Get top left value.
		///
		/// \return Top left value of rectangle as const double.
		///
		const double getTopLeft() const noexcept;

		///
		/// Get bottom right value.
		///
		/// \return Bottom right value of rectangle as const double.
		///
		const double getBottomRight() const noexcept;

	private:
		///
		/// Represents the top left corner of a rectangle.
		///
		double m_topLeft;

		///
		/// Represents the bottom right corner of a rectangle.
		///
		double m_bottomRight;
	};
}

#endif