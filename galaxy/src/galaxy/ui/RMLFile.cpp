///
/// RMLFile.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "RMLFile.hpp"

namespace galaxy
{
	namespace ui
	{
		Rml::FileHandle RMLFile::Open(const Rml::String& path)
		{
			auto& fs  = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto info = fs.find(path);
			if (info.m_code == fs::FileCode::FOUND)
			{
				return reinterpret_cast<Rml::FileHandle>(std::fopen(info.m_string.c_str(), "rb"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load RML resource: {0}, because {1}.", path, magic_enum::enum_name(info.m_code));
				return NULL;
			}
		}

		void RMLFile::Close(Rml::FileHandle file)
		{
			std::fclose(reinterpret_cast<FILE*>(file));
		}

		std::size_t RMLFile::Read(void* buffer, std::size_t size, Rml::FileHandle file)
		{
			return std::fread(buffer, 1, size, reinterpret_cast<FILE*>(file));
		}

		bool RMLFile::Seek(Rml::FileHandle file, long offset, int origin)
		{
			return std::fseek(reinterpret_cast<FILE*>(file), offset, origin) == 0;
		}

		std::size_t RMLFile::Tell(Rml::FileHandle file)
		{
			return std::ftell(reinterpret_cast<FILE*>(file));
		}
	} // namespace ui
} // namespace galaxy