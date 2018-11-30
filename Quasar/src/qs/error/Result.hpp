///
/// Result.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RESULT_HPP_
#define QUASAR_RESULT_HPP_

#include <string>

namespace qs
{
	///
	/// Returns from a function. Contains information about the result of the function.
	///
	struct Result
	{
		///
		/// Comparison object for success cases.
		///
		static qs::Result SUCCESS;

		///
		/// Comparison object for error cases.
		///
		static qs::Result ERROR;

		///
		/// Comparison object for fatal cases.
		///
		static qs::Result FATAL;

		///
		/// Default constructor.
		///
		Result();

		///
		/// Alternate constructor.
		///
		Result(qs::Result::Status status, const std::string& message = "");

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
		qs::Result::Status m_status;
	};

	///
	/// Comparison operator for results.
	///
	bool operator==(const qs::Result& a, const qs::Result& b)
	{
		return (a.m_type == b.m_type) ? true : false;
	}

	///
	/// Comparison operator for not results.
	///
	bool operator!=(const qs::Result& a, const qs::Result& b)
	{
		return (a.m_type != b.m_type) ? true : false;
	}
}

#endif