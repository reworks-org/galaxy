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
		FreeTypeLib::FreeTypeLib()
			: m_library {nullptr}
		{
			if (FT_Init_FreeType(&m_library) != FT_OK)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to init FreeType2.");
			}
		}

		FreeTypeLib::~FreeTypeLib() noexcept
		{
			if (m_library != nullptr)
			{
				FT_Done_FreeType(m_library);
				m_library = nullptr;
			}
		}

		FT_Library& FreeTypeLib::lib() noexcept
		{
			return m_library;
		}
	} // namespace graphics
} // namespace galaxy