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

#include "AudioFileFactory.hpp"

namespace galaxy
{
	namespace audio
	{
		irrklang::IFileReader* AudioFileFactory::createFileReader(const irrklang::ik_c8* filename)
		{
			auto& fs  = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto info = fs.find(filename);

			if (info.m_code == fs::FileInfo::Code::FOUND)
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
				GALAXY_LOG(GALAXY_ERROR,
					"Failed to find '{0}' in vfs, returned error '{1}'.",
					filename,
					magic_enum::enum_name<fs::FileInfo::Code>(info.m_code));
			}

			return nullptr;
		}
	} // namespace audio
} // namespace galaxy