///
/// Error.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Error.hpp"

namespace qs
{
	///
	/// Static declaration.
	///
	qs::Error Error::handle;

	Error::~Error() noexcept
	{
		clear();
		m_history.clear();
	}

	void Error::setError(const std::string& message) noexcept
	{
		m_currentError = message;
		m_history.push_back(message);
	}

	const std::string& Error::current() const noexcept
	{
		return m_currentError;
	}

	const std::vector<std::string>& Error::getHistory() const noexcept
	{
		return m_history;
	}

	void Error::clear() noexcept
	{
		m_currentError = "";
	}

	Error::Error() noexcept
		:m_currentError(""), m_history()
	{
	}
}