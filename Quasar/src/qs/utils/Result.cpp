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
	qs::Result qs::Result::SUCCESS("success comparitor", qs::Result::Status::SUCCESS);
	qs::Result qs::Result::FAILURE("failure comparitor", qs::Result::Status::FAILURE);

	Result::Result() noexcept
		:m_message("default"), m_status(qs::Result::Status::SUCCESS)
	{
	}

	Result::Result(const std::string& message, qs::Result::Status status) noexcept
		: m_message(message), m_status(status)
	{
	}
}