///
/// Vertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_VERTEX_HPP_
#define GALAXY_GRAPHICS_GL_VERTEX_HPP_

#include <array>

#include <glm/vec2.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	///
	/// Represents a single vertex point.
	///
	struct Vertex final
	{
		///
		/// Position.
		///
		glm::vec2 m_pos;

		///
		/// Texture coords (uv).
		///
		glm::vec2 m_texels;
	};

	namespace graphics
	{
		///
		/// Generate some default verticies.
		///
		/// \param width Width of quad.
		/// \param height Height of quad.
		///
		/// \return Vertices mapped from TOP LEFT to BOTTOM LEFT CLOCKWISE.
		///
		std::array<Vertex, 4> gen_quad_vertices(const float width, const float height) noexcept;

		///
		/// Generate some default indicies.
		///
		/// \return Indices mapping vertices from TOP LEFT to BOTTOM LEFT CLOCKWISE.
		///
		std::array<unsigned int, 6> gen_default_indices() noexcept;

		///
		/// Takes in a x positon texture coord and maps it to a texel.
		///
		/// \tparam Type Type of the variable.
		///
		/// \param x Position of texture coord.
		/// \param width Texture width.
		///
		/// \return OpenGL shader compatible texel.
		///
		template<meta::is_arithmetic Type>
		[[nodiscard]]
		inline float map_x_texel(const Type x, const float width) noexcept
		{
			return static_cast<float>(x) / width;
		}

		///
		/// Takes in a y positon texture coord and maps it to a texel.
		///
		/// \tparam Type Type of the variable.
		///
		/// \param y Position of texture coord.
		/// \param height Texture height.
		///
		/// \return OpenGL shader compatible texel.
		///
		template<meta::is_arithmetic Type>
		[[nodiscard]]
		inline float map_y_texel(const Type y, const float height) noexcept
		{
			return static_cast<float>(y) / height;
		}
	} // namespace graphics
} // namespace galaxy

#endif
