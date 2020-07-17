///
/// Colour.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_COLOUR_HPP_
#define PROTOSTAR_COLOUR_HPP_

#include <array>
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
		Colour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) noexcept;

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
		/// Converts to fixed size array of type Type.
		///
		/// \return Moveable fixed size std::array of Type.
		///
		template<IsArithmetic Type>
		[[nodiscard]] std::array<Type, 4>&& getAs();

		///
		/// \brief Normalizes values and returns as floats.
		///
		/// Floats are in range 0.0f - 1.0f.
		///
		/// \return Move invoked fixed size std::array of floats.
		///
		[[nodiscard]] std::array<float, 4>&& getNormalized();

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

	template<IsArithmetic Type>
	inline std::array<Type, 4>&& Colour::getAs()
	{
		std::array<Type, 4> arr =
		{ 
			static_cast<Type>(m_red),
			static_cast<Type>(m_green),
			static_cast<Type>(m_blue),
			static_cast<Type>(m_alpha)
		};

		return std::move(arr);
	}
}

#endif