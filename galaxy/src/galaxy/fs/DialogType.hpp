///
/// DialogType.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_FS_DIALOGTYPE_HPP_
#define GALAXY_FS_DIALOGTYPE_HPP_

namespace galaxy
{
	namespace fs
	{
		///
		/// Type of dialog box for use with tinyfd.
		///
		enum class DialogType : int
		{
			ok,
			okcancel,
			yesno,
			yesnocancel
		};
	} // namespace fs
} // namespace galaxy

#endif
