///
/// FreeType.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "FreeType.hpp"

namespace galaxy
{
	namespace graphics
	{
		FreeTypeLib& FreeTypeLib::handle() noexcept
		{
			static FreeTypeLib lib;
			return lib;
		}

		void FreeTypeLib::open()
		{
			if (FT_Init_FreeType(&m_freetype_lib) != 0)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to init FreeType.");
			}
		}

		void FreeTypeLib::close() noexcept
		{
			FT_Done_FreeType(m_freetype_lib);
		}

		FT_Library& FreeTypeLib::lib() noexcept
		{
			return m_freetype_lib;
		}
	} // namespace graphics
} // namespace galaxy