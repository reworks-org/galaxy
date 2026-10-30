#pragma once

#include <string>
#include <functional>

namespace sl
{

    /** Return the most recent library error for the calling thread. */
    const std::string &last_error();
    /** Clear the calling thread's recorded library error. */
    void clear_error();

	void set_log_callback(std::move_only_function<void(const std::string&)> callback);
    namespace detail
    {
        void set_error(const std::string &message);
    }

} // namespace sl