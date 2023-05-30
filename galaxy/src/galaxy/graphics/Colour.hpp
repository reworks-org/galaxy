///
/// Colour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_COLOUR_HPP_
#define GALAXY_GRAPHICS_COLOUR_HPP_

#include <array>
#include <compare>

#include <glm/vec4.hpp>

#undef OPAQUE
#undef TRANSPARENT

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
			Colour();

			///
			/// Argument Constructor.
			///
			/// \param r Red.
			/// \param g Green.
			/// \param b Blue.
			/// \param a Alpha level. Defaults to Opaque (255).
			///
			Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = OPAQUE);

			///
			/// Destructor.
			///
			~Colour();

			///
			/// Set using normalized values.
			///
			/// \param r Red.
			/// \param g Green.
			/// \param b Blue.
			/// \param a Alpha level. Defaults to Opaque (255).
			///
			void set_from_normalized(const float r, const float g, const float b, const float a);

			///
			/// \brief Normalizes values and returns as floats.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Array of floats.
			///
			[[nodiscard]] std::array<float, 4> to_array();

			// \brief Normalizes values and returns as floats.
			///
			/// Floats are in range 0.0f - 1.0f.
			///
			/// \return Vec4.
			///
			[[nodiscard]] glm::vec4 to_vec4();

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