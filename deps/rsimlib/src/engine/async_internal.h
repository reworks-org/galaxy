#pragma once

#include <cstddef>

namespace sl::detail
{
    /** Upload finished async loads to the GPU, up to `budget_bytes` (always at least one item). Main thread only. */
    void pump_async_loads(std::size_t budget_bytes);
    /** pump_async_loads() with the budget from set_async_upload_budget(). Called by end_frame(). */
    void pump_async_loads();
    /** True on an async loader's worker thread, where bitmaps are CPU-only and touch no shared state. */
    bool is_async_worker_thread();
    /** Stop the workers and release every unclaimed async result. Called while the graphics backend is still alive. */
    void async_load_shutdown();
} // namespace sl::detail
