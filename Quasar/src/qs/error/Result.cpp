///
/// Result.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Result.hpp"

namespace qs
{
	// Static declarations.
	qs::Result qs::Result::SUCCESS(qs::Result::Status::SUCCESS);
	qs::Result qs::Result::ERROR(qs::Result::Status::ERROR);
	qs::Result qs::Result::FATAL(qs::Result::Status::FATAL);

	Result::Result()
		:m_message(""), m_status(qs::Result::Status::SUCCESS)
	{
	}

	Result::Result(qs::Result::Status status, const std::string& message)
		: m_message(message), m_status(status)
	{
	}
}