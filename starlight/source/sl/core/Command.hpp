///
/// Command.hpp
/// starlight
///
/// Created by reworks on 13/01/2019.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_COMMAND_HPP_
#define	STARLIGHT_COMMAND_HPP_

namespace sl
{
	///
	/// A command is basically a request in an object oriented form.
	/// Similar to a callback, but with more potential.
	/// You can use it for example, to give multiple "actors" the ability to do the same thing.
	/// You can use it for other useful things, like returning it from a function when something happens, then processing whatever command there is.
	///
	class Command
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual inline ~Command() noexcept {};

		///
		/// Execute the command.
		///
		/// \return bool Return true if successful, false if not.
		///
		virtual bool exec() = 0;

		///
		/// Reverse the command's actions. Undo them.
		///
		/// \return bool Return true if successful, false if not.
		///
		virtual bool undo() = 0;
	};
}

#endif