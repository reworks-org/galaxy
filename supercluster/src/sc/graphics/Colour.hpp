///
/// Colour.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_COLOUR_HPP_
#define SUPERCLUSTER_COLOUR_HPP_

#include <cstdint>

///
/// Core namespace.
///
namespace sc
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
		/// Destructor.
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

		///
		/// Fully transparent.
		///
		static inline const std::uint8_t TRANSPARENT = 0;

		///
		/// Fully opaque.
		///
		static inline const std::uint8_t OPAQUE = 255;
	};
}

#endif