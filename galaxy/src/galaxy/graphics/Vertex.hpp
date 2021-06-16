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
		public:
			///
			/// Constructor.
			///
			Vertex() noexcept;

			///
			/// Argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param texels Vertex texture coordinates.
			/// \param colour Colour of vertex, and opacity.
			///
			Vertex(const glm::vec2& pos, const glm::vec2& texels = {0.0f, 0.0f}, const graphics::Colour& colour = {0.0f, 0.0f, 0.0f, 1.0f}) noexcept;

			///
			/// Reverse argument constructor.
			///
			/// \param pos Position of vertex.
			/// \param colour Colour of vertex, and opacity.
			/// \param texels Vertex texture coordinates.
			///
			Vertex(const glm::vec2& pos, const graphics::Colour& colour = {0.0f, 0.0f, 0.0f, 1.0f}, const glm::vec2& texels = {0.0f, 0.0f}) noexcept;

			///
			/// Move constructor.
			///
			Vertex(Vertex&&) noexcept;

			///
			/// Move assignment operator.
			///
			Vertex& operator=(Vertex&&) noexcept;

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
			/// Set opacity.
			///
			/// \param opacity Set alpha value of colour. Clamped between 0.0f and 1.0f.
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get colour.
			///
			/// \return Const reference to a glm::vec4.
			///
			[[nodiscard]] const glm::vec4& get_colour() const noexcept;

			///
			/// Spaceship operator.
			///
			[[nodiscard]] auto operator<=>(const Vertex&) const = default;

		private:
			///
			/// Copy constructor.
			///
			Vertex(const Vertex&) = delete;

			///
			/// Copy assignment operator.
			///
			Vertex& operator=(const Vertex&) = delete;

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
			/// Colour of Vertex.
			///
			glm::vec4 m_colour;
		};
	} // namespace graphics
} // namespace galaxy

#endif