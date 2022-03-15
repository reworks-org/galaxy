///
/// FreeType.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_FREETYPE_HPP_
#define GALAXY_GRAPHICS_FREETYPE_HPP_

#include <ft2build.h>
#include FT_FREETYPE_H

///
/// Shortcut Macro.
///
#define FT_HANDLE galaxy::graphics::FreeTypeLib::handle()

///
/// OK return value macro.
///
#define FT_OK 0

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Wraps around freetype library initialization.
		///
		/// Automatically added to the service locator for you.
		///
		class FreeTypeLib final
		{
		public:
			///
			/// Constructor.
			///
			FreeTypeLib();

			///
			/// Destructor.
			///
			~FreeTypeLib() noexcept;

			///
			/// Handle to FT library.
			///
			/// \return Reference to FT_Library.
			///
			[[nodiscard]] FT_Library& lib() noexcept;

		private:
			///
			/// Copy constructor.
			///
			FreeTypeLib(const FreeTypeLib&) = delete;

			///
			/// Move constructor.
			///
			FreeTypeLib(FreeTypeLib&&) = delete;

			///
			/// Copy assignment operator.
			///
			FreeTypeLib& operator=(const FreeTypeLib&) = delete;

			///
			/// Move assignment operator.
			///
			FreeTypeLib& operator=(FreeTypeLib&&) = delete;

		private:
			///
			/// FreeType library structure.
			///
			FT_Library m_library;
		};
	} // namespace graphics
} // namespace galaxy

#endif