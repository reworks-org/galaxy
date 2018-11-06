///
/// Quasar.hpp
/// Quasar
///
/// Created by reworks on 16/09/2018.
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ERROR_HPP_
#define QUASAR_ERROR_HPP_

#include <string>

namespace qs
{
	class Error
	{
	public:
		///
		/// Resets current error state.
		///
		void clear();

		///
		/// Gets the current error message.
		///
		std::string get();

		///
		/// Prints all error messages.
		///


	private:
		///
		///
		///
		Error() = default;
	};

	///
	/// Static declaration.
	///
}

#endif