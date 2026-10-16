#pragma once

/** @file
 * @brief Declares the internal shared SDL_mixer backend helpers.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <mutex>

namespace sl::audio_detail
{

    /** Acquire and initialize the shared mixer. */
    bool acquire_mixer();
    /** Release the shared mixer. */
    void release_mixer();
    /** Return the mutex protecting mixer operations. */
    std::mutex &mixer_mutex();

} // namespace sl::audio_detail