///
/// Error.hpp
/// starlight
///
/// Created by reworks on 27/11/2018
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ERROR_HPP_
#define STARLIGHT_ERROR_HPP_

#include <string>
#include <vector>

namespace sl
{
	///
	/// Returns from a function. Contains information about the result of the function.
	///
	struct Result
	{
		///
		/// Error status to return. Specifically, the type of error.
		///
		enum class Status
		{
			///
			/// When everything is good.
			///
			SUCCESS,

			///
			/// When there is an error or issue, but it does not affect the program running.
			///
			ERROR,

			///
			/// For when there is an unrecoverable error.
			///
			FATAL
		};

		///
		/// Message about the result.
		///
		std::string m_message;

		///
		/// The type of the result.
		///
		sl::Result::Type m_type;
	};

	///
	/// 
	///
	bool operator==(const Result& a, const Result& b)
	{
	}

	bool operator!=(const Result& a, const Result& b)
	{
	}
}

	///
	/// Error class to define and handle any errors.
	///
	class Error
	{
	public:

	private:
		std::vector<std::string> m_history;
	};
}

#endif