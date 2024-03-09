///
/// Polygon.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_POLYGON_HPP_
#define GALAXY_GRAPHICS_SHAPES_POLYGON_HPP_

#include "galaxy/graphics/shapes/Shape.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A closed loop line collection of points. I.e. A square, triangle, etc.
		///
		class Polygon final : public Shape
		{
		  public:
			///
			/// Constructor.
			///
			Polygon();

			///
			/// Move constructor.
			///
			Polygon(Polygon&&);

			///
			/// Move assignment operator.
			///
			Polygon& operator=(Polygon&&);

			///
			/// Destructor.
			///
			virtual ~Polygon();

			///
			/// Create shape.
			///
			/// \param points List of points. The first point and the last point will be joined.
			///
			void create(const meta::vector<glm::vec2>& points);

			///
			/// Get list of points.
			///
			/// \return List of points making up shape.
			///
			[[nodiscard]] const meta::vector<glm::vec2>& points() const;

		  private:
			///
			/// List of points.
			///
			meta::vector<glm::vec2> m_points;
		};
	} // namespace graphics
} // namespace galaxy

#endif
