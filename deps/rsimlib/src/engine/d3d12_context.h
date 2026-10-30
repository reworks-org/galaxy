#pragma once

#ifdef _WIN32

#include <SDL2/SDL_video.h>

#include <cstdint>
#include <functional>
#include <string>

struct ID3D12CommandAllocator;
struct ID3D12CommandList;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandQueue;
struct ID3D12DescriptorHeap;
struct ID3D12Device;
struct ID3D12Fence;
struct ID3D12Resource;
struct ID3D12DescriptorHeap;
struct IDXGISwapChain3;

namespace sl::detail
{
    /** Owns the D3D12 device, command submission objects, swap chain, and back buffers. */
    class D3D12Context
    {
    public:
        D3D12Context() = default;
        ~D3D12Context();

        D3D12Context(const D3D12Context &) = delete;
        D3D12Context &operator=(const D3D12Context &) = delete;

        bool initialise(SDL_Window *window, std::string &error);
        void shutdown();
        bool resize(int width, int height, std::string &error);
        bool begin_frame();
        void present(bool vsync);
        void clear(float red, float green, float blue, float alpha);
        bool wait_idle();
        /** Waits until the GPU has finished the last presented frame (cheap when it already has). */
        bool wait_frame();
        /** The fence value that will cover everything submitted from now on. */
        std::uint64_t retire_value() const { return fence_value_ + 1; }
        std::uint64_t completed_value() const;
        bool is_valid() const;
        bool frame_open() const { return frame_open_; }
        std::uint64_t frame_serial() const { return frame_serial_; }
        int width() const { return width_; }
        int height() const { return height_; }
        ID3D12Resource *back_buffer() const { return back_buffers_[frame_index_]; }
        void bind_backbuffer();

        ID3D12Device *device() const { return device_; }
        /** Returns the primary list; any queued parallel draws are recorded first so command order is kept. */
        ID3D12GraphicsCommandList *command_list() const
        {
            flush_pending();
            return command_list_;
        }
        /** The primary list without flushing queued draws (for the flush itself). */
        ID3D12GraphicsCommandList *raw_command_list() const { return command_list_; }
        /** Installs the callback that records queued draws; it is cleared before it runs. */
        void set_flush_hook(std::function<void()> hook) { flush_hook_ = std::move(hook); }
        void flush_pending() const
        {
            if (!flush_hook_)
                return;
            auto hook = std::move(flush_hook_);
            flush_hook_ = nullptr;
            hook();
        }
        /** Closes and executes the primary list followed by `lists`, then reopens the primary list. */
        bool execute_with(ID3D12CommandList *const *lists, unsigned count);
        ID3D12CommandQueue *command_queue() const { return command_queue_; }

    private:
        bool create_backbuffers(std::string &error);
        void release_backbuffers();
        bool wait_for_gpu();

        SDL_Window *window_ = nullptr;
        ID3D12Device *device_ = nullptr;
        ID3D12CommandQueue *command_queue_ = nullptr;
        ID3D12CommandAllocator *command_allocator_ = nullptr;
        ID3D12GraphicsCommandList *command_list_ = nullptr;
        IDXGISwapChain3 *swap_chain_ = nullptr;
        ID3D12DescriptorHeap *rtv_heap_ = nullptr;
        ID3D12Resource *back_buffers_[2] = {};
        ID3D12Fence *fence_ = nullptr;
        void *fence_event_ = nullptr;
        std::uint64_t fence_value_ = 0;
        std::uint32_t rtv_descriptor_size_ = 0;
        std::uint32_t frame_index_ = 0;
        int width_ = 0;
        int height_ = 0;
        mutable std::function<void()> flush_hook_;
        bool frame_open_ = false;
        std::uint64_t frame_serial_ = 0;
    };
}

#endif // _WIN32
