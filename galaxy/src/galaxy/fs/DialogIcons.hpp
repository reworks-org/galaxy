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
	///
	/// \brief Type of native system icon to display on file dialogs.
	///
	/// Enums are in lowercase for tinyfiledialogs.
	///
	enum class DialogIcon : int
	{
		info,
		warning,
		error,
		question
	};
} // namespace galaxy

#endif
