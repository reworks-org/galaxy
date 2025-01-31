///
/// Colour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_COLOUR_HPP_
#define GALAXY_GRAPHICS_COLOUR_HPP_

#include <raylib.hpp>

#include "galaxy/math/Vector3.hpp"
#include "galaxy/math/Vector4.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// RGBA colour.
		///
		class Colour final : public ::Color
		{
		public:
			///
			/// Constructor.
			///
			Colour() noexcept;

			///
			/// Argument constructor.
			///
			/// \param red Red channel.
			/// \param green Green channel.
			/// \param blue Blue channel.
			/// \param alpha Transparency. 0-255.
			///
			Colour(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255) noexcept;

			///
			/// Move constructor.
			///
			Colour(Colour&&) noexcept;

			///
			/// Move assignment operator.
			///
			[[nodiscard]]
			Colour& operator=(Colour&&) noexcept;

			///
			/// Copy constructor.
			///
			Colour(const Colour&) noexcept;

			///
			/// Copy assignment operator.
			///
			[[nodiscard]]
			Colour& operator=(const Colour&) noexcept;

			///
			/// Destructor.
			///
			~Colour() noexcept;

			///
			/// Get colour with alpha applied, alpha goes from 0.0f to 1.0f
			///
			/// \param alpha Alpha value.
			///
			/// \return The colour with alpha applied.
			///
			void fade(const float alpha) noexcept;

			///
			/// Get hexadecimal value for a Colour (0xRRGGBBAA).
			///
			/// \return Hexadecimal value.
			///
			[[nodiscard]]
			int to_hex() noexcept;

			///
			/// Get Colour structure from hexadecimal value.
			///
			/// \param hex_value Hexadecimal value.
			///
			void from_hex(const unsigned int hex_value) noexcept;

			///
			/// Get Colour normalized as float [0..1].
			///
			/// \return Normalized colour.
			///
			[[nodiscard]]
			math::Vector4 normalize() noexcept;

			///
			/// Get Colour from normalized values [0..1].
			///
			/// \param normalized Normalized colour values.
			///
			void from_normalized(const math::Vector4& normalized) noexcept;

			///
			/// Get HSV values for a Colour, hue [0..360], saturation/value [0..1].
			///
			/// \return math::Vector3 HSV values.
			///
			[[nodiscard]]
			math::Vector3 to_hsv() noexcept;

			///
			/// Get a Colour from HSV values, hue [0..360], saturation/value [0..1].
			///
			/// \param hue Hue value.
			/// \param saturation Saturation value.
			/// \param value Value.
			///
			void from_hsv(const float hue, const float saturation, const float value) noexcept;

			///
			/// Get colour multiplied with another colour.
			///
			/// \param tint The tint colour.
			///
			void set_tint(const Colour& tint) noexcept;

			///
			/// Get colour with brightness correction, brightness factor goes from -1.0f to 1.0f.
			///
			/// \param factor Brightness factor.
			///
			void set_brightness(const float factor) noexcept;

			///
			/// Get colour with contrast correction, contrast values between -1.0f and 1.0f.
			///
			/// \param contrast Contrast value.
			///
			void set_contrast(const float contrast) noexcept;

			///
			/// Get colour with alpha applied, alpha goes from 0.0f to 1.0f.
			///
			/// \param alpha Alpha value.
			///
			void set_alpha(const float alpha) noexcept;

			///
			/// Get src alpha-blended into dst colour with tint.
			///
			/// \param src Source colour.
			/// \param tint Tint colour.
			///
			void alpha_blend(const Colour& src, const Colour& tint) noexcept;

			///
			/// Get colour lerp interpolation between two colours, factor [0.0f..1.0f].
			///
			/// \param colour2 Second colour.
			/// \param factor Interpolation factor.
			///
			void lerp(const Colour& colour2, const float factor) noexcept;

		private:
			///
			/// Set from raylib colour.
			///
			/// \param colour Raylib colour
			///
			void set(const ::Color colour) noexcept;
		};

		///
		/// Equality operator for Colours.
		///
		/// \param lhs The left-hand side colour.
		/// \param rhs The right-hand side colour.
		///
		/// \return True if the colours are equal, false otherwise.
		///
		[[nodiscard]]
		bool operator==(const Colour& lhs, const Colour& rhs) noexcept;

		///
		/// Inequality operator for Colours.
		///
		/// \param lhs The left-hand side colour.
		/// \param rhs The right-hand side colour.
		///
		/// \return True if the colours are not equal, false otherwise.
		///
		[[nodiscard]]
		bool operator!=(const Colour& lhs, const Colour& rhs) noexcept;
	} // namespace graphics
} // namespace galaxy

#endif
