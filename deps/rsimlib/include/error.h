#pragma once

#include <string>

namespace sl
{

    /** Return the most recent library error for the calling thread. */
    const std::string &last_error();
    /** Clear the calling thread's recorded library error. */
    void clear_error();

    namespace detail
    {
        void set_error(const std::string &message);
    }

} // namespace sl