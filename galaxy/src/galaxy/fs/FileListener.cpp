///
/// FileListener.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "FileListener.hpp"

namespace galaxy
{
	namespace fs
	{
		void FileListener::set_action(Callback&& function) noexcept
		{
			m_callback = std::move(function);
		}

		void
		FileListener::handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename)
		{
			m_callback(watchid, dir, filename, action, oldFilename);
		}
	} // namespace fs
} // namespace galaxy