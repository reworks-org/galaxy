///
/// AudioFileReader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cstdio>

#include "galaxy/platform/Pragma.hpp"

#include "AudioFileReader.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26440)
#endif

namespace galaxy
{
	namespace audio
	{
		AudioFileReader::AudioFileReader(FILE* opened_file, const irrklang::ik_c8* filename) noexcept
			: m_file {opened_file}
			, m_filename {filename}
			, m_filesize {0}
		{
			std::fseek(m_file, 0, SEEK_END);
			m_filesize = std::ftell(m_file);
			std::fseek(m_file, 0, SEEK_SET);
		}

		AudioFileReader::~AudioFileReader() noexcept
		{
			std::fclose(m_file);
		}

		irrklang::ik_s32 AudioFileReader::read(void* buffer, irrklang::ik_u32 size_to_read)
		{
			return static_cast<irrklang::ik_s32>(std::fread(buffer, 1, size_to_read, m_file));
		}

		bool AudioFileReader::seek(irrklang::ik_s32 final_pos, bool relative_movement)
		{
			return std::fseek(m_file, final_pos, relative_movement ? SEEK_CUR : SEEK_SET) == 0;
		}

		irrklang::ik_s32 AudioFileReader::getSize()
		{
			return m_filesize;
		}

		irrklang::ik_s32 AudioFileReader::getPos()
		{
			return std::ftell(m_file);
		}

		const irrklang::ik_c8* AudioFileReader::getFileName()
		{
			return m_filename.c_str();
		}
	} // namespace audio
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif