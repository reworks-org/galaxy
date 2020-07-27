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
	FreeTypeLib& FreeTypeLib::handle() noexcept
	{
		static FreeTypeLib lib;
		return lib;
	}

	void FreeTypeLib::open()
	{
		if (FT_Init_FreeType(&m_freetype_lib))
		{
			PL_LOG(PL_FATAL, "Failed to init FreeType.");
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
} // namespace qs