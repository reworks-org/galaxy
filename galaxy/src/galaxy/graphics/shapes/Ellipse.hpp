///
/// Ellipse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_ELLIPSE_HPP_
#define GALAXY_GRAPHICS_SHAPES_ELLIPSE_HPP_

#include "galaxy/graphics/shapes/Shape.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// An elliptical circle.
		///
		class Ellipse final : public Shape
		{
		  public:
			///
			/// Constructor.
			///
			Ellipse();

			///
			/// Move constructor.
			///
			Ellipse(Ellipse&&);

			///
			/// Move assignment operator.
			///
			Ellipse& operator=(Ellipse&&);

			///
			/// Destructor.
			///
			virtual ~Ellipse();

			///
			/// Create the ellipse.
			///
			/// \param fragments How many points to use to create the shape. More means smoother edge.
			/// \param radii Ellipse horizontal and vertical radius.
			///
			void create(const float fragments, const glm::vec2& radii);

			///
			/// Get fragments.
			///
			/// \return Const float.
			///
			[[nodiscard]] float fragments() const;

			///
			/// Get radii.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& radii() const;

		  private:
			///
			/// Number of points used to create shape.
			///
			float m_fragments;

			///
			/// Radius of ellipse.
			///
			glm::vec2 m_radii;
		};
	} // namespace graphics
} // namespace galaxy

#endif
