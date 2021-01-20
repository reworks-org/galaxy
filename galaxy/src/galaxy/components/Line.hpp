///
/// Line.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_LINE_HPP_
#define GALAXY_GRAPHICS_SHAPES_LINE_HPP_

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Transform.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Line definition for renderer.
		///
		class Line final : public VertexData, public Transform
		{
		public:
			///
			/// Constructor.
			///
			Line() = default;

			///
			/// Constructor.
			///
			/// \param col Colour of line.
			/// \param x1 X1 position.
			/// \param y1 Y1 position.
			/// \param x2 X2 position.
			/// \param y2 Y2 position.
			///
			Line(graphics::Colour& col, const float x1, const float y1, const float x2, const float y2);

			///
			/// Destructor.
			///
			virtual ~Line() = default;

			///
			/// Create the Line.
			///
			/// \param col Colour of line.
			/// \param x1 X1 position.
			/// \param y1 Y1 position.
			/// \param x2 X2 position.
			/// \param y2 Y2 position.
			/// \param thickness Sets the line thickness, defaulting to 1.
			///
			void create(graphics::Colour& col, const float x1, const float y1, const float x2, const float y2);

			///
			/// Bind as active VA.
			///
			void bind();

			///
			/// Unbind as active VA.
			///
			void unbind();
		};
	} // namespace graphics
} // namespace galaxy

#endif