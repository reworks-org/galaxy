///
/// Colour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_COLOUR_HPP_
#define GALAXY_GRAPHICS_COLOUR_HPP_

#include <array>

#include <glm/vec4.hpp>

#undef OPAQUE
#undef TRANSPARENT

namespace galaxy
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
		/// Colour offsets for normalization.
		///
		inline static const constexpr std::uint8_t OFFSET = 0xFF;

		///
		/// Colour lower bounds.
		///
		inline static const constexpr std::uint8_t LOWER = 0;

		///
		/// Colour upper bounds.
		///
		inline static const constexpr std::uint8_t UPPER = 255;

	public:
		///
		/// Constructor.
		///
		Colour() noexcept;

		///
		/// Colour constructor.
		///
		/// \param r Red. 0 - 255.
		/// \param g Green. 0 - 255.
		/// \param b Blue. 0 - 255.
		/// \param a Alpha. 0 - 255.
		///
		Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = OPAQUE) noexcept;

		///
		/// Move constructor.
		///
		Colour(Colour&&) noexcept;

		///
		/// Copy constructor.
		///
		Colour(const Colour&) noexcept;

		///
		/// Move assignment operator.
		///
		Colour& operator=(Colour&&) noexcept;

		///
		/// Copy assignment operator.
		///
		Colour& operator=(const Colour& w) noexcept;

		///
		/// Destructor.
		///
		~Colour() noexcept;

		///
		/// Red.
		///
		/// \param r 0 - 255.
		///
		void r(const std::uint8_t r) noexcept;

		///
		/// Green.
		///
		/// \param g 0 - 255.
		///
		void g(const std::uint8_t g) noexcept;

		///
		/// Blue.
		///
		/// \param b 0 - 255.
		///
		void b(const std::uint8_t b) noexcept;

		///
		/// Alpha.
		///
		/// \param a 0 - 255.
		///
		void a(const std::uint8_t a) noexcept;

		///
		/// Get red.
		///
		/// \return Int in 0 - 255 range.
		///
		std::uint8_t r() const noexcept;

		///
		/// Get green.
		///
		/// \return Int in 0 - 255 range.
		///
		std::uint8_t g() const noexcept;

		///
		/// Get blue.
		///
		/// \return Int in 0 - 255 range.
		///
		std::uint8_t b() const noexcept;

		///
		/// Get alpha.
		///
		/// \return Int in 0 - 255 range.
		///
		std::uint8_t a() const noexcept;

		///
		/// Set RGBA from normalizaed values.
		///
		/// \param rgba Clamps from 0.0f to 1.0f.
		///
		void set_from_norm(const glm::vec4& rgba) noexcept;

		///
		/// Get normalized rgba vec4.
		///
		/// \return Vec4 normalized to 0.0f - 1.0f.
		///
		[[nodiscard]]
		glm::vec4 normalize() noexcept;

		///
		/// Get as array.
		///
		/// \return 0=red, 1=green, 2=blue, 3=alpha
		///
		[[nodiscard]]
		std::array<std::uint8_t, 4> array() noexcept;

		///
		/// Three-way comparison.
		///
		auto operator<=>(const Colour&) const = default;

		///
		/// Equality comparison.
		///
		bool operator==(const Colour&) const = default;

		///
		/// Inequality comparison.
		///
		bool operator!=(const Colour&) const = default;

		///
		/// Less than comparison.
		///
		bool operator<(const Colour&) const = default;

		///
		/// Greater than comparison.
		///
		bool operator>(const Colour&) const = default;

		///
		/// Less than or equal comparison
		///
		bool operator<=(const Colour&) const = default;

		///
		/// Greater than or equal comparison.
		///
		bool operator>=(const Colour&) const = default;

	private:
		///
		/// Red channel.
		///
		std::uint8_t m_red;

		///
		/// Green channel.
		///
		std::uint8_t m_green;

		///
		/// Blue channel.
		///
		std::uint8_t m_blue;

		///
		/// Alpha channel.
		///
		std::uint8_t m_alpha;
	};
} // namespace galaxy

#endif
