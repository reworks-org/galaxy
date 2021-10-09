///
/// PrimitiveVertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_PRIMITIVEVERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_PRIMITIVEVERTEX_HPP_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single PrimitiveVertex of a primitive.
		///
		class PrimitiveVertex final
		{
		public:
			///
			/// Default constructor.
			///
			PrimitiveVertex() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X pos.
			/// \param y Y pos.
			/// \param col Colour.
			///
			PrimitiveVertex(const float x, const float y, const graphics::Colour& col) noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Position.
			/// \param col Colour.
			///
			PrimitiveVertex(const glm::vec2& pos, const graphics::Colour& col) noexcept;

			///
			/// Destructor.
			///
			~PrimitiveVertex() noexcept = default;

		public:
			///
			/// Position of PrimitiveVertex.
			///
			glm::vec2 m_pos;

			///
			/// Colour of PrimitiveVertex.
			///
			glm::vec4 m_colour;
		};
	} // namespace graphics
} // namespace galaxy

#endif