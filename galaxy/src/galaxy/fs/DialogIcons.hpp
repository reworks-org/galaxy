///
/// DialogIcons.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_DIALOGICONS_HPP_
#define GALAXY_FS_DIALOGICONS_HPP_

namespace galaxy
{
	namespace fs
	{
		///
		/// \brief Type of native system icon to display on file dialogs.
		///
		/// Enums are in lowercase for tinyfiledialogs.
		///
		enum class DialogIcon
		{
			info,
			warning,
			error
		};
	} // namespace fs
} // namespace galaxy

#endif