///
/// Colour.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_COLOUR_HPP_
#define PROTOSTAR_COLOUR_HPP_

#include <cstdint>

///
/// Core namespace.
///
namespace protostar
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
		/// Argument constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		/// \param a Alpha.
		///
		Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a);

		///
		/// Default destructor.
		///
		~Colour() noexcept = default;

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
}

#endif