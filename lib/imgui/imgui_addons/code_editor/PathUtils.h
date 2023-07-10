#pragma once

#include <string>

namespace PathUtils
{
	void SetProgramDirectory(const std::string& executablePath);
	const std::string& GetProgramDirectory();
	const std::string& GetAssetsDirectory();

	std::string GetRelativePath(const std::string& fileRelativeTo, const std::string& targetPath);
	std::string GetFolderPath(const std::string& filePath);

	inline const char* GetFileNameFromPath(const char* path)
	{
		int i = 0;
		for (; path[i] != '\0'; i++);
		for (; path[i - 1] != '\\' && path[i - 1] != '/'; i--);
		return &(path[i]);
	}
	inline bool FileHasExtension(const char* path, const char* ext)
	{
		int i = 0;
		for (; path[i] != '\0'; i++);
		for (;; i--)
		{
			if (i == -1 || path[i] == '\\' || path[i] == '/')
				return false;
			if (path[i] == '.')
			{
				i++;
				for (int j = 0; ext[j] != '\0'; j++)
				{
					if (ext[j] != path[i + j])
						return false;
				}
				return true;
			}
		}
	}
}