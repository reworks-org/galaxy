#include "async_load.h"

#include "async_internal.h"
#include "dds.h"
#include "deferred_mesh.h"
#include "error.h"

#include <algorithm>
#include <cctype>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <limits>
#include <mutex>
#include <thread>
#include <vector>

namespace sl
{
    namespace detail
    {
        namespace
        {
            enum JobState : int
            {
                state_queued,
                state_decoding,
                state_decoded, // On the CPU, waiting for the main thread to upload it.
                state_ready,
                state_failed,
                state_taken
            };
        } // namespace

        struct AsyncJob
        {
            bool is_model = false;
            std::string path;
            AsyncBitmapOptions options;
            bool retain_mesh_data = false;

            std::atomic<int> state{state_queued};
            std::atomic<bool> cancelled{false};
            std::string error; // Written before state becomes state_failed.

            Bitmap *bitmap = nullptr;
            Model *model = nullptr;
            std::vector<DeferredMesh> meshes;
            std::size_t next_mesh = 0;
            std::size_t next_texture = 0;
        };

        namespace
        {
            struct Pool
            {
                std::mutex mutex;
                std::condition_variable wake;
                std::vector<std::thread> workers;
                std::deque<std::shared_ptr<AsyncJob>> queue;
                std::vector<std::shared_ptr<AsyncJob>> decoded; // Handed from workers to the main thread.
                std::vector<std::shared_ptr<AsyncJob>> registry; // Every job not yet taken or failed.
                std::vector<std::shared_ptr<AsyncJob>> uploading; // Main thread only.
                bool stop = false;
                std::size_t budget = 8u * 1024u * 1024u;
            };

            Pool &pool()
            {
                static Pool instance;
                return instance;
            }

            void dispose_payload(AsyncJob &job)
            {
                if (job.model)
                    destroy_model(job.model);
                if (job.bitmap)
                    destroy_bitmap(job.bitmap);
                job.model = nullptr;
                job.bitmap = nullptr;
                job.meshes.clear();
            }

            void remove_from_registry(Pool &p, const std::shared_ptr<AsyncJob> &job)
            {
                p.registry.erase(std::remove(p.registry.begin(), p.registry.end(), job), p.registry.end());
            }

            void fail(AsyncJob &job, std::string message)
            {
                job.error = std::move(message);
                job.state.store(state_failed, std::memory_order_release);
            }

            void run_job(const std::shared_ptr<AsyncJob> &job)
            {
                Pool &p = pool();
                if (job->cancelled.load())
                {
                    std::lock_guard<std::mutex> lock(p.mutex);
                    fail(*job, "The load was cancelled");
                    remove_from_registry(p, job);
                    return;
                }
                job->state.store(state_decoding);
                clear_error();
                if (job->is_model)
                {
                    set_mesh_cpu_data_retained(job->retain_mesh_data);
                    DeferredMeshScope scope(job->meshes);
                    job->model = load_model(job->path);
                }
                else
                {
                    const std::size_t dot = job->path.rfind('.');
                    std::string extension = dot == std::string::npos ? std::string() : job->path.substr(dot);
                    for (char &c : extension)
                        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                    if (extension == ".dds")
                    {
                        // Decoded to RGBA here, because the GPU upload happens on the main thread.
                        DdsOptions dds;
                        dds.allow_compressed_upload = false;
                        job->bitmap = load_dds(job->path, dds);
                    }
                    else
                        job->bitmap = load_bitmap(job->path);
                    if (job->bitmap && job->options.invert && !job->bitmap->pixels.empty())
                    {
                        const int floor = std::clamp(job->options.invert_floor, 0, 255);
                        std::vector<std::uint8_t> &px = job->bitmap->pixels;
                        for (std::size_t i = 0; i + 3 < px.size(); i += 4)
                            for (int k = 0; k < 3; ++k)
                                px[i + k] = static_cast<std::uint8_t>(std::max(255 - px[i + k], floor));
                    }
                    if (job->bitmap)
                    {
                        job->bitmap->smooth = job->options.smooth;
                        job->bitmap->mipmaps = job->options.mipmaps;
                        job->bitmap->anisotropy = job->options.anisotropy;
                    }
                }
                std::lock_guard<std::mutex> lock(p.mutex);
                if (!job->model && !job->bitmap)
                {
                    job->meshes.clear();
                    fail(*job, last_error().empty() ? "Unable to load " + job->path : last_error());
                    remove_from_registry(p, job);
                    return;
                }
                job->state.store(state_decoded);
                p.decoded.push_back(job);
            }

            thread_local bool t_worker = false;

            void worker_loop()
            {
                t_worker = true;
                Pool &p = pool();
                for (;;)
                {
                    std::shared_ptr<AsyncJob> job;
                    {
                        std::unique_lock<std::mutex> lock(p.mutex);
                        p.wake.wait(lock, [&p] { return p.stop || !p.queue.empty(); });
                        if (p.stop)
                            return;
                        job = std::move(p.queue.front());
                        p.queue.pop_front();
                    }
                    run_job(job);
                }
            }

            std::shared_ptr<AsyncJob> submit(std::shared_ptr<AsyncJob> job)
            {
                Pool &p = pool();
                std::lock_guard<std::mutex> lock(p.mutex);
                if (p.workers.empty())
                {
                    const unsigned hardware = std::thread::hardware_concurrency();
                    const unsigned count = std::clamp<unsigned>(hardware > 1 ? hardware - 1 : 1, 1, 4);
                    for (unsigned i = 0; i < count; ++i)
                        p.workers.emplace_back(worker_loop);
                }
                p.registry.push_back(job);
                p.queue.push_back(job);
                p.wake.notify_one();
                return job;
            }

            std::size_t charge(std::size_t &remaining, std::size_t bytes)
            {
                remaining = bytes >= remaining ? 0 : remaining - bytes;
                return bytes;
            }

            /** Upload as much of the job as the budget allows; returns true when nothing is left to do. */
            bool upload_step(AsyncJob &job, std::size_t &remaining, bool &uploaded_any)
            {
                if (!job.is_model)
                {
                    if (uploaded_any && remaining == 0)
                        return false;
                    uploaded_any = true;
                    charge(remaining, static_cast<std::size_t>(job.bitmap->width) * job.bitmap->height * 4);
                    if (!upload_bitmap(job.bitmap))
                    {
                        dispose_payload(job);
                        fail(job, "Unable to upload the texture to the graphics backend");
                        return true;
                    }
                    job.state.store(state_ready, std::memory_order_release);
                    return true;
                }
                while (job.next_mesh < job.meshes.size())
                {
                    if (uploaded_any && remaining == 0)
                        return false;
                    const DeferredMesh &deferred = job.meshes[job.next_mesh];
                    uploaded_any = true;
                    charge(remaining, deferred.mesh->vertices.size() * sizeof(Vertex3D) +
                                          deferred.mesh->indices.size() * sizeof(std::uint32_t));
                    if (!upload_deferred_mesh(deferred))
                    {
                        std::string message = last_error();
                        dispose_payload(job);
                        fail(job, std::move(message));
                        return true;
                    }
                    ++job.next_mesh;
                }
                while (job.next_texture < job.model->textures.size())
                {
                    if (uploaded_any && remaining == 0)
                        return false;
                    Bitmap *texture = job.model->textures[job.next_texture];
                    uploaded_any = true;
                    charge(remaining, static_cast<std::size_t>(texture->width) * texture->height * 4);
                    if (!upload_bitmap(texture))
                    {
                        dispose_payload(job);
                        fail(job, "Unable to upload a model texture to the graphics backend");
                        return true;
                    }
                    ++job.next_texture;
                }
                job.state.store(state_ready, std::memory_order_release);
                return true;
            }
        } // namespace

        bool is_async_worker_thread()
        {
            return t_worker;
        }

        void pump_async_loads()
        {
            std::size_t budget = 0;
            {
                std::lock_guard<std::mutex> lock(pool().mutex);
                budget = pool().budget;
            }
            pump_async_loads(budget);
        }

        void pump_async_loads(std::size_t budget_bytes)
        {
            Pool &p = pool();
            {
                std::lock_guard<std::mutex> lock(p.mutex);
                p.uploading.insert(p.uploading.end(), p.decoded.begin(), p.decoded.end());
                p.decoded.clear();
            }
            if (p.uploading.empty())
                return;
            std::size_t remaining = budget_bytes;
            bool uploaded_any = false;
            std::vector<std::shared_ptr<AsyncJob>> finished;
            for (const std::shared_ptr<AsyncJob> &job : p.uploading)
            {
                if (job->cancelled.load())
                {
                    dispose_payload(*job);
                    fail(*job, "The load was cancelled");
                    finished.push_back(job);
                    continue;
                }
                if (upload_step(*job, remaining, uploaded_any))
                    finished.push_back(job);
                else
                    break;
            }
            std::lock_guard<std::mutex> lock(p.mutex);
            for (const std::shared_ptr<AsyncJob> &job : finished)
            {
                p.uploading.erase(std::remove(p.uploading.begin(), p.uploading.end(), job), p.uploading.end());
                if (job->state.load() == state_failed)
                    remove_from_registry(p, job);
            }
        }

        void async_load_shutdown()
        {
            Pool &p = pool();
            std::vector<std::thread> workers;
            {
                std::lock_guard<std::mutex> lock(p.mutex);
                if (p.workers.empty() && p.registry.empty())
                    return;
                p.stop = true;
                workers.swap(p.workers);
            }
            p.wake.notify_all();
            for (std::thread &worker : workers)
                worker.join();
            std::lock_guard<std::mutex> lock(p.mutex);
            for (const std::shared_ptr<AsyncJob> &job : p.registry)
            {
                dispose_payload(*job);
                if (job->state.load() != state_failed)
                    fail(*job, "Graphics shut down before the load finished");
            }
            p.registry.clear();
            p.queue.clear();
            p.decoded.clear();
            p.uploading.clear();
            p.stop = false;
        }
    } // namespace detail

    using detail::AsyncJob;

    AsyncHandle::~AsyncHandle()
    {
        cancel();
    }

    AsyncHandle &AsyncHandle::operator=(AsyncHandle &&other) noexcept
    {
        if (this != &other)
        {
            cancel();
            job_ = std::move(other.job_);
        }
        return *this;
    }

    bool AsyncHandle::valid() const
    {
        return job_ && job_->state.load() != detail::state_taken;
    }

    bool AsyncHandle::ready() const
    {
        return job_ && job_->state.load(std::memory_order_acquire) == detail::state_ready;
    }

    bool AsyncHandle::failed() const
    {
        return job_ && job_->state.load(std::memory_order_acquire) == detail::state_failed;
    }

    std::string AsyncHandle::error() const
    {
        return failed() ? job_->error : std::string();
    }

    void AsyncHandle::cancel()
    {
        if (!job_)
            return;
        job_->cancelled.store(true);
        if (job_->state.load() == detail::state_ready)
        {
            detail::Pool &p = detail::pool();
            std::lock_guard<std::mutex> lock(p.mutex);
            detail::dispose_payload(*job_);
            job_->state.store(detail::state_taken);
            detail::remove_from_registry(p, job_);
        }
        job_.reset();
    }

    bool AsyncHandle::wait()
    {
        while (job_ && !done())
        {
            detail::pump_async_loads(std::numeric_limits<std::size_t>::max());
            if (!done())
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        return ready();
    }

    void *AsyncHandle::take_result()
    {
        if (!ready())
            return nullptr;
        detail::Pool &p = detail::pool();
        std::lock_guard<std::mutex> lock(p.mutex);
        void *result = job_->is_model ? static_cast<void *>(job_->model) : static_cast<void *>(job_->bitmap);
        job_->model = nullptr;
        job_->bitmap = nullptr;
        job_->meshes.clear();
        job_->state.store(detail::state_taken);
        detail::remove_from_registry(p, job_);
        job_.reset();
        return result;
    }

    AsyncBitmap load_bitmap_async(const std::string &path, const AsyncBitmapOptions &options)
    {
        auto job = std::make_shared<AsyncJob>();
        job->path = path;
        job->options = options;
        return AsyncBitmap(detail::submit(std::move(job)));
    }

    AsyncModel load_model_async(const std::string &path)
    {
        auto job = std::make_shared<AsyncJob>();
        job->is_model = true;
        job->path = path;
        job->retain_mesh_data = mesh_cpu_data_retained();
        return AsyncModel(detail::submit(std::move(job)));
    }

    void set_async_upload_budget(std::size_t bytes_per_frame)
    {
        detail::Pool &p = detail::pool();
        std::lock_guard<std::mutex> lock(p.mutex);
        p.budget = bytes_per_frame;
    }

    std::size_t async_loads_pending()
    {
        detail::Pool &p = detail::pool();
        std::lock_guard<std::mutex> lock(p.mutex);
        std::size_t pending = 0;
        for (const std::shared_ptr<AsyncJob> &job : p.registry)
            if (job->state.load() < detail::state_ready)
                ++pending;
        return pending;
    }

} // namespace sl
