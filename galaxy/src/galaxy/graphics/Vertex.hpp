///
/// Vertex.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_HPP_
#define GALAXY_GRAPHICS_VERTEX_HPP_

#include <compare>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents a single vertex.
		///
		class Vertex final
		{
			friend class VertexLayout;

		public:
			///
			/// Constructor.
			///
			Vertex() noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param depth Z-Level. 0 - 1000.
			/// \param texels Vertex texture coordinates.
			/// \param colour Colour of vertex, and opacity.
			///
			Vertex(const glm::vec2& pos, const int depth, const glm::vec2& texels = {0.0f, 0.0f}, const graphics::Colour& colour = {0, 0, 0, 1}) noexcept;

			///
			/// Reverse argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param depth Z-Level. 0 - 1000.
			/// \param colour Colour of vertex, and opacity.
			/// \param texels Vertex texture coordinates.
			///
			Vertex(const glm::vec2& pos, const int depth, const graphics::Colour& colour = {0, 0, 0, 1}, const glm::vec2& texels = {0.0f, 0.0f}) noexcept;

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
			/// Set colour.
			///
			/// \param colour Colour of vertex, and opacity.
			///
			void set_colour(const graphics::Colour& colour) noexcept;

			///
			/// Set depth.
			///
			/// \param depth Z-Level. 0 - 1000.
			///
			void set_depth(const int depth) noexcept;

			///
			/// Get colour.
			///
			/// \return Const reference to a glm::vec4.
			///
			[[nodiscard]] const glm::vec4& get_colour() const noexcept;

			///
			/// Get depth.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_depth() const noexcept;

			///
			/// Spaceship operator.
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

		private:
			///
			/// RGBA colour.
			///
			glm::vec4 m_colour;

			///
			/// Depth (Z-Level).
			///
			int m_depth;
		};
	} // namespace graphics
} // namespace galaxy

#endif