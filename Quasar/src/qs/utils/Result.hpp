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
	struct Result final
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
			/// When there is an error or issue, etc.
			///
			FAILURE
		};

		///
		/// Comparison object for success cases.
		///
		static qs::Result SUCCESS;

		///
		/// Comparison object for failed cases.
		///
		static qs::Result FAILURE;

		///
		/// Default constructor.
		///
		Result() noexcept;

		///
		/// Alternate constructor.
		///
		explicit Result(const std::string& message, qs::Result::Status status) noexcept;

		///
		/// Overload logical comparison operations.
		///
		explicit inline operator bool() const noexcept
		{
			return (m_status == qs::Result::Status::SUCCESS);
		}

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
	inline bool operator==(const qs::Result& a, const qs::Result& b) noexcept
	{
		return (a.m_status == b.m_status) ? true : false;
	}

	///
	/// Comparison operator for not results.
	///
	inline bool operator!=(const qs::Result& a, const qs::Result& b) noexcept
	{
		return (a.m_status != b.m_status) ? true : false;
	}
}

#endif