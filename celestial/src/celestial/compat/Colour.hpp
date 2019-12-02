///
/// Colour.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef COLOUR_CELESTIAL_HPP_
#define COLOUR_CELESTIAL_HPP_

#include <memory>
#include <cstdint>

namespace celestial
{
	///
	/// Basic Colour interface.
	///
	class Colour
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Colour() noexcept = default;

		///
		/// Get Red.
		///
		virtual const std::uint8_t getR() const noexcept = 0;

		///
		/// Get Green.
		///
		virtual const std::uint8_t getG() const noexcept = 0;

		///
		/// Get Blue.
		///
		virtual const std::uint8_t getB() const noexcept = 0;

		///
		/// Get Alpha.
		///
		virtual const std::uint8_t getA() const noexcept = 0;

	protected:
		///
		/// Default constructor.
		///
		Colour() = default;

		///
		/// Copy constructor.
		///
		Colour(const Colour&) = default;

		///
		/// Move constructor.
		///
		Colour(Colour&&) = default;
	};

	///
	/// Shortcut to get around defining an absolute type.
	///
	using ColourPtr = std::unique_ptr<celestial::Colour>;
}

#endif