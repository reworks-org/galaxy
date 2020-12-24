///
/// Colour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COLOUR_HPP_
#define GALAXY_COLOUR_HPP_

#include <array>
#include <compare>
#include <cstdint>

#include "galaxy/meta/Concepts.hpp"

///
/// Core namespace.
///
namespace galaxy
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
		Colour();

		///
		/// RGB constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		///
		Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b);

		///
		/// Argument constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		/// \param a Alpha.
		///
		Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a);

		///
		/// Copy constructor.
		///
		Colour(const Colour&) = default;

		///
		/// Move constructor.
		///
		Colour(Colour&&) = default;

		///
		/// Copy assignment operator.
		///
		Colour& operator=(const Colour&) = default;

		///
		/// Move assignment operator.
		///
		Colour& operator=(Colour&&) = default;

		///
		/// Default destructor.
		///
		~Colour() = default;

		///
		/// Converts to fixed size array of type Type.
		///
		/// \return Moveable fixed size std::array of Type.
		///
		template<is_arithmetic Type>
		[[nodiscard]] std::array<Type, 4> get_as();

		///
		/// \brief Normalizes values and returns as floats.
		///
		/// Floats are in range 0.0f - 1.0f.
		///
		/// \return Move invoked fixed size std::array of floats.
		///
		[[nodiscard]] std::array<float, 4> get_normalized();

		///
		/// Spaceship operator.
		///
		auto operator<=>(const Colour&) const = default;

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

	template<is_arithmetic Type>
	inline std::array<Type, 4> Colour::get_as()
	{
		std::array<Type, 4> arr =
		    {static_cast<Type>(m_red), static_cast<Type>(m_green), static_cast<Type>(m_blue), static_cast<Type>(m_alpha)};

		return arr;
	}
} // namespace galaxy

#endif