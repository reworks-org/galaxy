///
/// FileError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/logging/Log.hpp"

#include "FileError.hpp"

namespace galaxy
{
	FileError::FileError() noexcept
	{
	}

	FileError::FileError(const std::string& mode, const std::string& reason, const std::filesystem::path& path) noexcept
		: m_mode {mode}
		, m_reason {reason}
		, m_path {path}
	{
	}

	FileError::~FileError() noexcept
	{
	}

	void FileError::log() const noexcept
	{
		GALAXY_LOG(GALAXY_ERROR, "Failed to {0} {2} because {1}", m_mode, m_reason, m_path);
	}
} // namespace galaxy
