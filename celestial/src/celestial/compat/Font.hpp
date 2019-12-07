///
/// Font.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_FONT_HPP_
#define CELESTIAL_FONT_HPP_

#include "celestial/Config.hpp"

namespace celestial
{
	namespace compat
	{
		///
		/// Your Fonts should inherit from this class.
		/// Represents a font.
		///
		template<typename CRTP>
		class Font
		{
		public:
			///
			/// Virtual Destructor.
			///
			virtual ~Font();

			///
			/// Casts the class to its child type.
			///
			virtual CRTP* getChild() final;

			///
			/// Retrieves size integer.
			///
			/// \return size integer.
			///
			virtual int getSize() const noexcept = 0;

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

		template<typename CRTP>
		inline Font<CRTP>::~Font()
		{
		}

		template<typename CRTP>
		inline CRTP* Font<CRTP>::getChild()
		{
			return static_cast<CRTP>(this);
		}
	}

	///
	/// Shortcut to get around defining an absolute type.
	///
	using Font = celestial::compat::Font<FONT_CLASS>;
	using FontPtr = std::unique_ptr<Font>;
}

#endif