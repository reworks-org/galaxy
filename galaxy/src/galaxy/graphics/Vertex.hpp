///
/// Vertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_HPP_

#include <array>
#include <compare>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single vertex point.
		///
		class Vertex final
		{
		public:
			///
			/// Constructor.
			///
			Vertex() noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Position of vertex.
			///
			Vertex(const glm::vec2& pos) noexcept;

			///
			/// Reverse argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param texels Vertex texture coordinates.
			///
			Vertex(const glm::vec2& pos, const glm::vec2& texels) noexcept;

			///
			/// Reverse argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param colour Colour of vertex, and opacity.
			///
			Vertex(const glm::vec2& pos, const graphics::Colour& colour) noexcept;

			///
			/// Reverse argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param texels Vertex texture coordinates.
			/// \param colour Colour of vertex, and opacity.
			///
			Vertex(const glm::vec2& pos, const glm::vec2& texels, const graphics::Colour& colour) noexcept;

			///
			/// Move constructor.
			///
			Vertex(Vertex&&) noexcept;

			///
			/// Move assignment operator.
			///
			Vertex& operator=(Vertex&&) noexcept;

			///
			/// Copy constructor.
			///
			Vertex(const Vertex&) noexcept;

			///
			/// Copy assignment operator.
			///
			Vertex& operator=(const Vertex&) noexcept;

			///
			/// Destructor.
			///
			~Vertex() noexcept = default;

			///
			/// Generate some default verticies.
			///
			/// \param width Width of quad.
			/// \param height Height of quad.
			///
			/// \return Vertices mapped from TOP LEFT to BOTTOM LEFT CLOCKWISE.
			///
			static std::array<Vertex, 4> gen_quad_vertices(const float width, const float height) noexcept;

			///
			/// Generate some default indicies.
			///
			/// \return Indices mapping vertices from TOP LEFT to BOTTOM LEFT CLOCKWISE.
			///
			static const std::array<unsigned int, 6>& get_default_indices() noexcept;

			///
			/// Comparison operator.
			///
			[[nodiscard]] auto operator<=>(const Vertex&) const = default;

		public:
			///
			/// Position of Vertex.
			///
			glm::vec2 m_pos;

			///
			/// Vertex texels.
			///
			glm::vec2 m_texels;

			///
			/// RGBA colour.
			///
			graphics::Colour m_colour;
		};
	} // namespace graphics
} // namespace galaxy

#endif