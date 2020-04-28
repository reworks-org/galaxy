///
/// Point.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_POINT_HPP_
#define STARMAP_POINT_HPP_

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// A point on a polygon or a polyline, relative to the position of the object.
	/// This Point class is specific to Tiled.
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
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing object.
		///
		explicit Point(const nlohmann::json& json) noexcept;

		///
		/// Default destructor.
		///
		~Point() noexcept = default;

		///
		/// \brief Parse object level json.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing object.
		///
		void parse(const nlohmann::json& json) noexcept;

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