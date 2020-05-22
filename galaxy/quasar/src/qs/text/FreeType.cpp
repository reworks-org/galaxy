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
	FreeTypeLib::~FreeTypeLib() noexcept
	{
		close();
	}

	FreeTypeLib& FreeTypeLib::handle() noexcept
	{
		static FreeTypeLib lib;
		return lib;
	}

	void FreeTypeLib::close() noexcept
	{
		FT_Done_FreeType(m_ftLibrary);
	}

	FT_Library& FreeTypeLib::lib() noexcept
	{
		return m_ftLibrary;
	}

	FreeTypeLib::FreeTypeLib() noexcept
	{
		if (FT_Init_FreeType(&m_ftLibrary))
		{
			PL_LOG(PL_FATAL, "Failed to init FreeType.");
		}
	}
}