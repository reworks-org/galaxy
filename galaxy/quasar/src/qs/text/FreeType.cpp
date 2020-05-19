///
/// FreeType.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "qs/utils/Error.hpp"

#include "FreeType.hpp"

///
/// Core namespace.
///
namespace qs
{
	FreeTypeLib::FreeTypeLib() noexcept
	{
		if (FT_Init_FreeType(&m_ftLibrary))
		{
			qs::Error::handle().callback("FreeType.cpp", 22, "Failed to init FreeType.");
		}
	}

	FreeTypeLib::~FreeTypeLib() noexcept
	{
		FT_Done_FreeType(m_ftLibrary);
	}

	FT_Library& FreeTypeLib::lib() noexcept
	{
		static FreeTypeLib lib;
		return lib.m_ftLibrary;
	}
}