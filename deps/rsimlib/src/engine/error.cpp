/** @file
 * @brief Implements thread-local error reporting for the library.
 */

#include "error.h"

namespace sl
{
	namespace
	{
		thread_local std::string error;
		thread_local std::move_only_function<void(const std::string&)> logback;
	}

	const std::string &last_error()
	{
		return error;
	}

	void clear_error()
	{
		error.clear();
	}

	void set_log_callback(std::move_only_function<void(const std::string&)> callback)
	{
		logback = std::move(callback);
	}

	namespace detail
	{

		void set_error(const std::string &message)
		{
			if (logback)
			{
				logback(message);
			}

			error = message;
		}

	} // namespace detail
} // namespace sl