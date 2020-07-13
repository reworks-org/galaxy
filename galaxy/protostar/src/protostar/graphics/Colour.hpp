///
/// Colour.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_COLOUR_HPP_
#define PROTOSTAR_COLOUR_HPP_

#include <array>

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
		/// Argument constructor.
		///
		/// \param r Red.
		/// \param g Green.
		/// \param b Blue.
		/// \param a Alpha.
		///
		Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept;

		///
		/// Default destructor.
		///
		~Colour() noexcept = default;

		///
		/// Converts to fixed size float array.
		///
		/// \return Move invoked fixed size std::array of floats.
		///
		std::array<float, 4> asFloats() noexcept;

		///
		/// Converts to fixed size int array.
		///
		/// \return Move invoked fixed size std::array of ints.
		///
		std::array<int, 4> asIntegers() noexcept;

		///
		/// \brief Converts to fixed size float array compatible with OpenGL
		///
		/// Floats are in range 0.0f - 1.0f.
		///
		/// \return Move invoked fixed size std::array of floats.
		///
		std::array<float, 4> asGLColour() noexcept;

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