///
/// RMLFile.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "RMLFile.hpp"

namespace galaxy
{
	namespace ui
	{
		Rml::FileHandle RMLFile::Open(const Rml::String& path)
		{
			auto path_opt = SL_HANDLE.vfs()->absolute(path);
			if (path_opt != std::nullopt)
			{
				const auto& abs_path = path_opt.value();
				return reinterpret_cast<Rml::FileHandle>(std::fopen(abs_path.c_str(), "rb"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load RML resource: {0}.", path);
				return NULL;
			}
		}

		void RMLFile::Close(Rml::FileHandle file)
		{
			std::fclose(reinterpret_cast<FILE*>(file));
		}

		size_t RMLFile::Read(void* buffer, size_t size, Rml::FileHandle file)
		{
			return std::fread(buffer, 1, size, reinterpret_cast<FILE*>(file));
		}

		bool RMLFile::Seek(Rml::FileHandle file, long offset, int origin)
		{
			return std::fseek(reinterpret_cast<FILE*>(file), offset, origin) == 0;
		}

		size_t RMLFile::Tell(Rml::FileHandle file)
		{
			return std::ftell(reinterpret_cast<FILE*>(file));
		}
	} // namespace ui
} // namespace galaxy