///
/// Colour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_COLOUR_HPP_
#define GALAXY_GRAPHICS_COLOUR_HPP_

#include <compare>

#include <glm/vec4.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents an RGBA colour.
		///
		class Colour final
		{
		public:
			///
			/// Default constructor.
			///
			Colour() noexcept;

			///
			/// 8bit constructor.
			///
			/// \param r Red.
			/// \param g Green.
			/// \param b Blue.
			/// \param a Alpha level. Defaults to Opaque (255).
			///
			Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = 255) noexcept;

			///
			/// Default destructor.
			///
			~Colour() noexcept = default;

			///
			/// \brief Normalizes values and returns as floats.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Vec4.
			///
			[[nodiscard]] glm::vec4 normalized() noexcept;

			///
			/// \brief Normalizes r colour.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Normalized r colour as float.
			///
			[[nodiscard]] const float r_normal() noexcept;

			///
			/// \brief Normalizes g colour.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Normalized g colour as float.
			///
			[[nodiscard]] const float g_normal() noexcept;

			///
			/// \brief Normalizes b colour.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Normalized b colour as float.
			///
			[[nodiscard]] const float b_normal() noexcept;

			///
			/// \brief Normalizes alpha.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Normalizedalpha as float.
			///
			[[nodiscard]] const float a_normal() noexcept;

			///
			/// Spaceship operator.
			///
			[[nodiscard]] auto operator<=>(const Colour&) const = default;

		public:
			///
			/// Red
			///
			std::uint8_t m_red;

			///
			/// Green
			///
			std::uint8_t m_green;

			///
			/// Blue
			///
			std::uint8_t m_blue;

			///
			/// Alpha
			///
			std::uint8_t m_alpha;
		};
	} // namespace graphics
} // namespace galaxy

#endif