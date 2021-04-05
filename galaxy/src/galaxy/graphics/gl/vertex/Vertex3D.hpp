///
/// Vertex3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_TYPE_Vertex3D_HPP_
#define GALAXY_GRAPHICS_VERTEX_TYPE_Vertex3D_HPP_

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
			/// \param x X pos.
			/// \param y Y pos.
			/// \param z Z pos.
			/// \param u Texel x coord.
			/// \param v Texel y coord.
			///
			Vertex3D(const float x, const float y, const float z, const float u, const float v) noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Coords (x, y, z) as a vec3.
			/// \param u Texel x coord.
			/// \param v Texel y coord.
			///
			Vertex3D(const glm::vec3& pos, const float u, const float v) noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X pos.
			/// \param y Y pos.
			/// \param z Z pos.
			/// \param texels Texture coords (u, v) as a vec2.
			///
			Vertex3D(const float x, const float y, const float z, const glm::vec2& texels) noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Coords (x, y, z) as a vec3.
			/// \param texels Texture coords (u, v) as a vec2.
			///
			Vertex3D(const glm::vec3& pos, const glm::vec2& texels) noexcept;

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
			/// Texels.
			///
			glm::vec2 m_texels;
		};
	} // namespace graphics
} // namespace galaxy

#endif