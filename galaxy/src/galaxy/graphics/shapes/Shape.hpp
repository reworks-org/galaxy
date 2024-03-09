///
/// Shape.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_SHAPE_HPP_
#define GALAXY_GRAPHICS_SHAPES_SHAPE_HPP_

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/gl/VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A generic 2D shape.
		///
		class Shape
		{
		  public:
			///
			/// Constructor.
			///
			Shape();

			///
			/// Move constructor.
			///
			Shape(Shape&&);

			///
			/// Move assignment operator.
			///
			Shape& operator=(Shape&&);

			///
			/// Destructor.
			///
			virtual ~Shape();

			///
			/// Get OpenGL rendering mode.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int mode() const;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float height() const;

			///
			/// Get vertex array object.
			///
			/// \return Const reference to VAO.
			///
			[[nodiscard]] VertexArray& vao();

		  public:
			///
			/// Used by all primitives.
			///
			Colour m_colour;

		  protected:
			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int m_mode;

			///
			/// Cached width.
			///
			float m_width;

			///
			/// Cached height.
			///
			float m_height;

			///
			/// Vertex Array Object.
			///
			VertexArray m_vao;
		};
	} // namespace graphics
} // namespace galaxy

#endif
