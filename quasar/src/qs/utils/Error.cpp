///
/// Error.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <qs/libs/glad/glad.h>

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
	}

	void Error::setGLCallback(void(*callback)(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)) noexcept
	{
		glDebugMessageCallback(callback, nullptr);
	}

	void Error::setQSCallback(std::function<void(std::string_view, unsigned int, std::string_view)> callback) noexcept
	{
		m_callback = callback;
	}

	void Error::callback(std::string_view file, unsigned int line, std::string_view message) noexcept
	{
		if (m_callback != nullptr)
		{
			m_history.emplace_back(message);
			m_callback(file, line, message);
		}
	}

	const std::vector<std::string>& Error::getHistory() const noexcept
	{
		return m_history;
	}

	void Error::clear() noexcept
	{
		m_history.clear();
	}

	Error::Error() noexcept
		:m_callback(nullptr), m_history()
	{
	}
}