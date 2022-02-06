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
		FreeTypeLib::FreeTypeLib() noexcept
			: m_freetype_lib {nullptr}
		{
		}

		FreeTypeLib::~FreeTypeLib() noexcept
		{
			if (m_freetype_lib != nullptr)
			{
				close();
			}
		}

		FreeTypeLib& FreeTypeLib::handle() noexcept
		{
			static FreeTypeLib lib;
			return lib;
		}

		void FreeTypeLib::open()
		{
			if (FT_Init_FreeType(&m_freetype_lib) != FT_OK)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to init FreeType2.");
			}
		}

		void FreeTypeLib::close() noexcept
		{
			FT_Done_FreeType(m_freetype_lib);
			m_freetype_lib = nullptr;
		}

		FT_Library& FreeTypeLib::lib() noexcept
		{
			return m_freetype_lib;
		}
	} // namespace graphics
} // namespace galaxy