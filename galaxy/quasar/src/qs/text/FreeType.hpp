///
/// FreeType.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FREETYPE_HPP_
#define QUASAR_FREETYPE_HPP_

#include <ft2build.h>
#include FT_FREETYPE_H

///
/// Core namespace.
///
namespace qs
{
	///
	/// Wraps around freetype library initialization.
	///
	class FreeTypeLib final
	{
	public:
		///
		/// Destructor.
		///
		~FreeTypeLib() noexcept;

		///
		/// Handle to FT library.
		///
		/// \return Reference to FT_Library.
		///
		static FT_Library& lib() noexcept;

	private:
		///
		/// Constructor.
		///
		FreeTypeLib() noexcept;

	private:
		///
		/// FreeType library structure.
		///
		FT_Library m_ftLibrary;
	};
}

#endif