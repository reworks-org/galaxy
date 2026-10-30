#pragma once

#include "draw.h"
#include "model3d.h"

#include <cstddef>
#include <memory>
#include <string>

namespace sl
{
    namespace detail
    {
        struct AsyncJob;
    }

    /**
     * Handle to a load running on a worker thread. Files are read and decoded off the main thread; the graphics
     * upload (GPU textures and meshes) happens on the main thread, a little each frame, so loading never stalls
     * a frame. Poll ready()/failed() once per frame, then take the result with get() on the owning subclass.
     *
     * All of this API, including destroying a handle, belongs to the thread that owns the display. Destroying a
     * handle before the load finishes cancels it. Handles are movable and not copyable.
     */
    class AsyncHandle
    {
    public:
        AsyncHandle() = default;
        ~AsyncHandle();
        AsyncHandle(AsyncHandle &&other) noexcept = default;
        AsyncHandle &operator=(AsyncHandle &&other) noexcept;
        AsyncHandle(const AsyncHandle &) = delete;
        AsyncHandle &operator=(const AsyncHandle &) = delete;

        /** True while this handle refers to a load (false when empty, or once the result has been taken). */
        bool valid() const;
        /** True once the load finished and the result can be taken with get(). */
        bool ready() const;
        /** True if the load failed or was cancelled; error() says why. */
        bool failed() const;
        /** True when the load is no longer running (ready() or failed()). */
        bool done() const { return ready() || failed(); }
        /** Why the load failed; empty otherwise. */
        std::string error() const;
        /** Abandon the load; anything already built is released. */
        void cancel();
        /** Block until the load is done, uploading to the GPU as needed. Returns ready(). For loading screens and tools. */
        bool wait();

    protected:
        explicit AsyncHandle(std::shared_ptr<detail::AsyncJob> job) : job_(std::move(job)) {}
        std::shared_ptr<detail::AsyncJob> job_;
        void *take_result();
    };

    /** Settings applied to the texture before it is uploaded (the equivalents of Bitmap::smooth and friends). */
    struct AsyncBitmapOptions
    {
        bool smooth = false;
        bool mipmaps = false;
        float anisotropy = 1.0f;
        /**
         * Invert the red, green and blue channels on the worker thread, then raise each to at least `invert_floor`
         * (0-255). Turns a gloss map (white is shiny) into the roughness map that Material::metallic_roughness wants.
         */
        bool invert = false;
        int invert_floor = 0;
    };

    /** A bitmap loading in the background. The texture is already on the GPU when ready() turns true. */
    class AsyncBitmap : public AsyncHandle
    {
    public:
        AsyncBitmap() = default;
        /** Take the finished bitmap (you own it; free with destroy_bitmap()). Returns nullptr until ready(), and after it was taken. */
        Bitmap *get() { return static_cast<Bitmap *>(take_result()); }

    private:
        friend AsyncBitmap load_bitmap_async(const std::string &, const AsyncBitmapOptions &);
        explicit AsyncBitmap(std::shared_ptr<detail::AsyncJob> job) : AsyncHandle(std::move(job)) {}
    };

    /** A model loading in the background. Its meshes and textures are on the GPU when ready() turns true. */
    class AsyncModel : public AsyncHandle
    {
    public:
        AsyncModel() = default;
        /** Take the finished model (you own it; free with destroy_model()). Returns nullptr until ready(), and after it was taken. */
        Model *get() { return static_cast<Model *>(take_result()); }

    private:
        friend AsyncModel load_model_async(const std::string &);
        explicit AsyncModel(std::shared_ptr<detail::AsyncJob> job) : AsyncHandle(std::move(job)) {}
    };

    /** Start loading an image file in the background (same formats as load_bitmap(), plus .dds files, which are decoded to RGBA). */
    AsyncBitmap load_bitmap_async(const std::string &path, const AsyncBitmapOptions &options = {});
    /** Start loading an .obj, .gltf, .glb or .ac model in the background (same as load_model()). */
    AsyncModel load_model_async(const std::string &path);

    /**
     * Limit how many bytes of texture and mesh data are uploaded to the GPU per frame by end_frame() (default 8 MiB).
     * At least one item is always uploaded per frame, so a single huge texture cannot stall the queue.
     */
    void set_async_upload_budget(std::size_t bytes_per_frame);
    /** Number of async loads that are not finished yet; use for a loading-screen progress readout. */
    std::size_t async_loads_pending();

} // namespace sl
