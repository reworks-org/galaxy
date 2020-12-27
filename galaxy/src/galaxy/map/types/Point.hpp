///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_POINT_HPP_
#define GALAXY_MAP_TYPES_POINT_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace map
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
			Point();

			///
			/// Parameter constructor.
			///
			/// \param x X coordinate in pixels.
			/// \param y Y coordinate in pixels.
			///
			explicit Point(const double x, const double y);

			///
			/// \brief Parse constructor.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure/array containing object.
			///
			explicit Point(const nlohmann::json& json);

			///
			/// Default destructor.
			///
			~Point() = default;

			///
			/// \brief Parse object level json.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure/array containing object.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get the x coord.
			///
			/// \return const double x coord in pixels.
			///
			[[nodiscard]] const double get_x() const;

			///
			/// Get the y coord.
			///
			/// \return const double y coord in pixels.
			///
			[[nodiscard]] const double get_y() const;

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
	} // namespace map
} // namespace galaxy

#endif