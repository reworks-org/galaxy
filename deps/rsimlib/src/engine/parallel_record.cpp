#include "parallel_record.h"

#include "parallel_render.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace sl
{
    namespace
    {
        std::atomic<bool> g_enabled{true};
        std::atomic<std::size_t> g_threshold{256};
        std::atomic<unsigned> g_max_threads{0};
        std::atomic<std::uint64_t> g_parallel_draws{0};

        unsigned auto_threads()
        {
            static const unsigned cached = std::min(4u, std::max(1u, std::thread::hardware_concurrency()));
            return cached;
        }
    } // namespace

    void set_parallel_recording(bool enabled) { g_enabled = enabled; }
    bool parallel_recording_enabled() { return g_enabled; }
    void set_parallel_recording_threshold(std::size_t draws) { g_threshold = std::max<std::size_t>(draws, 1); }
    std::size_t parallel_recording_threshold() { return g_threshold; }
    void set_parallel_recording_threads(unsigned threads) { g_max_threads = threads; }
    unsigned parallel_recording_threads()
    {
        const unsigned requested = g_max_threads;
        return requested == 0 ? auto_threads() : std::min(requested, 16u);
    }

    namespace detail
    {
        namespace
        {
            class Pool
            {
            public:
                ~Pool() { stop(); }

                void run(unsigned chunks, const std::function<void(unsigned)> &fn)
                {
                    if (chunks <= 1)
                    {
                        if (chunks == 1)
                            fn(0);
                        return;
                    }
                    std::lock_guard<std::mutex> dispatch(dispatch_mutex_);
                    ensure_workers(chunks - 1);
                    {
                        std::lock_guard<std::mutex> lock(mutex_);
                        job_ = &fn;
                        total_ = chunks;
                        next_ = 0;
                        remaining_ = chunks;
                        ++generation_;
                    }
                    wake_.notify_all();
                    work();
                    std::unique_lock<std::mutex> lock(mutex_);
                    done_.wait(lock, [&] { return remaining_ == 0; });
                    job_ = nullptr;
                }

                void stop()
                {
                    std::lock_guard<std::mutex> dispatch(dispatch_mutex_);
                    {
                        std::lock_guard<std::mutex> lock(mutex_);
                        quit_ = true;
                    }
                    wake_.notify_all();
                    for (std::thread &worker : workers_)
                        if (worker.joinable())
                            worker.join();
                    workers_.clear();
                    std::lock_guard<std::mutex> lock(mutex_);
                    quit_ = false;
                }

            private:
                void ensure_workers(unsigned wanted)
                {
                    while (workers_.size() < wanted)
                        workers_.emplace_back([this] { worker_loop(); });
                }

                // Claim chunks until none are left; each chunk index is handed out exactly once.
                // The job is read under the lock together with the index so a late worker never runs a stale job.
                void work()
                {
                    for (;;)
                    {
                        unsigned index;
                        const std::function<void(unsigned)> *fn;
                        {
                            std::lock_guard<std::mutex> lock(mutex_);
                            if (!job_ || next_ >= total_)
                                return;
                            index = next_++;
                            fn = job_;
                        }
                        (*fn)(index);
                        std::lock_guard<std::mutex> lock(mutex_);
                        if (--remaining_ == 0)
                            done_.notify_all();
                    }
                }

                void worker_loop()
                {
                    std::uint64_t seen = 0;
                    for (;;)
                    {
                        {
                            std::unique_lock<std::mutex> lock(mutex_);
                            wake_.wait(lock, [&] { return quit_ || generation_ != seen; });
                            if (quit_)
                                return;
                            seen = generation_;
                        }
                        work();
                    }
                }

                std::mutex dispatch_mutex_;
                std::mutex mutex_;
                std::condition_variable wake_;
                std::condition_variable done_;
                std::vector<std::thread> workers_;
                const std::function<void(unsigned)> *job_ = nullptr;
                unsigned total_ = 0;
                unsigned next_ = 0;
                unsigned remaining_ = 0;
                std::uint64_t generation_ = 0;
                bool quit_ = false;
            };

            Pool &pool()
            {
                static Pool instance;
                return instance;
            }
        } // namespace

        ParallelRecordSettings parallel_record_settings()
        {
            return {g_enabled, g_threshold, parallel_recording_threads()};
        }

        unsigned parallel_record_chunks(std::size_t draws)
        {
            const ParallelRecordSettings settings = parallel_record_settings();
            if (!settings.enabled || settings.max_threads <= 1 || draws < settings.threshold)
                return 1;
            // Keep every chunk big enough that recording it outweighs waking a worker.
            const std::size_t useful = std::max<std::size_t>(draws / 128, 1);
            return static_cast<unsigned>(std::min<std::size_t>(settings.max_threads, useful));
        }

        void parallel_record_run(unsigned chunks, const std::function<void(unsigned)> &fn) { pool().run(chunks, fn); }
        std::uint64_t parallel_draws_recorded() { return g_parallel_draws; }
        void parallel_draws_add(std::uint64_t draws) { g_parallel_draws += draws; }
        void parallel_record_shutdown() { pool().stop(); }
    } // namespace detail
} // namespace sl
