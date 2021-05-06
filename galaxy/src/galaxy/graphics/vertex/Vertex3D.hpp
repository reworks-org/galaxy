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
		/// Represents a single vertex of a 3D object.
		///
		class Vertex3D final
		{
		public:
			///
			/// Default constructor.
			///
			Vertex3D() noexcept;

			///
			/// Destructor.
			///
			~Vertex3D() noexcept = default;

			///
			/// Set index.
			///
			/// \param index Index in order to sort vertex.
			///
			void set_index(const unsigned int index) noexcept;

			///
			/// Get index.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int get_index() const noexcept;

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
			/// Tangents vector.
			///
			glm::vec3 m_tangents;

			///
			/// Texels.
			///
			glm::vec2 m_texels;

		private:
			///
			/// Used for ordering.
			///
			unsigned int m_index;
		};
	} // namespace graphics
} // namespace galaxy

#endif