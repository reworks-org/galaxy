///
/// AudioFileFactory.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cstdio>

#include "galaxy/audio/AudioFileReader.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "AudioFileFactory.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26403)
GALAXY_DISABLE_WARNING(26409)
GALAXY_DISABLE_WARNING(26487)
#endif

namespace galaxy
{
	namespace audio
	{
		irrklang::IFileReader* AudioFileFactory::createFileReader(const irrklang::ik_c8* filename)
		{
			auto& fs        = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			const auto info = fs.find(filename);

			if (info.m_code == fs::FileCode::FOUND)
			{
				auto* file = std::fopen(filename, "rb");
				if (!file)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open '{0}' with std::fopen.", filename);
				}
				else
				{
					return new AudioFileReader(file, filename);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' in vfs, returned error '{1}'.", filename, magic_enum::enum_name(info.m_code));
			}

			return nullptr;
		}
	} // namespace audio
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif