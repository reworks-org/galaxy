/** @file
 * @brief D3D12 device, command submission, and swap-chain boilerplate.
 */
#ifdef _WIN32

#include "d3d12_context.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <windows.h>

#include <cstdio>
#include <vector>

namespace sl::detail
{
    namespace
    {
        constexpr std::uint32_t frame_count = 2;

        std::string hresult_error(const char *message, HRESULT result)
        {
            char code[16];
            std::snprintf(code, sizeof(code), "%08lX", static_cast<unsigned long>(result));
            return std::string(message) + " (HRESULT 0x" + code + ").";
        }
    }

    D3D12Context::~D3D12Context()
    {
        shutdown();
    }

    bool D3D12Context::initialise(SDL_Window *window, std::string &error)
    {
        if (!window)
        {
            error = "D3D12 context requires a valid window.";
            return false;
        }
        SDL_SysWMinfo info;
        SDL_VERSION(&info.version);
        if (!SDL_GetWindowWMInfo(window, &info))
        {
            error = SDL_GetError();
            return false;
        }
        int window_width = 0;
        int window_height = 0;
        SDL_GetWindowSize(window, &window_width, &window_height);
        if (window_width <= 0 || window_height <= 0)
        {
            error = "D3D12 context requires a non-zero window size.";
            return false;
        }
        window_ = window;
        width_ = window_width;
        height_ = window_height;

        HRESULT result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0,
                                           IID_PPV_ARGS(&device_));
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 device", result);
            shutdown();
            return false;
        }

        D3D12_COMMAND_QUEUE_DESC queue_desc{};
        queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        result = device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&command_queue_));
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 command queue", result);
            shutdown();
            return false;
        }
        result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                 IID_PPV_ARGS(&command_allocator_));
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 command allocator", result);
            shutdown();
            return false;
        }
        result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                            command_allocator_, nullptr, IID_PPV_ARGS(&command_list_));
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 command list", result);
            shutdown();
            return false;
        }
        command_list_->Close();

        result = device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 fence", result);
            shutdown();
            return false;
        }
        fence_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!fence_event_)
        {
            error = "Unable to create the D3D12 fence event.";
            shutdown();
            return false;
        }

        IDXGIFactory4 *factory = nullptr;
        result = CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
        if (SUCCEEDED(result))
        {
            DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{};
            swap_chain_desc.Width = static_cast<UINT>(width_);
            swap_chain_desc.Height = static_cast<UINT>(height_);
            swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            swap_chain_desc.SampleDesc.Count = 1;
            swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swap_chain_desc.BufferCount = frame_count;
            swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            IDXGISwapChain1 *swap_chain = nullptr;
            result = factory->CreateSwapChainForHwnd(command_queue_, info.info.win.window,
                                                     &swap_chain_desc, nullptr, nullptr, &swap_chain);
            if (SUCCEEDED(result))
            {
                result = swap_chain->QueryInterface(IID_PPV_ARGS(&swap_chain_));
                swap_chain->Release();
            }
            factory->Release();
        }
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 swap chain", result);
            shutdown();
            return false;
        }
        if (!create_backbuffers(error))
        {
            shutdown();
            return false;
        }
        frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
        return true;
    }

    bool D3D12Context::create_backbuffers(std::string &error)
    {
        D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};
        heap_desc.NumDescriptors = frame_count;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        HRESULT result = device_->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&rtv_heap_));
        if (FAILED(result))
        {
            error = hresult_error("Unable to create the D3D12 render-target heap", result);
            return false;
        }
        rtv_descriptor_size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
        for (std::uint32_t index = 0; index < frame_count; ++index)
        {
            result = swap_chain_->GetBuffer(index, IID_PPV_ARGS(&back_buffers_[index]));
            if (FAILED(result))
            {
                error = hresult_error("Unable to retrieve a D3D12 swap-chain buffer", result);
                release_backbuffers();
                return false;
            }
            device_->CreateRenderTargetView(back_buffers_[index], nullptr, handle);
            handle.ptr += rtv_descriptor_size_;
        }
        return true;
    }

    void D3D12Context::release_backbuffers()
    {
        for (auto *&buffer : back_buffers_)
        {
            if (buffer)
                buffer->Release();
            buffer = nullptr;
        }
        if (rtv_heap_)
        {
            rtv_heap_->Release();
            rtv_heap_ = nullptr;
        }
    }

    bool D3D12Context::wait_for_gpu()
    {
        if (!command_queue_ || !fence_ || !fence_event_)
            return false;
        const std::uint64_t value = ++fence_value_;
        if (FAILED(command_queue_->Signal(fence_, value)))
            return false;
        if (fence_->GetCompletedValue() < value)
        {
            if (FAILED(fence_->SetEventOnCompletion(value, static_cast<HANDLE>(fence_event_))))
                return false;
            WaitForSingleObject(static_cast<HANDLE>(fence_event_), INFINITE);
        }
        return true;
    }

    bool D3D12Context::wait_frame()
    {
        if (!fence_ || !fence_event_)
            return false;
        if (fence_->GetCompletedValue() >= fence_value_)
            return true;
        if (FAILED(fence_->SetEventOnCompletion(fence_value_, static_cast<HANDLE>(fence_event_))))
            return false;
        WaitForSingleObject(static_cast<HANDLE>(fence_event_), INFINITE);
        return true;
    }

    std::uint64_t D3D12Context::completed_value() const
    {
        return fence_ ? fence_->GetCompletedValue() : 0;
    }

    bool D3D12Context::wait_idle()
    {
        flush_pending();
        return wait_for_gpu();
    }

    bool D3D12Context::execute_with(ID3D12CommandList *const *lists, unsigned count)
    {
        if (!frame_open_ || FAILED(command_list_->Close()))
            return false;
        std::vector<ID3D12CommandList *> all;
        all.reserve(count + 1);
        all.push_back(command_list_);
        all.insert(all.end(), lists, lists + count);
        command_queue_->ExecuteCommandLists(static_cast<UINT>(all.size()), all.data());
        // The list may be reset right after execution; the allocator keeps growing until the frame ends.
        return SUCCEEDED(command_list_->Reset(command_allocator_, nullptr));
    }

    bool D3D12Context::begin_frame()
    {
        if (!is_valid() || frame_open_)
            return false;
        // The single allocator and the per-frame buffers are reused, so the previous frame must be finished.
        if (!wait_frame())
            return false;
        if (FAILED(command_allocator_->Reset()) || FAILED(command_list_->Reset(command_allocator_, nullptr)))
            return false;
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = back_buffers_[frame_index_];
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        command_list_->ResourceBarrier(1, &barrier);
        frame_open_ = true;
        ++frame_serial_;
        bind_backbuffer();
        return true;
    }

    void D3D12Context::bind_backbuffer()
    {
        flush_pending();
        if (!frame_open_ || !rtv_heap_)
            return;
        D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += static_cast<SIZE_T>(frame_index_) * rtv_descriptor_size_;
        command_list_->OMSetRenderTargets(1, &handle, FALSE, nullptr);
        D3D12_VIEWPORT viewport{0.0f, 0.0f, static_cast<float>(width_),
                                static_cast<float>(height_), 0.0f, 1.0f};
        D3D12_RECT scissor{0, 0, width_, height_};
        command_list_->RSSetViewports(1, &viewport);
        command_list_->RSSetScissorRects(1, &scissor);
    }

    void D3D12Context::present(bool vsync)
    {
        flush_pending();
        if (!swap_chain_ || !frame_open_)
            return;
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = back_buffers_[frame_index_];
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        command_list_->ResourceBarrier(1, &barrier);
        if (SUCCEEDED(command_list_->Close()))
        {
            ID3D12CommandList *lists[] = {command_list_};
            command_queue_->ExecuteCommandLists(1, lists);
            swap_chain_->Present(vsync ? 1 : 0, 0);
            // No wait here: the CPU carries on while the GPU draws, and begin_frame() or wait_idle() waits when it must.
            if (FAILED(command_queue_->Signal(fence_, ++fence_value_)))
                wait_for_gpu();
            frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
        }
        frame_open_ = false;
    }

    void D3D12Context::clear(float red, float green, float blue, float alpha)
    {
        flush_pending();
        if (!frame_open_ || !rtv_heap_)
            return;
        D3D12_CPU_DESCRIPTOR_HANDLE handle = rtv_heap_->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += static_cast<SIZE_T>(frame_index_) * rtv_descriptor_size_;
        const float color[4] = {red, green, blue, alpha};
        command_list_->ClearRenderTargetView(handle, color, 0, nullptr);
    }

    bool D3D12Context::resize(int width, int height, std::string &error)
    {
        if (!swap_chain_ || width <= 0 || height <= 0)
            return false;
        if (frame_open_)
            present(true);
        if (!wait_for_gpu())
        {
            error = "Unable to wait for the D3D12 GPU before resizing.";
            return false;
        }
        release_backbuffers();
        const HRESULT result = swap_chain_->ResizeBuffers(frame_count, static_cast<UINT>(width),
                                                          static_cast<UINT>(height), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        if (FAILED(result))
        {
            error = hresult_error("Unable to resize the D3D12 swap chain", result);
            // Keep rendering at the old size rather than leaving the swap chain without back buffers.
            std::string ignored;
            create_backbuffers(ignored);
            frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
            return false;
        }
        width_ = width;
        height_ = height;
        if (!create_backbuffers(error))
            return false;
        // The flip-model swap chain restarts its buffer numbering after a resize.
        frame_index_ = swap_chain_->GetCurrentBackBufferIndex();
        return true;
    }

    bool D3D12Context::is_valid() const
    {
        return device_ && command_queue_ && command_allocator_ && command_list_ && swap_chain_ && rtv_heap_;
    }

    void D3D12Context::shutdown()
    {
        if (command_queue_ && fence_ && fence_event_)
            wait_for_gpu();
        release_backbuffers();
        if (fence_event_)
        {
            CloseHandle(static_cast<HANDLE>(fence_event_));
            fence_event_ = nullptr;
        }
        if (fence_)
        {
            fence_->Release();
            fence_ = nullptr;
        }
        if (command_list_)
        {
            command_list_->Release();
            command_list_ = nullptr;
        }
        if (command_allocator_)
        {
            command_allocator_->Release();
            command_allocator_ = nullptr;
        }
        if (command_queue_)
        {
            command_queue_->Release();
            command_queue_ = nullptr;
        }
        if (swap_chain_)
        {
            swap_chain_->Release();
            swap_chain_ = nullptr;
        }
        if (device_)
        {
            device_->Release();
            device_ = nullptr;
        }
        window_ = nullptr;
        fence_value_ = 0;
        frame_index_ = 0;
        width_ = 0;
        height_ = 0;
        frame_open_ = false;
    }
}

#endif // _WIN32
