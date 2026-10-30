#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace sl::detail
{
    /** Settings behind set_parallel_recording() and friends. Readable from any thread. */
    struct ParallelRecordSettings
    {
        bool enabled;
        std::size_t threshold;
        unsigned max_threads;
    };
    ParallelRecordSettings parallel_record_settings();

    /** How many chunks a batch of `draws` should be split into (1 means record inline). */
    unsigned parallel_record_chunks(std::size_t draws);

    /**
     * Run fn(0..chunks-1) concurrently on the persistent recording workers plus the calling thread and return when
     * all have finished. Chunk i is always run by exactly one thread. Not re-entrant; main thread only.
     */
    void parallel_record_run(unsigned chunks, const std::function<void(unsigned)> &fn);

    /** Cumulative number of draws recorded through parallel_record_run() batches (for FrameStats). */
    std::uint64_t parallel_draws_recorded();
    void parallel_draws_add(std::uint64_t draws);

    /** Stop and join the workers. Safe to call more than once; the pool restarts lazily. */
    void parallel_record_shutdown();
} // namespace sl::detail
