///
/// DialogButton.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_DIALOGBUTTON_HPP_
#define GALAXY_FS_DIALOGBUTTON_HPP_

namespace galaxy
{
	///
	/// Type of button for tinyfd boxes.
	///
	enum class DialogButton : int
	{
		cancel_no     = 0,
		ok_yes        = 1,
		yes_no_cancel = 2
	};
} // namespace galaxy

#endif
