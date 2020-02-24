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
	/// Identifies this class as an interface.
	///
	namespace interface
	{
		///
		/// Your Fonts should inherit from this class.
		/// Represents a font.
		///
		class Font
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Font() noexcept = default;

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
			Font() noexcept = default;

			///
			/// Copy constructor.
			///
			Font(const Font&) noexcept = default;

			///
			/// Move constructor.
			///
			Font(Font&&) noexcept = default;

			///
			/// Copy assignment operator.
			///
			Font& operator= (const Font&&) noexcept = default;

			///
			/// Move assignment operator.
			///
			Font& operator= (Font&&) noexcept = default;
		};

		///
		/// Shorthand for unique_ptr Font.
		///
		using FontPtr = std::unique_ptr<celestial::interface::Font>;
	}
}

#endif