#pragma once

#include <cstddef>

namespace sl
{
    /**
     * Parallel command recording for the Vulkan and Direct3D 12 backends. When a 3D surface issues many separate
     * draw_mesh() calls, the draws past the threshold are queued and recorded into per-thread command buffers at the
     * next point that needs them, then submitted in their original order. OpenGL ignores these settings. Output is
     * identical either way; the only visible effects are lower CPU time and FrameStats::parallel_draws.
     */

    /** Turn parallel recording on or off (default on). Takes effect from the next draw. */
    void set_parallel_recording(bool enabled);
    bool parallel_recording_enabled();

    /** Separate mesh draws in a row before the rest are queued for parallel recording (default 256, minimum 1). */
    void set_parallel_recording_threshold(std::size_t draws);
    std::size_t parallel_recording_threshold();

    /** Threads used for recording, including the calling thread. 0 picks min(4, hardware threads); 1 disables. */
    void set_parallel_recording_threads(unsigned threads);
    unsigned parallel_recording_threads();
} // namespace sl
