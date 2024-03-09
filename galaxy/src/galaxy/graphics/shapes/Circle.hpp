///
/// Circle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_CIRCLE_HPP_
#define GALAXY_GRAPHICS_SHAPES_CIRCLE_HPP_

#include "galaxy/graphics/shapes/Shape.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A circle.
		///
		class Circle final : public Shape
		{
		  public:
			///
			/// Constructor.
			///
			Circle();

			///
			/// Move constructor.
			///
			Circle(Circle&&);

			///
			/// Move assignment operator.
			///
			Circle& operator=(Circle&&);

			///
			/// Destructor.
			///
			virtual ~Circle();

			///
			/// Create the circle.
			///
			/// \param fragments Number of points to use. Higher number means smoother shape.
			/// \param radius Radius of circle.
			///
			void create(const float fragments, const float radius);

			///
			/// Get fragments.
			///
			/// \return Const float.
			///
			[[nodiscard]] float fragments() const;

			///
			/// Get radius.
			///
			/// \return Const float.
			///
			[[nodiscard]] float radius() const;

		  private:
			///
			/// Points making up circumference.
			///
			float m_fragments;

			///
			/// Radius.
			///
			float m_radius;
		};
	} // namespace graphics
} // namespace galaxy

#endif
