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
			/// Opaque colour.
			///
			inline static const constexpr std::uint8_t OPAQUE = 255;

			///
			/// Transparent colour.
			///
			inline static const constexpr std::uint8_t TRANSPARENT = 0;

			///
			/// \brief Constructor.
			///
			/// Defaults to White.
			///
			Colour() noexcept;

			///
			/// Argument Constructor.
			///
			/// \param r Red.
			/// \param g Green.
			/// \param b Blue.
			/// \param a Alpha level. Defaults to Opaque (255).
			///
			Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = OPAQUE) noexcept;

			///
			/// Destructor.
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
			/// Comparison operator.
			///
			[[nodiscard]] auto operator<=>(const Colour&) const = default;

		public:
			///
			/// Red.
			///
			std::uint8_t m_red;

			///
			/// Green.
			///
			std::uint8_t m_green;

			///
			/// Blue.
			///
			std::uint8_t m_blue;

			///
			/// Alpha.
			///
			std::uint8_t m_alpha;
		};
	} // namespace graphics
} // namespace galaxy

#endif