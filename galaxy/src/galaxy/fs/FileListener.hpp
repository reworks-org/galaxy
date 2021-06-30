///
/// FileListener.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_FILELISTENER_HPP_
#define GALAXY_FS_FILELISTENER_HPP_

#include <functional>

#include <efsw/efsw.hpp>

namespace galaxy
{
	namespace fs
	{
		///
		/// Implementation of FileWatchListener from efsw.
		///
		class FileListener : public efsw::FileWatchListener
		{
		public:
			///
			/// Callback Type.
			///
			using Callback = std::function<void(efsw::WatchID, const std::string&, const std::string&, efsw::Action, std::string)>;

			///
			/// Constructor.
			///
			FileListener() noexcept = default;

			///
			/// Virtual destructor.
			///
			virtual ~FileListener() noexcept = default;

			///
			/// Set action callback.
			///
			/// \param function Function callback to set.
			///
			void set_action(Callback&& function) noexcept;

			///
			/// Handles the action file action
			/// \param watchid The watch id for the directory.
			/// \param dir The directory.
			/// \param filename The filename that was accessed (not full path).
			/// \param action Action that was performed.
			/// \param oldFilename The name of the file or directory moved.
			///
			void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "") override;

		private:
			///
			/// Callback to use on a file action.
			///
			Callback m_callback;
		};
	} // namespace fs
} // namespace galaxy

#endif