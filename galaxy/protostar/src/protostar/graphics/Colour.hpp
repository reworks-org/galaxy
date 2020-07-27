///
/// Colour.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_COLOUR_HPP_
#define PROTOSTAR_COLOUR_HPP_

#include <array>
#include <compare>
#include <cstdint>

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
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
		/// RGB constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		///
		Colour(const pr::positive_uint8_t auto r, const pr::positive_uint8_t auto g, const pr::positive_uint8_t auto b) noexcept;

		///
		/// Argument constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		/// \param a Alpha.
		///
		Colour(const pr::positive_uint8_t auto r, const pr::positive_uint8_t auto g, const pr::positive_uint8_t auto b, const pr::positive_uint8_t auto a) noexcept;

		///
		/// Default destructor.
		///
		~Colour() noexcept = default;

		///
		/// Converts to fixed size array of type Type.
		///
		/// \return Moveable fixed size std::array of Type.
		///
		template<is_arithmetic type>
		[[nodiscard]] std::array<type, 4> get_as();

		///
		/// \brief Normalizes values and returns as floats.
		///
		/// Floats are in range 0.0f - 1.0f.
		///
		/// \return Move invoked fixed size std::array of floats.
		///
		[[nodiscard]] std::array<float, 4> get_normalized() noexcept;

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

	template<is_arithmetic type>
	inline std::array<type, 4> Colour::get_as()
	{
		std::array<type, 4> arr =
		    {static_cast<type>(m_red), static_cast<type>(m_green), static_cast<type>(m_blue), static_cast<type>(m_alpha)};

		return arr;
	}
} // namespace pr

#endif