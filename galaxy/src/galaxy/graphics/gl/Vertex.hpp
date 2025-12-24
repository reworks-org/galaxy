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
#include <glm/vec3.hpp>

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
		glm::vec3 m_pos;

		///
		/// Texture coords (uv).
		///
		glm::vec2 m_texels;

		///
		/// Texture handle.
		///
		std::uint64_t m_handle;
	};

	namespace graphics
	{
		///
		/// Generate some default verticies.
		///
		/// \param width Width of quad.
		/// \param height Height of quad.
		/// \param depth Rendering layer. Clamped from -1.0 to 1.0.
		/// \param handle Texture handle.
		///
		/// \return Vertices mapped from TOP LEFT to BOTTOM LEFT CLOCKWISE.
		///
		std::vector<Vertex> gen_quad_vertices(const float width, const float height, float depth, const std::uint64_t handle) noexcept;

		///
		/// Generate some default indices.
		///
		/// \return Indices mapping vertices from TOP LEFT to BOTTOM LEFT CLOCKWISE.
		///
		std::vector<unsigned int> gen_default_indices() noexcept;

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
