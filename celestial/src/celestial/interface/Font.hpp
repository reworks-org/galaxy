///
/// Font.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_FONT_HPP_
#define CELESTIAL_FONT_HPP_

#include "celestial/Config.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Your Fonts should inherit from this class.
	/// Represents a font.
	///
	class Font
	{
	public:
		///
		/// Virtual Destructor.
		///
		virtual ~Font() noexcept;

		///
		/// Retrieves size integer.
		///
		/// \return size integer.
		///
		virtual const int getSize() const noexcept = 0;

	protected:
		///
		/// Default constructor.
		///
		Font() = default;

		///
		/// Copy constructor.
		///
		Font(const Font&) = default;

		///
		/// Move constructor.
		///
		Font(Font&&) = default;
	};

	///
	/// Shorthand for unique_ptr Font.
	///
	using FontPtr = std::unique_ptr<Font>;
}

#endif