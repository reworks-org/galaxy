///
/// FreeType.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "FreeType.hpp"

///
/// Core namespace.
///
namespace qs
{
	FreeTypeLib& FreeTypeLib::handle()
	{
		static FreeTypeLib lib;
		return lib;
	}

	void FreeTypeLib::open()
	{
		if (FT_Init_FreeType(&m_freetype_lib) != 0)
		{
			PL_LOG(PL_FATAL, "Failed to init FreeType.");
		}
	}

	void FreeTypeLib::close()
	{
		FT_Done_FreeType(m_freetype_lib);
	}

	FT_Library& FreeTypeLib::lib()
	{
		return m_freetype_lib;
	}
} // namespace qs