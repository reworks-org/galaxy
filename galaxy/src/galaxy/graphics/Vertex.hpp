///
/// Vertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_HPP_

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
			Vertex();

			///
			/// Argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param texels Vertex texture coordinates.
			/// \param normals Vertex Normalmap texels.
			/// \param colour Colour of vertex, and opacity.
			///
			Vertex(const glm::vec2& pos, const glm::vec2& texels = {}, const glm::vec2& normals = {}, const graphics::Colour& colour = {});

			///
			/// Move constructor.
			///
			Vertex(Vertex&&);

			///
			/// Move assignment operator.
			///
			Vertex& operator=(Vertex&&);

			///
			/// Copy constructor.
			///
			Vertex(const Vertex&);

			///
			/// Copy assignment operator.
			///
			Vertex& operator=(const Vertex&);

			///
			/// Destructor.
			///
			~Vertex() = default;

			///
			/// Generate some default verticies.
			///
			/// \param width Width of quad.
			/// \param height Height of quad.
			///
			/// \return Vertices mapped from TOP LEFT to BOTTOM LEFT CLOCKWISE.
			///
			static std::array<Vertex, 4> gen_quad_vertices(const int width, const int height);

			///
			/// Generate some default indicies.
			///
			/// \return Indices mapping vertices from TOP LEFT to BOTTOM LEFT CLOCKWISE.
			///
			static std::array<unsigned int, 6>& get_default_indices();

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