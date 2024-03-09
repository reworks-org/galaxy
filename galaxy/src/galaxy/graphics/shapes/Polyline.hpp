///
/// Polyline.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_POLYLINE_HPP_
#define GALAXY_GRAPHICS_SHAPES_POLYLINE_HPP_

#include "galaxy/graphics/shapes/Shape.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// An open looped line of points.
		///
		class Polyline final : public Shape
		{
		  public:
			///
			/// Constructor.
			///
			Polyline();

			///
			/// Move constructor.
			///
			Polyline(Polyline&&);

			///
			/// Move assignment operator.
			///
			Polyline& operator=(Polyline&&);

			///
			/// Destructor.
			///
			virtual ~Polyline();

			///
			/// List of points to create a line from.
			///
			/// \param points List of points. Will not close or loop.
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
