///
/// Vertex3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_VERTEX3D_HPP_
#define GALAXY_GRAPHICS_VERTEX_VERTEX3D_HPP_

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single vertex of a 3D object .
		///
		class Vertex3D final
		{
		public:
			///
			/// Default constructor.
			///
			Vertex3D() noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Coords (x, y, z) as a vec3.
			/// \param normal Normal vector of vertex.
			/// \param texels Texture coords (u, v) as a vec2.
			///
			Vertex3D(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texels) noexcept;

			///
			/// Destructor.
			///
			~Vertex3D() noexcept = default;

		public:
			///
			/// Position.
			///
			glm::vec3 m_pos;

			///
			/// Normal vector.
			///
			glm::vec3 m_normal;

			///
			/// Texels.
			///
			glm::vec2 m_texels;
		};
	} // namespace graphics
} // namespace galaxy

#endif