///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_POINT_HPP_
#define GALAXY_GRAPHICS_SHAPES_POINT_HPP_

#include "galaxy/graphics/shapes/Shape.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A single point.
		///
		class Point final : public Shape
		{
		  public:
			///
			/// Constructor.
			///
			Point();

			///
			/// Move constructor.
			///
			Point(Point&&) = default;

			///
			/// Move assignment operator.
			///
			Point& operator=(Point&&) = default;

			///
			/// Destructor.
			///
			virtual ~Point();

			///
			/// Initial point positon.
			///
			/// \param pos X/Y coords.
			///
			void create(const glm::vec2& pos);

			///
			/// Get pos.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& pos() const;

		  private:
			///
			/// Position.
			///
			glm::vec2 m_pos;
		};
	} // namespace graphics
} // namespace galaxy

#endif
