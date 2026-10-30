/** @file
 * @brief D3D12 renderer; only compiled on Windows.
 */
#ifdef _WIN32

#include "d3d12_renderer.h"
#include "d3d12_context.h"
#include "d3d12_shader_translator.h"
#include "parallel_record.h"
#include "shader_paths.h"

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <windows.h>
#ifdef SIMLIB_HAVE_DXC
#include <dxcapi.h>
#endif

#ifdef SIMLIB_GUI_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_dx12.h>
#endif

#include <algorithm>
#include <array>
#include <climits>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sl::detail
{
    namespace
    {
        constexpr std::size_t max_batch_vertices = 65536;
        // Each flush within a frame needs its own region; the GPU reads them after recording ends.
        constexpr std::size_t vertex_buffer_vertices = max_batch_vertices * 16;
        constexpr std::uint32_t descriptor_capacity = 4096;
        constexpr std::uint32_t render_target_capacity = 256;
        // Shader-visible descriptors after the texture range, rewritten for each draw or dispatch.
        constexpr std::uint32_t transient_descriptor_capacity = 8192;
        constexpr std::size_t constant_buffer_bytes = 4 * 1024 * 1024;
        constexpr std::uint32_t max_storage_bindings = 16;
        constexpr std::uint32_t max_texture_units = 32;
        // fxc (shader model 5) allows 16 samplers per shader; DXC (shader model 6) allows every texture unit.
        constexpr std::uint32_t max_program_samplers_sm5 = 16;
        constexpr std::uint32_t max_program_samplers_sm6 = max_texture_units;
        // Shader-visible sampler descriptors after the texture range, rewritten for each draw or dispatch.
        // Shader-visible sampler heaps are capped at 2048 descriptors, so textures share a small table of deduplicated samplers.
        constexpr std::uint32_t persistent_sampler_capacity = 128;
        constexpr std::uint32_t transient_sampler_capacity = 1024;
        constexpr std::uint32_t max_image_units = translated_max_images;

        struct Texture
        {
            ID3D12Resource *resource = nullptr;
            std::uint32_t descriptor = 0;
            std::uint32_t sampler = 0;
            D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COPY_DEST;
        };

        struct RenderTarget
        {
            std::uint32_t texture = 0;
            std::uint32_t descriptor = 0;
            std::uint32_t depth_descriptor = render_target_capacity;
            ID3D12Resource *depth = nullptr;
            // Multisampled colour buffer (always kept in the render-target state); resolved into the texture.
            ID3D12Resource *msaa_colour = nullptr;
            int samples = 1;
            int width = 0;
            int height = 0;
        };

        struct Mesh
        {
            ID3D12Resource *vertices = nullptr;
            ID3D12Resource *indices = nullptr;
            D3D12_VERTEX_BUFFER_VIEW vertex_view{};
            D3D12_INDEX_BUFFER_VIEW index_view{};
            UINT index_count = 0;
        };

        struct D3D12InstanceBuffer
        {
            ID3D12Resource *resource = nullptr;
            std::size_t capacity = 0;
            D3D12_VERTEX_BUFFER_VIEW view{};
        };

        struct StorageBuffer
        {
            ID3D12Resource *resource = nullptr;
            std::size_t size = 0;
        };

        // Blend variants: 0 straight alpha, 1 premultiplied alpha, 2 additive, 3 multiply, 4 screen.
        constexpr std::size_t blend_variant_count = 5;
        constexpr std::size_t primitive_pipeline_count = 3;
        // Pipelines are built per render-target sample count (1, 2, 4 or 8).
        constexpr std::size_t sample_slot_count = 4;
        constexpr std::size_t pipeline_set_size = primitive_pipeline_count * blend_variant_count;

        inline std::size_t sample_slot(int samples)
        {
            return samples >= 8 ? 3 : samples >= 4 ? 2 : samples >= 2 ? 1 : 0;
        }

        struct ShaderProgram
        {
            bool compute = false;
            ID3DBlob *vertex_blob = nullptr;
            ID3DBlob *pixel_blob = nullptr;
            ID3DBlob *compute_blob = nullptr;
            ID3D12RootSignature *root_signature = nullptr;
            TranslatedShader vertex;
            // For compute programs this holds the compute shader's reflection data.
            TranslatedShader pixel;
            std::vector<std::uint8_t> vertex_constants;
            std::vector<std::uint8_t> pixel_constants;
            std::vector<std::string> samplers;
            std::vector<int> sampler_units;
            std::uint32_t image_registers = 0;
            int image_parameter = -1;
            std::uint32_t storage_registers = 0;
            int vertex_constants_parameter = -1;
            int pixel_constants_parameter = -1;
            int storage_parameter = -1;
            int texture_parameter_base = -1;
            int sampler_parameter_base = -1;
            std::array<ID3D12PipelineState *, pipeline_set_size * sample_slot_count> pipelines{};
            ID3D12PipelineState *compute_pipeline = nullptr;
            // Set once the DXC-built shaders were rejected at pipeline creation and fxc (SM 5.1) replaced them.
            bool fxc_fallback = false;
        };

        D3D12_BLEND_DESC blend_description(std::size_t variant)
        {
            D3D12_BLEND_DESC desc{};
            auto &blend = desc.RenderTarget[0];
            blend.BlendEnable = TRUE;
            blend.SrcBlend = variant == 0 ? D3D12_BLEND_SRC_ALPHA : variant == 3 ? D3D12_BLEND_DEST_COLOR : D3D12_BLEND_ONE;
            blend.DestBlend = variant == 2 ? D3D12_BLEND_ONE : variant == 4 ? D3D12_BLEND_INV_SRC_COLOR : D3D12_BLEND_INV_SRC_ALPHA;
            blend.BlendOp = D3D12_BLEND_OP_ADD;
            blend.SrcBlendAlpha = D3D12_BLEND_ONE;
            blend.DestBlendAlpha = variant == 2 ? D3D12_BLEND_ONE : D3D12_BLEND_INV_SRC_ALPHA;
            blend.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            blend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            return desc;
        }

        std::string load_shader(const char *name)
        {
            const std::filesystem::path relative = std::filesystem::path("d3d") / name;
            std::ifstream file(find_shader_root(relative) / relative);
            return file ? std::string(std::istreambuf_iterator<char>(file), {}) : std::string{};
        }

        bool compile_shader(const std::string &source, const char *entry, const char *target,
                            ID3DBlob **blob, std::string &error, UINT flags = 0)
        {
            ID3DBlob *messages = nullptr;
            const HRESULT result = D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr,
                                              entry, target, D3DCOMPILE_ENABLE_STRICTNESS | flags, 0, blob, &messages);
            if (FAILED(result))
            {
                error = messages ? static_cast<const char *>(messages->GetBufferPointer())
                                 : "Unable to compile the D3D12 built-in shader.";
                if (messages)
                    messages->Release();
                return false;
            }
            if (messages)
                messages->Release();
            return true;
        }

#ifdef SIMLIB_HAVE_DXC
        // Shader model 6 compiler (DXC), loaded at runtime so the backend still works with fxc when it is absent.
        class DxcCompiler
        {
        public:
            static DxcCompiler &instance()
            {
                static DxcCompiler compiler;
                return compiler;
            }

            bool available(ID3D12Device *device)
            {
                if (!tried_)
                {
                    tried_ = true;
                    load(device);
                }
                return compiler_ != nullptr;
            }

            const wchar_t *model_suffix() const { return model_suffix_; }

            bool compile(const std::string &source, const char *stage, ID3DBlob **blob, std::string &error)
            {
                std::wstring target;
                for (const char *c = stage; *c; ++c)
                    target.push_back(static_cast<wchar_t>(*c));
                target += L"_";
                target += model_suffix_;
                const wchar_t *arguments[] = {L"-E", L"main", L"-T", target.c_str()};
                DxcBuffer buffer{source.data(), source.size(), DXC_CP_UTF8};
                IDxcResult *result = nullptr;
                if (FAILED(compiler_->Compile(&buffer, arguments, 4, nullptr, IID_PPV_ARGS(&result))) || !result)
                {
                    error = "DXC failed to start compiling the D3D12 shader.";
                    return false;
                }
                HRESULT status = E_FAIL;
                result->GetStatus(&status);
                if (FAILED(status))
                {
                    IDxcBlobUtf8 *messages = nullptr;
                    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&messages), nullptr);
                    error = messages && messages->GetStringLength() > 0 ? std::string(messages->GetStringPointer())
                                                                        : "DXC failed to compile the D3D12 shader.";
                    if (messages)
                        messages->Release();
                    result->Release();
                    return false;
                }
                IDxcBlob *object = nullptr;
                result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&object), nullptr);
                bool ok = false;
                if (object && SUCCEEDED(D3DCreateBlob(object->GetBufferSize(), blob)))
                {
                    std::memcpy((*blob)->GetBufferPointer(), object->GetBufferPointer(), object->GetBufferSize());
                    ok = true;
                }
                else
                    error = "DXC produced no shader bytecode.";
                if (object)
                    object->Release();
                result->Release();
                return ok;
            }

        private:
            void load(ID3D12Device *device)
            {
                // Shader model 6.0 is enough for the translated shaders and the most widely supported DXIL model.
                // Higher models gain nothing here, and shaders compiled for 6.6 are rejected with E_INVALIDARG
                // when creating pipeline states on some drivers and runtimes.
                struct Model
                {
                    D3D_SHADER_MODEL model;
                    const wchar_t *suffix;
                };
                const Model models[] = {{D3D_SHADER_MODEL_6_0, L"6_0"}};
                for (const Model &candidate : models)
                {
                    D3D12_FEATURE_DATA_SHADER_MODEL support{candidate.model};
                    if (device && SUCCEEDED(device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &support,
                                                                       sizeof(support))))
                    {
                        model_suffix_ = candidate.suffix;
                        break;
                    }
                }
                if (!model_suffix_)
                    return;

                HMODULE module = LoadLibraryW(L"dxcompiler.dll");
#ifdef SIMLIB_DXC_DIR
                // Load by full path so that dxil.dll (needed to sign DXIL) is found beside it.
                if (!module)
                    module = LoadLibraryExW(L"" SIMLIB_DXC_DIR L"\\dxcompiler.dll", nullptr,
                                            LOAD_WITH_ALTERED_SEARCH_PATH);
#endif
                if (!module)
                    return;
                const auto create = reinterpret_cast<DxcCreateInstanceProc>(GetProcAddress(module, "DxcCreateInstance"));
                if (!create || FAILED(create(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler_))))
                    compiler_ = nullptr;
            }

            bool tried_ = false;
            const wchar_t *model_suffix_ = nullptr;
            IDxcCompiler3 *compiler_ = nullptr;
        };

        bool dxc_available(ID3D12Device *device) { return DxcCompiler::instance().available(device); }

        bool dxc_compile(const std::string &source, const char *stage, ID3DBlob **blob, std::string &error)
        {
            return DxcCompiler::instance().compile(source, stage, blob, error);
        }
#else
        bool dxc_available(ID3D12Device *) { return false; }

        bool dxc_compile(const std::string &, const char *, ID3DBlob **, std::string &) { return false; }
#endif

        // Set SIMLIB_D3D12_TRACE=1 to log shader compilation and pipeline creation to stderr.
        void trace(const char *format, ...)
        {
            static const bool enabled = std::getenv("SIMLIB_D3D12_TRACE") != nullptr;
            if (!enabled)
                return;
            char text[512];
            va_list args;
            va_start(args, format);
            std::vsnprintf(text, sizeof(text), format, args);
            va_end(args);
            std::fprintf(stderr, "[d3d12] %s\n", text);
            std::fflush(stderr);
        }

        // With SIMLIB_D3D12_TRACE set, an unhandled crash reports its code and the faulting module and offset of each
        // stack frame, so a bare "exit code 0xC0000005" can be traced back to a place in the code.
        LONG WINAPI trace_crash_filter(EXCEPTION_POINTERS *info)
        {
            std::fprintf(stderr, "[d3d12] CRASH code 0x%08lX\n", static_cast<unsigned long>(info->ExceptionRecord->ExceptionCode));
            if (info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && info->ExceptionRecord->NumberParameters >= 2)
                std::fprintf(stderr, "[d3d12] access violation %s address 0x%p\n",
                             info->ExceptionRecord->ExceptionInformation[0] ? "writing" : "reading",
                             reinterpret_cast<void *>(info->ExceptionRecord->ExceptionInformation[1]));
            const auto describe = [](void *address, const char *prefix)
            {
                HMODULE module = nullptr;
                char name[MAX_PATH] = "?";
                if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                       static_cast<LPCSTR>(address), &module))
                    GetModuleFileNameA(module, name, sizeof(name));
                const char *base = std::strrchr(name, '\\');
                std::fprintf(stderr, "[d3d12]   %s %s+0x%llX\n", prefix, base ? base + 1 : name,
                             static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(address) -
                                                              reinterpret_cast<std::uintptr_t>(module)));
            };
            describe(info->ExceptionRecord->ExceptionAddress, "at");
            // The faulting thread's own stack, scanned for return addresses into this program and the driver: the
            // exception dispatcher's frames hide the code that made the failing call.
#if defined(_M_X64) || defined(__x86_64__)
            const auto *stack = reinterpret_cast<const std::uintptr_t *>(info->ContextRecord->Rsp);
            const HMODULE self = GetModuleHandleA(nullptr);
            int shown = 0;
            for (int i = 0; i < 4096 && shown < 40; ++i)
            {
                MEMORY_BASIC_INFORMATION memory{};
                if (!VirtualQuery(stack + i, &memory, sizeof(memory)) || memory.State != MEM_COMMIT ||
                    (memory.Protect & (PAGE_NOACCESS | PAGE_GUARD)))
                    break;
                void *value = reinterpret_cast<void *>(stack[i]);
                HMODULE module = nullptr;
                if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                        static_cast<LPCSTR>(value), &module))
                    continue;
                char name[MAX_PATH] = "";
                GetModuleFileNameA(module, name, sizeof(name));
                if (module == self || std::strstr(name, "nvwgf2") || std::strstr(name, "amdxc") || std::strstr(name, "ig"))
                {
                    describe(value, "stack");
                    ++shown;
                }
            }
#endif
            std::fflush(stderr);
            return EXCEPTION_CONTINUE_SEARCH;
        }

        // Compiles translated HLSL with DXC (shader model 6) when available, otherwise fxc (shader model 5.1).
        bool compile_program_shader(ID3D12Device *device, const std::string &source, const char *stage,
                                    ID3DBlob **blob, std::string &error)
        {
            const bool dxc = dxc_available(device);
            trace("compile %s with %s", stage, dxc ? "DXC (sm 6_0)" : "FXC (sm 5_1)");
            const bool ok = dxc ? dxc_compile(source, stage, blob, error)
                                : compile_shader(source, "main", (std::string(stage) + "_5_1").c_str(), blob, error);
            trace("compile %s %s%s%s", stage, ok ? "ok" : "FAILED: ", ok ? "" : error.c_str(), "");
            return ok;
        }

        // Drivers sometimes reject DXC-built DXIL at pipeline creation; fxc's shader model 5.1 is the safe fallback.
        bool compile_program_shader_fxc(const std::string &source, const char *stage, ID3DBlob **blob,
                                        std::string &error)
        {
            return compile_shader(source, "main", (std::string(stage) + "_5_1").c_str(), blob, error);
        }

        D3D12_HEAP_PROPERTIES heap_properties(D3D12_HEAP_TYPE type)
        {
            D3D12_HEAP_PROPERTIES properties{};
            properties.Type = type;
            properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            properties.CreationNodeMask = 1;
            properties.VisibleNodeMask = 1;
            return properties;
        }

        D3D12_RESOURCE_DESC buffer_description(std::uint64_t size)
        {
            D3D12_RESOURCE_DESC description{};
            description.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            description.Width = size;
            description.Height = 1;
            description.DepthOrArraySize = 1;
            description.MipLevels = 1;
            description.SampleDesc.Count = 1;
            description.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            return description;
        }

        void transition_resource(ID3D12GraphicsCommandList *commands, ID3D12Resource *resource,
                                 D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
        {
            D3D12_RESOURCE_BARRIER barrier{};
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Transition.pResource = resource;
            barrier.Transition.StateBefore = before;
            barrier.Transition.StateAfter = after;
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            commands->ResourceBarrier(1, &barrier);
        }

        class D3D12Renderer final : public Renderer
        {
        public:
            bool initialise(SDL_Window *window, std::string &error) override
            {
                window_ = window;
                if (std::getenv("SIMLIB_D3D12_TRACE"))
                    SetUnhandledExceptionFilter(trace_crash_filter);
                return context_.initialise(window, error);
            }

#ifdef SIMLIB_GUI_IMGUI
            bool gui_backend_init() override
            {
                if (imgui_ready_)
                    return true;
                D3D12_DESCRIPTOR_HEAP_DESC heap_desc{};
                heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                heap_desc.NumDescriptors = imgui_descriptor_count;
                heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
                if (FAILED(context_.device()->CreateDescriptorHeap(&heap_desc, IID_PPV_ARGS(&imgui_heap_))))
                    return false;
                imgui_descriptor_size_ =
                    context_.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                imgui_free_.clear();
                for (UINT index = imgui_descriptor_count; index > 0; --index)
                    imgui_free_.push_back(index - 1);

                ImGui_ImplDX12_InitInfo info;
                info.Device = context_.device();
                info.CommandQueue = context_.command_queue();
                info.NumFramesInFlight = 1;
                info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
                info.DSVFormat = DXGI_FORMAT_UNKNOWN;
                info.UserData = this;
                info.SrvDescriptorHeap = imgui_heap_;
                info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo *init, D3D12_CPU_DESCRIPTOR_HANDLE *cpu,
                                               D3D12_GPU_DESCRIPTOR_HANDLE *gpu)
                {
                    auto *self = static_cast<D3D12Renderer *>(init->UserData);
                    const UINT index = self->imgui_free_.back();
                    self->imgui_free_.pop_back();
                    *cpu = self->imgui_heap_->GetCPUDescriptorHandleForHeapStart();
                    *gpu = self->imgui_heap_->GetGPUDescriptorHandleForHeapStart();
                    cpu->ptr += static_cast<SIZE_T>(index) * self->imgui_descriptor_size_;
                    gpu->ptr += static_cast<UINT64>(index) * self->imgui_descriptor_size_;
                };
                info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo *init, D3D12_CPU_DESCRIPTOR_HANDLE cpu,
                                              D3D12_GPU_DESCRIPTOR_HANDLE)
                {
                    auto *self = static_cast<D3D12Renderer *>(init->UserData);
                    const SIZE_T start = self->imgui_heap_->GetCPUDescriptorHandleForHeapStart().ptr;
                    self->imgui_free_.push_back(static_cast<UINT>((cpu.ptr - start) / self->imgui_descriptor_size_));
                };
                if (!ImGui_ImplDX12_Init(&info))
                {
                    imgui_heap_->Release();
                    imgui_heap_ = nullptr;
                    return false;
                }
                imgui_ready_ = true;
                return true;
            }

            void gui_backend_new_frame() override
            {
                if (imgui_ready_)
                    ImGui_ImplDX12_NewFrame();
            }

            void gui_backend_render() override
            {
                if (!imgui_ready_ || !ensure_frame())
                    return;
                flush_2d();
                auto *commands = context_.command_list();
                ID3D12DescriptorHeap *heaps[] = {imgui_heap_};
                commands->SetDescriptorHeaps(1, heaps);
                ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commands);
            }

            void gui_backend_shutdown() override
            {
                if (!imgui_ready_)
                    return;
                context_.wait_idle();
                ImGui_ImplDX12_Shutdown();
                imgui_ready_ = false;
                if (imgui_heap_)
                    imgui_heap_->Release();
                imgui_heap_ = nullptr;
            }
#endif

            void shutdown() override
            {
                gui_backend_shutdown();
                if (context_.frame_open())
                {
                    flush_2d();
                    context_.present(vsync_enabled_);
                }
                else
                {
                    context_.wait_idle();
                }
                release_record_workers();
                shutdown_2d();
                for (auto &[handle, mesh] : meshes_)
                    release_mesh(mesh);
                meshes_.clear();
                for (auto &[handle, instances] : instance_buffers_)
                    if (instances.resource)
                        instances.resource->Release();
                instance_buffers_.clear();
                for (ID3D12PipelineState *&pipeline : mesh_pipelines_)
                {
                    if (pipeline)
                        pipeline->Release();
                    pipeline = nullptr;
                }
                for (ID3D12PipelineState *&pipeline : mesh_instanced_pipelines_)
                {
                    if (pipeline)
                        pipeline->Release();
                    pipeline = nullptr;
                }
                if (mesh_root_signature_)
                    mesh_root_signature_->Release();
                mesh_root_signature_ = nullptr;
                for (ID3D12PipelineState *&pipeline : sky_pipelines_)
                {
                    if (pipeline)
                        pipeline->Release();
                    pipeline = nullptr;
                }
                if (sky_root_signature_)
                    sky_root_signature_->Release();
                sky_root_signature_ = nullptr;
                if (sky_vertex_blob_)
                    sky_vertex_blob_->Release();
                if (sky_pixel_blob_)
                    sky_pixel_blob_->Release();
                sky_vertex_blob_ = nullptr;
                sky_pixel_blob_ = nullptr;
                if (mesh_vertex_blob_)
                    mesh_vertex_blob_->Release();
                if (mesh_pixel_blob_)
                    mesh_pixel_blob_->Release();
                if (mesh_instanced_vertex_blob_)
                    mesh_instanced_vertex_blob_->Release();
                mesh_vertex_blob_ = nullptr;
                mesh_pixel_blob_ = nullptr;
                mesh_instanced_vertex_blob_ = nullptr;
                for (auto &[handle, texture] : textures_)
                    release_texture(texture);
                textures_.clear();
                for (auto &[handle, target] : render_targets_)
                {
                    if (target.depth)
                        target.depth->Release();
                    if (target.msaa_colour)
                        target.msaa_colour->Release();
                }
                for (auto &texture : retired_textures_)
                    release_texture(texture);
                retired_textures_.clear();
                release_retired_textures();
                for (auto &[handle, program] : programs_)
                    release_program(program);
                programs_.clear();
                for (auto &[handle, buffer] : storage_buffers_)
                {
                    if (buffer.resource)
                        buffer.resource->Release();
                }
                storage_buffers_.clear();
                if (constant_buffer_)
                {
                    constant_buffer_->Unmap(0, nullptr);
                    constant_buffer_->Release();
                }
                constant_buffer_ = nullptr;
                mapped_constants_ = nullptr;
                if (sampler_heap_)
                    sampler_heap_->Release();
                if (srv_heap_)
                    srv_heap_->Release();
                if (sampler_staging_heap_)
                    sampler_staging_heap_->Release();
                if (srv_staging_heap_)
                    srv_staging_heap_->Release();
                sampler_staging_heap_ = nullptr;
                srv_staging_heap_ = nullptr;
                if (render_target_heap_)
                    render_target_heap_->Release();
                if (depth_stencil_heap_)
                    depth_stencil_heap_->Release();
                sampler_heap_ = nullptr;
                sampler_slots_.clear();
                srv_heap_ = nullptr;
                render_target_heap_ = nullptr;
                depth_stencil_heap_ = nullptr;
                render_targets_.clear();
                context_.shutdown();
                window_ = nullptr;
            }

            bool resize(int width, int height, std::string &error) override
            {
                flush_2d();
                return context_.resize(width, height, error);
            }

            bool set_vsync(bool enabled) override
            {
                vsync_enabled_ = enabled;
                return true;
            }
            bool vsync_active() const override { return vsync_enabled_; }
            void present() override
            {
                flush_2d();
                static int traced_presents = 0;
                const bool log_present = traced_presents < 3;
                if (log_present)
                    trace("present %d: suspend render target", traced_presents);
                suspend_active_render_target();
                if (log_present)
                    trace("present %d: submit and flip", traced_presents);
                context_.present(vsync_enabled_);
                if (log_present)
                    trace("present %d: done", traced_presents++);
                release_retired_textures();
                if (shutdown_2d_pending_)
                    release_2d_resources();
            }
            void wait_idle() override
            {
                if (context_.frame_open())
                    present();
                else
                    context_.wait_idle();
            }
            bool begin_frame(std::string &error) override
            {
                if (context_.frame_open() || context_.begin_frame())
                {
                    inline_mesh_run_ = 0;
                    return true;
                }
                error = "Unable to begin the D3D12 frame.";
                return false;
            }
            bool end_frame(std::string &error) override
            {
                if (!context_.frame_open())
                    return true;
                present();
                return true;
            }
            SDL_GLContext native_context() const override { return nullptr; }

            bool create_texture(const TextureDesc &description, std::uint32_t &texture) override
            {
                return create_texture_impl(description, DXGI_FORMAT_R8G8B8A8_UNORM,
                                           description.mipmaps ? mip_level_count(description.width, description.height) : 1,
                                           true, texture);
            }

            bool create_texture_impl(const TextureDesc &description, DXGI_FORMAT format, int mip_levels, bool allow_uav,
                                     std::uint32_t &texture)
            {
                texture = 0;
                if (description.width <= 0 || description.height <= 0 ||
                    !std::isfinite(description.anisotropy) ||
                    !context_.device() || !ensure_descriptor_heaps())
                    return false;

                const std::uint32_t descriptor = acquire_descriptor();
                if (descriptor == descriptor_capacity)
                    return false;

                D3D12_RESOURCE_DESC resource_desc{};
                resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                resource_desc.Width = static_cast<UINT64>(description.width);
                resource_desc.Height = static_cast<UINT>(description.height);
                resource_desc.DepthOrArraySize = 1;
                resource_desc.MipLevels = static_cast<UINT16>(mip_levels);
                resource_desc.Format = format;
                resource_desc.SampleDesc.Count = 1;
                resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
                resource_desc.Flags = (description.render_target ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
                                                                  : D3D12_RESOURCE_FLAG_NONE) |
                                      (allow_uav ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : D3D12_RESOURCE_FLAG_NONE);
                Texture resource;
                resource.descriptor = descriptor;
                const D3D12_HEAP_PROPERTIES default_heap = heap_properties(D3D12_HEAP_TYPE_DEFAULT);
                const HRESULT created = context_.device()->CreateCommittedResource(
                    &default_heap, D3D12_HEAP_FLAG_NONE,
                    &resource_desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
                    IID_PPV_ARGS(&resource.resource));
                if (FAILED(created))
                {
                    free_descriptors_.push_back(descriptor);
                    return false;
                }

                D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
                srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                srv_desc.Format = format;
                srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srv_desc.Texture2D.MipLevels = resource_desc.MipLevels;
                // Descriptors are authored in a CPU-only staging heap and mirrored into the shader-visible heap: copying
                // out of a shader-visible heap is not allowed and crashes NVIDIA's driver.
                context_.device()->CreateShaderResourceView(
                    resource.resource, &srv_desc, cpu_descriptor(srv_staging_heap_, srv_descriptor_size_, descriptor));
                context_.device()->CopyDescriptorsSimple(
                    1, cpu_descriptor(srv_heap_, srv_descriptor_size_, descriptor),
                    cpu_descriptor(srv_staging_heap_, srv_descriptor_size_, descriptor),
                    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

                const bool use_anisotropy = description.anisotropy > 1.0f && description.filter == TextureFilter::linear;
                const UINT max_anisotropy = use_anisotropy ? static_cast<UINT>(std::clamp(description.anisotropy, 1.0f, 16.0f)) : 1u;
                const std::uint32_t sampler_key = (use_anisotropy ? max_anisotropy : 0u) * 2u +
                                                  (description.filter == TextureFilter::linear ? 1u : 0u);
                const auto known = sampler_slots_.find(sampler_key);
                if (known != sampler_slots_.end())
                    resource.sampler = known->second;
                else if (sampler_slots_.size() < persistent_sampler_capacity)
                {
                    D3D12_SAMPLER_DESC sampler_desc{};
                    sampler_desc.Filter = use_anisotropy
                                              ? D3D12_FILTER_ANISOTROPIC
                                              : description.filter == TextureFilter::linear
                                                    ? D3D12_FILTER_MIN_MAG_MIP_LINEAR
                                                    : D3D12_FILTER_MIN_MAG_MIP_POINT;
                    sampler_desc.MaxAnisotropy = max_anisotropy;
                    sampler_desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                    sampler_desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                    sampler_desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                    sampler_desc.MaxLOD = D3D12_FLOAT32_MAX;
                    resource.sampler = static_cast<std::uint32_t>(sampler_slots_.size());
                    context_.device()->CreateSampler(
                        &sampler_desc, cpu_descriptor(sampler_staging_heap_, sampler_descriptor_size_, resource.sampler));
                    context_.device()->CopyDescriptorsSimple(
                        1, cpu_descriptor(sampler_heap_, sampler_descriptor_size_, resource.sampler),
                        cpu_descriptor(sampler_staging_heap_, sampler_descriptor_size_, resource.sampler),
                        D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
                    sampler_slots_.emplace(sampler_key, resource.sampler);
                }

                texture = next_texture_++;
                if (texture == 0)
                    texture = next_texture_++;
                textures_.emplace(texture, resource);
                return true;
            }

            bool upload_texture(std::uint32_t texture, int width, int height,
                                const std::uint8_t *pixels) override
            {
                const auto found = textures_.find(texture);
                if (found == textures_.end() || !pixels || width <= 0 || height <= 0 ||
                    !context_.device() || !context_.command_queue())
                    return false;

                if (found->second.state == D3D12_RESOURCE_STATE_RENDER_TARGET)
                    return false;
                if (context_.frame_open())
                {
                    // Submit what has been recorded so far without presenting, so earlier draws
                    // in this frame are not lost, then restore the target state on the fresh list.
                    flush_2d();
                    if (!context_.execute_with(nullptr, 0))
                        return false;
                    rebind_frame_targets();
                }
                if (!context_.wait_idle())
                    return false;

                D3D12_RESOURCE_DESC destination_desc = found->second.resource->GetDesc();
                if (destination_desc.Width != static_cast<UINT64>(width) ||
                    destination_desc.Height != static_cast<UINT>(height))
                    return false;

                // Mip levels 1..N-1 are rebuilt on the CPU from the uploaded base level.
                const UINT level_count = destination_desc.MipLevels;
                std::vector<std::uint8_t> mip_chain;
                std::vector<const std::uint8_t *> level_pixels{pixels};
                std::vector<std::size_t> level_row_bytes{static_cast<std::size_t>(width) * 4};
                if (level_count > 1)
                {
                    mip_chain = build_mip_chain(pixels, width, height);
                    std::size_t chain_offset = 0;
                    for (UINT level = 1; level < level_count; ++level)
                    {
                        const std::size_t level_width = static_cast<std::size_t>                        ((std::max)(width >> level, 1));
                                                const std::size_t level_height = static_cast<std::size_t>((std::max)(height >> level, 1));
                        level_pixels.push_back(mip_chain.data() + chain_offset);
                        level_row_bytes.push_back(level_width * 4);
                        chain_offset += level_width * level_height * 4;
                    }
                }

                return upload_subresources(found->second, level_pixels, level_row_bytes);
            }

            bool upload_subresources(Texture &target, const std::vector<const std::uint8_t *> &level_pixels,
                                     const std::vector<std::size_t> &level_row_bytes)
            {
                D3D12_RESOURCE_DESC destination_desc = target.resource->GetDesc();
                const UINT level_count = destination_desc.MipLevels;
                std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> footprints(level_count);
                std::vector<UINT> row_counts(level_count);
                std::vector<UINT64> unpadded_row_sizes(level_count);
                UINT64 upload_size = 0;
                context_.device()->GetCopyableFootprints(&destination_desc, 0, level_count, 0,
                                                         footprints.data(), row_counts.data(),
                                                         unpadded_row_sizes.data(), &upload_size);
                ID3D12Resource *upload = nullptr;
                const D3D12_RESOURCE_DESC upload_desc = buffer_description(upload_size);
                const D3D12_HEAP_PROPERTIES upload_heap = heap_properties(D3D12_HEAP_TYPE_UPLOAD);
                HRESULT result = context_.device()->CreateCommittedResource(
                    &upload_heap, D3D12_HEAP_FLAG_NONE,
                    &upload_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                    IID_PPV_ARGS(&upload));
                if (FAILED(result))
                    return false;

                std::uint8_t *mapped = nullptr;
                D3D12_RANGE read_range{0, 0};
                result = upload->Map(0, &read_range, reinterpret_cast<void **>(&mapped));
                if (FAILED(result))
                {
                    upload->Release();
                    return false;
                }
                for (UINT level = 0; level < level_count; ++level)
                {
                    const auto &footprint = footprints[level];
                    for (UINT row = 0; row < row_counts[level]; ++row)
                    {
                        std::memcpy(mapped + footprint.Offset + static_cast<std::size_t>(row) * footprint.Footprint.RowPitch,
                                    level_pixels[level] + static_cast<std::size_t>(row) * level_row_bytes[level],
                                    static_cast<std::size_t>(unpadded_row_sizes[level]));
                    }
                }
                upload->Unmap(0, nullptr);

                ID3D12CommandAllocator *allocator = nullptr;
                ID3D12GraphicsCommandList *commands = nullptr;
                result = context_.device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                   IID_PPV_ARGS(&allocator));
                if (SUCCEEDED(result))
                    result = context_.device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                  allocator, nullptr, IID_PPV_ARGS(&commands));
                if (FAILED(result))
                {
                    if (commands)
                        commands->Release();
                    if (allocator)
                        allocator->Release();
                    upload->Release();
                    return false;
                }

                const D3D12_RESOURCE_STATES previous_state = target.state;
                if (previous_state != D3D12_RESOURCE_STATE_COPY_DEST)
                    transition_resource(commands, target.resource, previous_state,
                                        D3D12_RESOURCE_STATE_COPY_DEST);
                for (UINT level = 0; level < level_count; ++level)
                {
                    D3D12_TEXTURE_COPY_LOCATION source_location{};
                    source_location.pResource = upload;
                    source_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                    source_location.PlacedFootprint = footprints[level];
                    D3D12_TEXTURE_COPY_LOCATION destination_location{};
                    destination_location.pResource = target.resource;
                    destination_location.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                    destination_location.SubresourceIndex = level;
                    commands->CopyTextureRegion(&destination_location, 0, 0, 0, &source_location, nullptr);
                }
                transition_resource(commands, target.resource,
                                    D3D12_RESOURCE_STATE_COPY_DEST,
                                    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                result = commands->Close();
                if (SUCCEEDED(result))
                {
                    ID3D12CommandList *lists[] = {commands};
                    context_.command_queue()->ExecuteCommandLists(1, lists);
                    if (!context_.wait_idle())
                        result = E_FAIL;
                }
                commands->Release();
                allocator->Release();
                upload->Release();
                if (FAILED(result))
                    return false;
                target.state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                return true;
            }

            static DXGI_FORMAT compressed_dxgi_format(CompressedFormat format)
            {
                switch (format)
                {
                case CompressedFormat::bc1:
                    return DXGI_FORMAT_BC1_UNORM;
                case CompressedFormat::bc3:
                    return DXGI_FORMAT_BC3_UNORM;
                default:
                    return DXGI_FORMAT_BC7_UNORM;
                }
            }

            bool supports_compressed_format(CompressedFormat) const override { return context_.device() != nullptr; }

            bool create_texture_compressed(const CompressedTextureDesc &description, const std::uint8_t *data,
                                           std::uint32_t &texture) override
            {
                texture = 0;
                if (!data || compressed_texture_byte_count(description) == 0)
                    return false;
                TextureDesc base;
                base.width = description.width;
                base.height = description.height;
                base.filter = description.filter;
                base.anisotropy = description.anisotropy;
                std::uint32_t handle = 0;
                if (!create_texture_impl(base, compressed_dxgi_format(description.format), description.mip_levels, false, handle))
                    return false;
                const auto found = textures_.find(handle);
                if (found == textures_.end())
                    return false;
                if (context_.frame_open())
                {
                    flush_2d();
                    if (!context_.execute_with(nullptr, 0))
                    {
                        destroy_texture(handle);
                        return false;
                    }
                    rebind_frame_targets();
                }
                if (!context_.wait_idle())
                {
                    destroy_texture(handle);
                    return false;
                }
                std::vector<const std::uint8_t *> level_pixels;
                std::vector<std::size_t> level_row_bytes;
                std::size_t offset = 0;
                for (int level = 0; level < description.mip_levels; ++level)
                {
                    const int width = (std::max)(description.width >> level, 1);
                    const int height = (std::max)(description.height >> level, 1);
                    level_pixels.push_back(data + offset);
                    level_row_bytes.push_back(compressed_level_byte_count(description.format, width, 4) );
                    offset += compressed_level_byte_count(description.format, width, height);
                }
                if (!upload_subresources(found->second, level_pixels, level_row_bytes))
                {
                    destroy_texture(handle);
                    return false;
                }
                texture = handle;
                return true;
            }

            bool download_texture(std::uint32_t texture, int width, int height,
                                  std::uint8_t *pixels) override
            {
                const auto found = textures_.find(texture);
                return active_render_target_ == 0 && found != textures_.end() &&
                       readback_resource(found->second.resource, found->second.state, width, height, pixels);
            }

            void destroy_texture(std::uint32_t handle) override
            {
                const auto found = textures_.find(handle);
                if (found == textures_.end())
                    return;
                flush_2d();
                if (context_.frame_open())
                {
                    retired_textures_.push_back(found->second);
                }
                else
                {
                    context_.wait_idle();
                    release_texture(found->second);
                }
                textures_.erase(found);
            }

            bool create_render_target(int width, int height, std::uint32_t &texture,
                                      std::uint32_t &framebuffer) override
            {
                texture = 0;
                framebuffer = 0;
                if (width <= 0 || height <= 0 || !ensure_render_target_heap())
                    return false;
                const std::uint32_t descriptor = acquire_render_target_descriptor();
                if (descriptor == render_target_capacity)
                    return false;
                if (!create_texture({width, height, TextureFilter::linear, true}, texture))
                {
                    free_render_target_descriptors_.push_back(descriptor);
                    return false;
                }
                const auto resource = textures_.find(texture);
                if (resource == textures_.end())
                {
                    destroy_texture(texture);
                    free_render_target_descriptors_.push_back(descriptor);
                    texture = 0;
                    return false;
                }
                context_.device()->CreateRenderTargetView(
                    resource->second.resource, nullptr,
                    cpu_descriptor(render_target_heap_, render_target_descriptor_size_, descriptor));
                framebuffer = next_render_target_++;
                if (framebuffer == 0)
                    framebuffer = next_render_target_++;
                RenderTarget target;
                target.texture = texture;
                target.descriptor = descriptor;
                target.width = width;
                target.height = height;
                render_targets_.emplace(framebuffer, target);
                return true;
            }
            void destroy_render_target(std::uint32_t texture, std::uint32_t framebuffer) override
            {
                const auto target = render_targets_.find(framebuffer);
                if (target == render_targets_.end())
                    return;
                if (active_render_target_ == framebuffer)
                {
                    std::string error;
                    end_render_target(error);
                }
                flush_2d();
                if (context_.frame_open())
                {
                    context_.present(vsync_enabled_);
                    release_retired_textures();
                }
                if (!context_.wait_idle())
                    return;
                free_render_target_descriptors_.push_back(target->second.descriptor);
                if (target->second.depth)
                {
                    target->second.depth->Release();
                    free_depth_stencil_descriptors_.push_back(target->second.depth_descriptor);
                }
                if (target->second.msaa_colour)
                    target->second.msaa_colour->Release();
                render_targets_.erase(target);
                destroy_texture(texture);
            }
            template <typename Target>
            void bind_render_target_state(const Target &target, int width, int height,
                                          ID3D12GraphicsCommandList *commands = nullptr)
            {
                if (!commands)
                    commands = context_.command_list();
                D3D12_CPU_DESCRIPTOR_HANDLE handle =
                    cpu_descriptor(render_target_heap_, render_target_descriptor_size_,
                                   target.descriptor);
                D3D12_CPU_DESCRIPTOR_HANDLE depth_handle{};
                const bool has_depth = target.depth != nullptr;
                if (has_depth)
                    depth_handle = cpu_descriptor(depth_stencil_heap_, depth_stencil_descriptor_size_,
                                                  target.depth_descriptor);
                commands->OMSetRenderTargets(1, &handle, FALSE, has_depth ? &depth_handle : nullptr);
                D3D12_VIEWPORT viewport{0.0f, 0.0f, static_cast<float>(width),
                                        static_cast<float>(height), 0.0f, 1.0f};
                D3D12_RECT scissor{0, 0, width, height};
                commands->RSSetViewports(1, &viewport);
                commands->RSSetScissorRects(1, &scissor);
            }
            bool begin_render_target(std::uint32_t framebuffer, int width, int height,
                                     std::string &error) override
            {
                const auto target = render_targets_.find(framebuffer);
                const auto texture = target == render_targets_.end() ? textures_.end()
                                                                      : textures_.find(target->second.texture);
                if (target == render_targets_.end() || texture == textures_.end() ||
                    target->second.width != width || target->second.height != height ||
                    !ensure_frame())
                {
                    error = "Invalid D3D12 render target.";
                    return false;
                }
                flush_2d();
                context_.flush_pending();
                if (target->second.samples == 1 && texture->second.state != D3D12_RESOURCE_STATE_RENDER_TARGET)
                {
                    transition_resource(context_.command_list(), texture->second.resource,
                                        texture->second.state, D3D12_RESOURCE_STATE_RENDER_TARGET);
                    texture->second.state = D3D12_RESOURCE_STATE_RENDER_TARGET;
                }
                if (active_render_target_ != 0)
                    render_target_stack_.push_back(active_render_target_);
                bind_render_target_state(target->second, width, height);
                active_render_target_ = framebuffer;
                return true;
            }
            bool end_render_target(std::string &error) override
            {
                if (active_render_target_ == 0)
                {
                    error = "No D3D12 render target is active.";
                    return false;
                }
                if (!ensure_frame())
                {
                    error = last_error_;
                    return false;
                }
                flush_2d();
                context_.flush_pending();
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end())
                {
                    active_render_target_ = 0;
                    error = "The active D3D12 render target no longer exists.";
                    return false;
                }
                const auto texture = textures_.find(target->second.texture);
                if (texture == textures_.end())
                {
                    active_render_target_ = 0;
                    error = "The active D3D12 render-target texture no longer exists.";
                    return false;
                }
                if (target->second.samples > 1)
                    resolve_render_target(target->second, texture->second);
                else if (texture->second.state == D3D12_RESOURCE_STATE_RENDER_TARGET)
                    transition_resource(context_.command_list(), texture->second.resource,
                                        D3D12_RESOURCE_STATE_RENDER_TARGET,
                                        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                texture->second.state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                active_render_target_ = 0;
                if (!render_target_stack_.empty())
                {
                    const std::uint32_t previous = render_target_stack_.back();
                    render_target_stack_.pop_back();
                    const auto outer = render_targets_.find(previous);
                    if (outer != render_targets_.end())
                    {
                        bind_render_target_state(outer->second, outer->second.width, outer->second.height);
                        active_render_target_ = previous;
                        return true;
                    }
                }
                context_.bind_backbuffer();
                return true;
            }
            bool download_render_target(std::uint32_t framebuffer, int width, int height,
                                        std::uint8_t *pixels) override
            {
                ID3D12Resource *resource = nullptr;
                D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                if (framebuffer == 0)
                {
                    resource = context_.back_buffer();
                    state = D3D12_RESOURCE_STATE_PRESENT;
                    if (active_render_target_ != 0)
                        return false;
                }
                else
                {
                    const auto target = render_targets_.find(framebuffer);
                    if (target == render_targets_.end() || active_render_target_ == framebuffer)
                        return false;
                    const auto texture = textures_.find(target->second.texture);
                    if (texture == textures_.end() || target->second.width != width ||
                        target->second.height != height)
                        return false;
                    resource = texture->second.resource;
                    state = texture->second.state;
                }
                return readback_resource(resource, state, width, height, pixels);
            }
            void warm_up_pipelines(int samples) override
            {
                samples = supported_sample_count(samples);
                if (initialise_2d() && initialise_mesh_pipeline())
                {
                    for (std::size_t cull = 0; cull < 3; ++cull)
                        for (std::uint32_t blend = 0; blend < 3; ++blend)
                        {
                            get_mesh_pipeline(cull, samples, false, blend);
                            get_mesh_pipeline(cull, samples, true, blend);
                        }
                }
                if (initialise_2d() && initialise_sky_pipeline())
                    get_sky_pipeline(samples);
            }
            std::uint64_t pipelines_created() const override { return pipelines_created_; }
            bool supports_parallel_recording() const override { return true; }
            bool create_render_target_3d(int width, int height, int &samples,
                                         std::uint32_t &texture, std::uint32_t &framebuffer) override
            {
                samples = supported_sample_count(samples);
                if (!create_render_target(width, height, texture, framebuffer))
                    return false;
                if (!ensure_depth_stencil_heap())
                {
                    destroy_render_target(texture, framebuffer);
                    texture = 0;
                    framebuffer = 0;
                    return false;
                }
                const std::uint32_t descriptor = acquire_depth_stencil_descriptor();
                if (descriptor == render_target_capacity)
                {
                    destroy_render_target(texture, framebuffer);
                    texture = 0;
                    framebuffer = 0;
                    return false;
                }
                D3D12_RESOURCE_DESC resource_desc{};
                resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                resource_desc.Width = static_cast<UINT64>(width);
                resource_desc.Height = static_cast<UINT>(height);
                resource_desc.DepthOrArraySize = 1;
                resource_desc.MipLevels = 1;
                resource_desc.Format = DXGI_FORMAT_D32_FLOAT;
                resource_desc.SampleDesc.Count = static_cast<UINT>(samples);
                resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
                resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
                D3D12_CLEAR_VALUE clear_value{};
                clear_value.Format = DXGI_FORMAT_D32_FLOAT;
                clear_value.DepthStencil.Depth = 1.0f;
                const D3D12_HEAP_PROPERTIES default_heap = heap_properties(D3D12_HEAP_TYPE_DEFAULT);
                ID3D12Resource *depth = nullptr;
                if (FAILED(context_.device()->CreateCommittedResource(
                        &default_heap, D3D12_HEAP_FLAG_NONE, &resource_desc,
                        D3D12_RESOURCE_STATE_DEPTH_WRITE, &clear_value, IID_PPV_ARGS(&depth))))
                {
                    free_depth_stencil_descriptors_.push_back(descriptor);
                    destroy_render_target(texture, framebuffer);
                    texture = 0;
                    framebuffer = 0;
                    return false;
                }
                D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
                dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
                dsv_desc.ViewDimension = samples > 1 ? D3D12_DSV_DIMENSION_TEXTURE2DMS
                                                      : D3D12_DSV_DIMENSION_TEXTURE2D;
                context_.device()->CreateDepthStencilView(
                    depth, &dsv_desc,
                    cpu_descriptor(depth_stencil_heap_, depth_stencil_descriptor_size_, descriptor));
                const auto target = render_targets_.find(framebuffer);
                if (target == render_targets_.end())
                {
                    depth->Release();
                    free_depth_stencil_descriptors_.push_back(descriptor);
                    destroy_render_target(texture, framebuffer);
                    texture = 0;
                    framebuffer = 0;
                    return false;
                }
                if (samples > 1)
                {
                    D3D12_RESOURCE_DESC colour_desc = resource_desc;
                    colour_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                    colour_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
                    D3D12_CLEAR_VALUE colour_clear{};
                    colour_clear.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                    ID3D12Resource *msaa_colour = nullptr;
                    if (FAILED(context_.device()->CreateCommittedResource(
                            &default_heap, D3D12_HEAP_FLAG_NONE, &colour_desc,
                            D3D12_RESOURCE_STATE_RENDER_TARGET, &colour_clear, IID_PPV_ARGS(&msaa_colour))))
                    {
                        depth->Release();
                        free_depth_stencil_descriptors_.push_back(descriptor);
                        destroy_render_target(texture, framebuffer);
                        texture = 0;
                        framebuffer = 0;
                        return false;
                    }
                    // Draws go to the multisampled buffer; the texture only receives the resolved image.
                    context_.device()->CreateRenderTargetView(
                        msaa_colour, nullptr,
                        cpu_descriptor(render_target_heap_, render_target_descriptor_size_,
                                       target->second.descriptor));
                    target->second.msaa_colour = msaa_colour;
                    target->second.samples = samples;
                }
                target->second.depth = depth;
                target->second.depth_descriptor = descriptor;
                return true;
            }
            bool create_mesh(const Vertex3D *vertices, std::size_t vertex_count,
                             const std::uint32_t *indices, std::size_t index_count,
                             std::uint32_t &handle) override
            {
                handle = 0;
                if (!vertices || !indices || vertex_count == 0 || index_count == 0 ||
                    index_count > UINT_MAX ||
                    vertex_count > UINT_MAX / sizeof(Vertex3D) ||
                    index_count > UINT_MAX / sizeof(std::uint32_t) ||
                    !context_.device())
                    return false;

                Mesh mesh;
                const D3D12_HEAP_PROPERTIES upload_heap = heap_properties(D3D12_HEAP_TYPE_UPLOAD);
                const D3D12_RESOURCE_DESC vertex_desc =
                    buffer_description(sizeof(Vertex3D) * vertex_count);
                HRESULT result = context_.device()->CreateCommittedResource(
                    &upload_heap, D3D12_HEAP_FLAG_NONE, &vertex_desc,
                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mesh.vertices));
                if (FAILED(result))
                    return false;
                const D3D12_RESOURCE_DESC index_desc =
                    buffer_description(sizeof(std::uint32_t) * index_count);
                result = context_.device()->CreateCommittedResource(
                    &upload_heap, D3D12_HEAP_FLAG_NONE, &index_desc,
                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mesh.indices));
                if (FAILED(result))
                {
                    release_mesh(mesh);
                    return false;
                }
                void *mapped = nullptr;
                const D3D12_RANGE read_range{0, 0};
                result = mesh.vertices->Map(0, &read_range, &mapped);
                if (FAILED(result))
                {
                    release_mesh(mesh);
                    return false;
                }
                std::memcpy(mapped, vertices, sizeof(Vertex3D) * vertex_count);
                const D3D12_RANGE written_range{0, 0};
                mesh.vertices->Unmap(0, &written_range);
                mapped = nullptr;
                result = mesh.indices->Map(0, &read_range, &mapped);
                if (FAILED(result))
                {
                    release_mesh(mesh);
                    return false;
                }
                std::memcpy(mapped, indices, sizeof(std::uint32_t) * index_count);
                mesh.indices->Unmap(0, &written_range);
                mesh.vertex_view.BufferLocation = mesh.vertices->GetGPUVirtualAddress();
                mesh.vertex_view.SizeInBytes = static_cast<UINT>(sizeof(Vertex3D) * vertex_count);
                mesh.vertex_view.StrideInBytes = sizeof(Vertex3D);
                mesh.index_view.BufferLocation = mesh.indices->GetGPUVirtualAddress();
                mesh.index_view.SizeInBytes = static_cast<UINT>(sizeof(std::uint32_t) * index_count);
                mesh.index_view.Format = DXGI_FORMAT_R32_UINT;
                mesh.index_count = static_cast<UINT>(index_count);
                handle = next_mesh_++;
                if (handle == 0)
                    handle = next_mesh_++;
                meshes_.emplace(handle, mesh);
                return true;
            }
            bool update_mesh_vertices(std::uint32_t handle, const Vertex3D *vertices, std::size_t vertex_count) override
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end() || !vertices ||
                    sizeof(Vertex3D) * vertex_count != found->second.vertex_view.SizeInBytes)
                    return false;
                void *mapped = nullptr;
                const D3D12_RANGE read_range{0, 0};
                if (FAILED(found->second.vertices->Map(0, &read_range, &mapped)))
                    return false;
                std::memcpy(mapped, vertices, sizeof(Vertex3D) * vertex_count);
                found->second.vertices->Unmap(0, nullptr);
                return true;
            }
            void destroy_mesh(std::uint32_t handle) override
            {
                const auto found = meshes_.find(handle);
                if (found == meshes_.end())
                    return;
                flush_2d();
                if (context_.frame_open())
                    present();
                if (!context_.wait_idle())
                    return;
                release_mesh(found->second);
                meshes_.erase(found);
            }
            bool draw_mesh(std::uint32_t handle, const MeshDrawParams &params) override
            {
                return draw_mesh_common(handle, params, nullptr, 0);
            }
            bool create_instance_buffer(std::size_t capacity, std::uint32_t &handle) override
            {
                handle = 0;
                if (capacity == 0 || capacity > UINT_MAX / sizeof(InstanceGpu) || !context_.device())
                    return false;
                D3D12InstanceBuffer instances;
                const D3D12_HEAP_PROPERTIES upload_heap = heap_properties(D3D12_HEAP_TYPE_UPLOAD);
                const D3D12_RESOURCE_DESC desc = buffer_description(sizeof(InstanceGpu) * capacity);
                if (FAILED(context_.device()->CreateCommittedResource(
                        &upload_heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                        IID_PPV_ARGS(&instances.resource))))
                    return false;
                instances.capacity = capacity;
                instances.view.BufferLocation = instances.resource->GetGPUVirtualAddress();
                instances.view.SizeInBytes = static_cast<UINT>(sizeof(InstanceGpu) * capacity);
                instances.view.StrideInBytes = sizeof(InstanceGpu);
                handle = next_instance_buffer_++;
                if (handle == 0)
                    handle = next_instance_buffer_++;
                instance_buffers_.emplace(handle, instances);
                return true;
            }
            bool update_instance_buffer(std::uint32_t handle, const InstanceGpu *instances, std::size_t count) override
            {
                const auto found = instance_buffers_.find(handle);
                if (found == instance_buffers_.end() || count > found->second.capacity || (!instances && count > 0))
                    return false;
                if (count == 0)
                    return true;
                void *mapped = nullptr;
                const D3D12_RANGE read_range{0, 0};
                if (FAILED(found->second.resource->Map(0, &read_range, &mapped)))
                    return false;
                std::memcpy(mapped, instances, sizeof(InstanceGpu) * count);
                const D3D12_RANGE written_range{0, sizeof(InstanceGpu) * count};
                found->second.resource->Unmap(0, &written_range);
                return true;
            }
            void destroy_instance_buffer(std::uint32_t handle) override
            {
                const auto found = instance_buffers_.find(handle);
                if (found == instance_buffers_.end())
                    return;
                flush_2d();
                if (context_.frame_open())
                    present();
                if (!context_.wait_idle())
                    return;
                found->second.resource->Release();
                instance_buffers_.erase(found);
            }
            bool draw_mesh_instanced(std::uint32_t handle, const MeshDrawParams &params, std::uint32_t instance_buffer,
                                     std::size_t count) override
            {
                const auto instances = instance_buffers_.find(instance_buffer);
                if (instances == instance_buffers_.end() || count == 0 || count > instances->second.capacity ||
                    params.terrain_albedo != 0)
                    return false;
                return draw_mesh_common(handle, params, &instances->second, count);
            }
            bool draw_mesh_common(std::uint32_t handle, const MeshDrawParams &params,
                                  const D3D12InstanceBuffer *instances, std::size_t instance_count)
            {
                const auto mesh = meshes_.find(handle);
                // While draws are queued the target is already bound; re-binding would flush the queue every draw.
                const bool frame_ready = queued_draws_.empty() ? ensure_frame() : context_.frame_open();
                if (mesh == meshes_.end() || active_render_target_ == 0 ||
                    !initialise_2d() || !initialise_mesh_pipeline() || !frame_ready)
                    return false;
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end() || !target->second.depth)
                    return false;
                flush_2d();

                auto texture = textures_.find(params.texture != 0 ? params.texture : white_texture_);
                if (texture == textures_.end() ||
                    texture->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                    texture = textures_.find(white_texture_);
                if (texture == textures_.end())
                    return false;
                auto normal_map = textures_.find(params.normal_texture != 0 ? params.normal_texture : white_texture_);
                if (normal_map == textures_.end() ||
                    normal_map->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                    normal_map = textures_.find(white_texture_);
                if (normal_map == textures_.end())
                    return false;

                auto material_map = textures_.find(params.material_texture != 0 ? params.material_texture : white_texture_);
                if (material_map == textures_.end() ||
                    material_map->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                    material_map = textures_.find(white_texture_);
                if (material_map == textures_.end())
                    return false;
                auto shadow_map = textures_.find(params.shadow_texture != 0 ? params.shadow_texture : white_texture_);
                if (shadow_map == textures_.end() ||
                    shadow_map->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                    shadow_map = textures_.find(white_texture_);
                if (shadow_map == textures_.end())
                    return false;
                auto local_shadow_map =
                    textures_.find(params.local_shadow_texture != 0 ? params.local_shadow_texture : white_texture_);
                if (local_shadow_map == textures_.end() ||
                    local_shadow_map->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                    local_shadow_map = textures_.find(white_texture_);
                if (local_shadow_map == textures_.end())
                    return false;
                // The eye position, shadow parameters and matrix are followed by four float4s per light and then
                // the local-shadow matrices starting at local_shadow_matrix_base.
                std::array<float, 44 + local_light_vec4s * 4> pbr_constants{};
                std::copy(std::begin(params.pbr), std::end(params.pbr), pbr_constants.begin());
                std::copy(std::begin(params.emissive), std::end(params.emissive), pbr_constants.begin() + 4);
                std::copy(std::begin(params.camera_right), std::end(params.camera_right), pbr_constants.begin() + 8);
                std::copy(std::begin(params.camera_up), std::end(params.camera_up), pbr_constants.begin() + 12);
                std::copy(std::begin(params.camera_forward), std::end(params.camera_forward), pbr_constants.begin() + 16);
                pbr_constants[19] = static_cast<float>(params.style);
                std::copy(std::begin(params.camera_position), std::end(params.camera_position), pbr_constants.begin() + 20);
                const std::size_t local_lights =
                    static_cast<std::size_t>(std::clamp(params.camera_position[3], 0.0f, 16.0f));
                std::copy(std::begin(params.shadow), std::end(params.shadow), pbr_constants.begin() + 24);
                std::copy(std::begin(params.shadow_matrix), std::end(params.shadow_matrix), pbr_constants.begin() + 28);
                const std::size_t shadow_slots =
                    static_cast<std::size_t>(std::clamp(params.shadow[3], 0.0f,
                                                        static_cast<float>(local_shadow_slots)));
                const std::size_t local_vec4s =
                    (std::max)(local_lights * 4,
                               params.local_shadow_texture != 0
                                   ? static_cast<std::size_t>(local_shadow_matrix_base) + shadow_slots * 24
                                   : 0);
                std::memcpy(pbr_constants.data() + 44, params.local_lights, local_vec4s * sizeof(float) * 4);
                const D3D12_GPU_VIRTUAL_ADDRESS pbr_address =
                    allocate_constants(pbr_constants.data(), sizeof(float) * (44 + local_vec4s * 4));
                if (pbr_address == 0)
                    return false;

                std::array<float, 48> constants{};
                std::copy(std::begin(params.mvp), std::end(params.mvp), constants.begin());
                std::copy(&params.normal_rows[0][0], &params.normal_rows[0][0] + 12,
                          constants.begin() + 16);
                std::copy(std::begin(params.light), std::end(params.light), constants.begin() + 28);
                // Matches the cbuffer after `light`: light_colour, fog_colour, fog_beta, fog_view.
                std::copy(std::begin(params.light_colour), std::end(params.light_colour), constants.begin() + 32);
                std::copy(std::begin(params.fog_colour), std::end(params.fog_colour), constants.begin() + 36);
                std::copy(std::begin(params.fog_beta), std::end(params.fog_beta), constants.begin() + 40);
                std::copy(std::begin(params.fog_view), std::end(params.fog_view), constants.begin() + 44);

                ID3D12PipelineState *mesh_pipeline =
                    get_mesh_pipeline(static_cast<std::size_t>(params.cull), target->second.samples, instances != nullptr,
                                      std::min<std::uint32_t>(params.blend, 2), params.style == 2);
                if (!mesh_pipeline)
                    return false;

                QueuedMeshDraw draw;
                draw.constants = constants;
                draw.texture_srv = gpu_descriptor(srv_heap_, srv_descriptor_size_, texture->second.descriptor);
                draw.texture_sampler = gpu_descriptor(sampler_heap_, sampler_descriptor_size_, texture->second.sampler);
                draw.normal_srv = gpu_descriptor(srv_heap_, srv_descriptor_size_, normal_map->second.descriptor);
                draw.normal_sampler = gpu_descriptor(sampler_heap_, sampler_descriptor_size_, normal_map->second.sampler);
                draw.material_srv = gpu_descriptor(srv_heap_, srv_descriptor_size_, material_map->second.descriptor);
                draw.material_sampler = gpu_descriptor(sampler_heap_, sampler_descriptor_size_, material_map->second.sampler);
                draw.shadow_srv = gpu_descriptor(srv_heap_, srv_descriptor_size_, shadow_map->second.descriptor);
                draw.local_shadow_srv =
                    gpu_descriptor(srv_heap_, srv_descriptor_size_, local_shadow_map->second.descriptor);
                draw.pbr_address = pbr_address;
                draw.pipeline = mesh_pipeline;
                draw.views[0] = mesh->second.vertex_view;
                draw.views[1] = instances ? instances->view : D3D12_VERTEX_BUFFER_VIEW{};
                draw.view_count = instances ? 2 : 1;
                draw.index_view = mesh->second.index_view;
                draw.index_count = mesh->second.index_count;
                draw.instance_count = instances ? static_cast<UINT>(instance_count) : 1;

                // The first `threshold` draws record inline; later ones are queued and recorded in parallel.
                const detail::ParallelRecordSettings settings = detail::parallel_record_settings();
                if (!settings.enabled || settings.max_threads <= 1)
                    context_.flush_pending();
                else if (!queued_draws_.empty() || inline_mesh_run_ >= settings.threshold)
                {
                    if (queued_draws_.empty())
                        context_.set_flush_hook([this] { flush_queued_draws(); });
                    queued_draws_.push_back(draw);
                    return true;
                }
                ++inline_mesh_run_;
                record_mesh_draw(context_.command_list(), draw);
                return true;
            }
            struct QueuedMeshDraw
            {
                std::array<float, 48> constants;
                D3D12_GPU_DESCRIPTOR_HANDLE texture_srv, texture_sampler, normal_srv, normal_sampler,
                    material_srv, material_sampler, shadow_srv, local_shadow_srv;
                D3D12_GPU_VIRTUAL_ADDRESS pbr_address;
                ID3D12PipelineState *pipeline;
                D3D12_VERTEX_BUFFER_VIEW views[2];
                UINT view_count;
                D3D12_INDEX_BUFFER_VIEW index_view;
                UINT index_count;
                UINT instance_count;
            };
            struct RecordWorker
            {
                ID3D12CommandAllocator *allocator = nullptr;
                ID3D12GraphicsCommandList *list = nullptr;
                std::uint64_t frame_serial = 0;
            };
            // Touches no shared renderer state, so worker threads may call it on their own lists.
            void record_mesh_draw(ID3D12GraphicsCommandList *commands, const QueuedMeshDraw &draw) const
            {
                ID3D12DescriptorHeap *heaps[] = {srv_heap_, sampler_heap_};
                commands->SetDescriptorHeaps(2, heaps);
                commands->SetGraphicsRootSignature(mesh_root_signature_);
                commands->SetGraphicsRoot32BitConstants(0, static_cast<UINT>(draw.constants.size()),
                                                         draw.constants.data(), 0);
                commands->SetGraphicsRootDescriptorTable(1, draw.texture_srv);
                commands->SetGraphicsRootDescriptorTable(2, draw.texture_sampler);
                commands->SetGraphicsRootDescriptorTable(3, draw.normal_srv);
                commands->SetGraphicsRootDescriptorTable(4, draw.normal_sampler);
                commands->SetGraphicsRootDescriptorTable(5, draw.material_srv);
                commands->SetGraphicsRootDescriptorTable(6, draw.material_sampler);
                commands->SetGraphicsRootConstantBufferView(7, draw.pbr_address);
                commands->SetGraphicsRootDescriptorTable(8, draw.shadow_srv);
                commands->SetGraphicsRootDescriptorTable(9, draw.local_shadow_srv);
                commands->SetPipelineState(draw.pipeline);
                commands->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                commands->IASetVertexBuffers(0, draw.view_count, draw.views);
                commands->IASetIndexBuffer(&draw.index_view);
                commands->DrawIndexedInstanced(draw.index_count, draw.instance_count, 0, 0, 0);
            }
            bool ensure_record_workers(unsigned count, std::uint64_t serial)
            {
                if (worker_serial_ != serial)
                {
                    worker_serial_ = serial;
                    workers_used_ = 0;
                }
                while (workers_.size() < workers_used_ + count)
                {
                    RecordWorker worker;
                    if (FAILED(context_.device()->CreateCommandAllocator(
                            D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&worker.allocator))))
                        return false;
                    if (FAILED(context_.device()->CreateCommandList(
                            0, D3D12_COMMAND_LIST_TYPE_DIRECT, worker.allocator, nullptr,
                            IID_PPV_ARGS(&worker.list))))
                    {
                        worker.allocator->Release();
                        return false;
                    }
                    worker.list->Close();
                    workers_.push_back(worker);
                }
                return true;
            }
            void release_record_workers()
            {
                for (RecordWorker &worker : workers_)
                {
                    if (worker.list)
                        worker.list->Release();
                    if (worker.allocator)
                        worker.allocator->Release();
                }
                workers_.clear();
                workers_used_ = 0;
            }
            // Records the queued draws on worker lists that execute right after the primary list's earlier commands.
            void flush_queued_draws()
            {
                std::vector<QueuedMeshDraw> draws;
                draws.swap(queued_draws_);
                inline_mesh_run_ = 0;
                const auto target = render_targets_.find(active_render_target_);
                ID3D12GraphicsCommandList *primary = context_.raw_command_list();
                if (draws.empty() || target == render_targets_.end() || !primary)
                    return;
                const auto record_inline = [&]
                {
                    for (const QueuedMeshDraw &draw : draws)
                        record_mesh_draw(primary, draw);
                };
                const std::size_t count = draws.size();
                const unsigned chunks = detail::parallel_record_chunks(count);
                const std::uint64_t serial = context_.frame_serial();
                if (chunks <= 1 || !ensure_record_workers(chunks, serial))
                    return record_inline();

                const std::size_t per_chunk = (count + chunks - 1) / chunks;
                bool prepared = true;
                for (unsigned i = 0; i < chunks && prepared; ++i)
                {
                    RecordWorker &worker = workers_[workers_used_ + i];
                    // Each allocator is used once per frame; the GPU is idle at frame start.
                    if (worker.frame_serial != serial && FAILED(worker.allocator->Reset()))
                        prepared = false;
                    else if (FAILED(worker.list->Reset(worker.allocator, nullptr)))
                        prepared = false;
                    else
                        worker.frame_serial = serial;
                }
                if (!prepared)
                {
                    for (unsigned i = 0; i < chunks; ++i)
                        workers_[workers_used_ + i].list->Close();
                    return record_inline();
                }

                const int width = target->second.width;
                const int height = target->second.height;
                RecordWorker *first_worker = &workers_[workers_used_];
                workers_used_ += chunks;
                detail::parallel_record_run(chunks,
                                            [&](unsigned chunk)
                                            {
                                                ID3D12GraphicsCommandList *list = first_worker[chunk].list;
                                                bind_render_target_state(target->second, width, height, list);
                                                const std::size_t begin = (std::min)(count, chunk * per_chunk);
                                                const std::size_t end = (std::min)(count, begin + per_chunk);
                                                for (std::size_t i = begin; i < end; ++i)
                                                    record_mesh_draw(list, draws[i]);
                                                list->Close();
                                            });
                std::vector<ID3D12CommandList *> lists(chunks);
                for (unsigned i = 0; i < chunks; ++i)
                    lists[i] = first_worker[i].list;
                if (!context_.execute_with(lists.data(), chunks))
                    return;
                bind_render_target_state(target->second, width, height, context_.raw_command_list());
                detail::parallel_draws_add(count);
            }
            bool draw_sky(const SkyDrawParams &params) override
            {
                if (active_render_target_ == 0 || !initialise_2d() || !initialise_sky_pipeline() || !ensure_frame())
                    return false;
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end() || !target->second.depth)
                    return false;
                ID3D12PipelineState *pipeline = get_sky_pipeline(target->second.samples);
                if (!pipeline)
                    return false;
                flush_2d();
                ID3D12GraphicsCommandList *commands = context_.command_list();
                commands->SetGraphicsRootSignature(sky_root_signature_);
                const D3D12_GPU_VIRTUAL_ADDRESS sky_address =
                    allocate_constants(&params.data[0][0], sizeof(params.data));
                if (sky_address == 0)
                    return false;
                commands->SetGraphicsRootConstantBufferView(0, sky_address);
                commands->SetPipelineState(pipeline);
                commands->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                commands->DrawInstanced(3, 1, 0, 0);
                return true;
            }
            bool clear_3d(float red, float green, float blue, float alpha) override
            {
                if (active_render_target_ == 0 || !ensure_frame())
                    return false;
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end() || !target->second.depth)
                    return false;
                flush_2d();
                D3D12_CPU_DESCRIPTOR_HANDLE color =
                    cpu_descriptor(render_target_heap_, render_target_descriptor_size_,
                                   target->second.descriptor);
                D3D12_CPU_DESCRIPTOR_HANDLE depth =
                    cpu_descriptor(depth_stencil_heap_, depth_stencil_descriptor_size_,
                                   target->second.depth_descriptor);
                const float colour[4] = {red, green, blue, alpha};
                context_.command_list()->ClearRenderTargetView(color, colour, 0, nullptr);
                context_.command_list()->ClearDepthStencilView(
                    depth, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
                return true;
            }
            bool clear_depth_3d() override
            {
                if (active_render_target_ == 0 || !ensure_frame())
                    return false;
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end() || !target->second.depth)
                    return false;
                flush_2d();
                D3D12_CPU_DESCRIPTOR_HANDLE depth =
                    cpu_descriptor(depth_stencil_heap_, depth_stencil_descriptor_size_,
                                   target->second.depth_descriptor);
                context_.command_list()->ClearDepthStencilView(depth, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
                return true;
            }
            bool create_shader(const ShaderSource &vertex_source, const ShaderSource &fragment_source,
                               std::uint32_t &handle, std::string &error) override
            {
                handle = 0;
                if (vertex_source.language != ShaderLanguage::glsl || fragment_source.language != ShaderLanguage::glsl)
                {
                    error = "The D3D12 backend accepts GLSL shader sources only.";
                    return false;
                }
                if (!context_.device() || !ensure_descriptor_heaps())
                {
                    error = "The D3D12 device is not ready for shader creation.";
                    return false;
                }
                ShaderProgram program;
                if (!translate_glsl_to_hlsl(vertex_source.text, TranslateStage::vertex, nullptr,
                                            &program.samplers, program.vertex, error))
                    return false;
                if (!translate_glsl_to_hlsl(fragment_source.text, TranslateStage::fragment,
                                            &program.vertex.varyings, &program.samplers, program.pixel, error))
                    return false;
                const std::uint32_t sampler_limit =
                    dxc_available(context_.device()) ? max_program_samplers_sm6 : max_program_samplers_sm5;
                if (program.samplers.size() > sampler_limit)
                {
                    error = "A D3D12 shader supports at most " + std::to_string(sampler_limit) +
                            " samplers (install dxcompiler.dll for shader model 6 to raise the limit).";
                    return false;
                }
                if (!compile_program_shader(context_.device(), program.vertex.hlsl, "vs", &program.vertex_blob, error) ||
                    !compile_program_shader(context_.device(), program.pixel.hlsl, "ps", &program.pixel_blob, error))
                {
                    release_program(program);
                    return false;
                }
                program.storage_registers =
                    (std::max)(program.vertex.storage_registers, program.pixel.storage_registers);
                for (const std::uint32_t binding : program.vertex.images)
                    program.image_registers = (std::max)(program.image_registers, binding + 1);
                for (const std::uint32_t binding : program.pixel.images)
                    program.image_registers = (std::max)(program.image_registers, binding + 1);
                if (!create_program_root_signature(program, error))
                {
                    release_program(program);
                    return false;
                }
                return store_program(program, handle);
            }
            bool create_compute_shader(const ShaderSource &source, std::uint32_t &handle,
                                       std::string &error) override
            {
                handle = 0;
                if (source.language != ShaderLanguage::glsl)
                {
                    error = "The D3D12 backend accepts GLSL shader sources only.";
                    return false;
                }
                if (!context_.device() || !ensure_descriptor_heaps())
                {
                    error = "The D3D12 device is not ready for shader creation.";
                    return false;
                }
                ShaderProgram program;
                program.compute = true;
                if (!translate_glsl_to_hlsl(source.text, TranslateStage::compute, nullptr, &program.samplers,
                                            program.pixel, error))
                    return false;
                const std::uint32_t sampler_limit =
                    dxc_available(context_.device()) ? max_program_samplers_sm6 : max_program_samplers_sm5;
                if (program.samplers.size() > sampler_limit)
                {
                    error = "A D3D12 shader supports at most " + std::to_string(sampler_limit) +
                            " samplers (install dxcompiler.dll for shader model 6 to raise the limit).";
                    return false;
                }
                if (!compile_program_shader(context_.device(), program.pixel.hlsl, "cs", &program.compute_blob, error))
                {
                    release_program(program);
                    return false;
                }
                program.storage_registers = program.pixel.storage_registers;
                for (const std::uint32_t binding : program.pixel.images)
                    program.image_registers = (std::max)(program.image_registers, binding + 1);
                if (!create_program_root_signature(program, error))
                {
                    release_program(program);
                    return false;
                }
                D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
                desc.pRootSignature = program.root_signature;
                desc.CS = {program.compute_blob->GetBufferPointer(), program.compute_blob->GetBufferSize()};
                HRESULT pipeline_result =
                    context_.device()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&program.compute_pipeline));
                trace("compute pipeline HRESULT 0x%08lX", static_cast<unsigned long>(pipeline_result));
                if (FAILED(pipeline_result) && dxc_available(context_.device()) &&
                    program.samplers.size() <= max_program_samplers_sm5)
                {
                    ID3DBlob *fallback = nullptr;
                    std::string fallback_error;
                    if (compile_program_shader_fxc(program.pixel.hlsl, "cs", &fallback, fallback_error))
                    {
                        program.compute_blob->Release();
                        program.compute_blob = fallback;
                        program.fxc_fallback = true;
                        desc.CS = {fallback->GetBufferPointer(), fallback->GetBufferSize()};
                        pipeline_result = context_.device()->CreateComputePipelineState(
                            &desc, IID_PPV_ARGS(&program.compute_pipeline));
                    }
                }
                if (FAILED(pipeline_result))
                {
                    char code[48];
                    std::snprintf(code, sizeof(code), " (HRESULT 0x%08lX)", static_cast<unsigned long>(pipeline_result));
                    error = std::string("Unable to create the D3D12 compute pipeline state.") + code;
                    release_program(program);
                    return false;
                }
                return store_program(program, handle);
            }
            bool create_shader_module(ShaderStage, const ShaderSource &, std::uint32_t &,
                                      std::string &error) override
            {
                error = "The D3D12 renderer uses linked GLSL programs rather than standalone shader modules.";
                return false;
            }
            void destroy_shader(std::uint32_t handle) override
            {
                const auto found = programs_.find(handle);
                if (found == programs_.end())
                    return;
                if (active_program_ == handle)
                {
                    flush_2d();
                    active_program_ = 0;
                }
                // The in-flight command list may still reference the program's pipeline objects.
                retire_program(found->second);
                programs_.erase(found);
            }
            bool use_shader(std::uint32_t handle) override
            {
                if (programs_.find(handle) == programs_.end())
                    return false;
                if (active_program_ != handle)
                {
                    flush_2d();
                    active_program_ = handle;
                }
                return true;
            }
            void stop_shader() override
            {
                flush_2d();
                active_program_ = 0;
            }
            bool dispatch_compute(std::uint32_t handle, unsigned int groups_x, unsigned int groups_y,
                                  unsigned int groups_z) override
            {
                const auto found = programs_.find(handle);
                if (found == programs_.end() || !found->second.compute)
                {
                    last_error_ = "The shader is not a D3D12 compute program.";
                    return false;
                }
                if (!initialise_2d() || !ensure_frame())
                    return false;
                flush_2d();
                ShaderProgram &program = found->second;
                auto *commands = context_.command_list();
                commands->SetComputeRootSignature(program.root_signature);
                ID3D12DescriptorHeap *heaps[] = {srv_heap_, sampler_heap_};
                commands->SetDescriptorHeaps(2, heaps);
                commands->SetPipelineState(program.compute_pipeline);
                if (program.pixel_constants_parameter >= 0)
                {
                    const D3D12_GPU_VIRTUAL_ADDRESS address =
                        allocate_constants(program.pixel_constants.data(), program.pixel_constants.size());
                    if (!address)
                        return false;
                    commands->SetComputeRootConstantBufferView(static_cast<UINT>(program.pixel_constants_parameter),
                                                               address);
                }
                if (!bind_program_resources(program, false))
                    return false;
                commands->Dispatch(groups_x, groups_y, groups_z);
                static int traced_dispatches = 0;
                if (traced_dispatches++ < 3)
                    trace("dispatch recorded");
                restore_image_textures();
                D3D12_RESOURCE_BARRIER barrier{};
                barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
                commands->ResourceBarrier(1, &barrier);
                return true;
            }
            // Like glUseProgram semantics: setting a uniform on a graphics program makes it the active 2D program.
            bool activate_for_uniform(std::uint32_t handle, const ShaderProgram &program)
            {
                if (!program.compute)
                    use_shader(handle);
                return true;
            }
            bool set_shader_int(std::uint32_t handle, const char *name, int value) override
            {
                const auto found = programs_.find(handle);
                if (found == programs_.end() || !activate_for_uniform(handle, found->second))
                    return false;
                if (set_sampler_unit(found->second, name, value))
                    return true;
                return set_program_uniform(found->second, name, &value, sizeof(value));
            }
            bool set_shader_float(std::uint32_t handle, const char *name, float value) override
            {
                const auto found = programs_.find(handle);
                return found != programs_.end() && activate_for_uniform(handle, found->second) && set_program_uniform(found->second, name, &value, sizeof(value));
            }
            bool set_shader_float2(std::uint32_t handle, const char *name, float x, float y) override
            {
                const float values[2] = {x, y};
                const auto found = programs_.find(handle);
                return found != programs_.end() && activate_for_uniform(handle, found->second) && set_program_uniform(found->second, name, values, sizeof(values));
            }
            bool set_shader_int2(std::uint32_t handle, const char *name, int x, int y) override
            {
                const int values[2] = {x, y};
                const auto found = programs_.find(handle);
                return found != programs_.end() && activate_for_uniform(handle, found->second) && set_program_uniform(found->second, name, values, sizeof(values));
            }
            bool set_shader_float3(std::uint32_t handle, const char *name, float x, float y, float z) override
            {
                const float values[3] = {x, y, z};
                const auto found = programs_.find(handle);
                return found != programs_.end() && activate_for_uniform(handle, found->second) && set_program_uniform(found->second, name, values, sizeof(values));
            }
            bool set_shader_mat4(std::uint32_t handle, const char *name, const float *matrix) override
            {
                const auto found = programs_.find(handle);
                return matrix && found != programs_.end() && activate_for_uniform(handle, found->second) &&
                       set_program_uniform(found->second, name, matrix, sizeof(float) * 16);
            }

            bool initialise_2d() override
            {
                if (initialized_2d_)
                    return true;
                if (root_signature_)
                    return false;
                shutdown_2d_pending_ = false;
                if (!ensure_descriptor_heaps())
                    return false;

                const std::string vertex_source = load_shader("default_2d.vert.hlsl");
                const std::string pixel_source = load_shader("default_2d.frag.hlsl");
                if (vertex_source.empty() || pixel_source.empty())
                {
                    last_error_ = "Unable to load the D3D12 default 2D shader assets.";
                    return false;
                }
                ID3DBlob *vertex_blob = nullptr;
                ID3DBlob *pixel_blob = nullptr;
                if (!compile_shader(vertex_source, "main", "vs_5_0", &vertex_blob, last_error_) ||
                    !compile_shader(pixel_source, "main", "ps_5_0", &pixel_blob, last_error_))
                {
                    if (vertex_blob)
                        vertex_blob->Release();
                    if (pixel_blob)
                        pixel_blob->Release();
                    return false;
                }

                // Ranges: colour SRV t0, colour sampler s0, normal-map SRV t1, normal-map sampler s1.
                D3D12_DESCRIPTOR_RANGE ranges[4]{};
                for (UINT index = 0; index < 4; ++index)
                {
                    ranges[index].RangeType = (index % 2 == 0) ? D3D12_DESCRIPTOR_RANGE_TYPE_SRV
                                                               : D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                    ranges[index].NumDescriptors = 1;
                    ranges[index].BaseShaderRegister = index / 2;
                    ranges[index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                }
                D3D12_ROOT_PARAMETER parameters[3]{};
                parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
                parameters[0].Constants.ShaderRegister = 0;
                parameters[0].Constants.RegisterSpace = 0;
                parameters[0].Constants.Num32BitValues = 16;
                parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
                for (UINT index = 0; index < 2; ++index)
                {
                    parameters[index + 1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                    parameters[index + 1].DescriptorTable.NumDescriptorRanges = 1;
                    parameters[index + 1].DescriptorTable.pDescriptorRanges = &ranges[index];
                    parameters[index + 1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                }
                D3D12_ROOT_SIGNATURE_DESC signature_desc{};
                signature_desc.NumParameters = static_cast<UINT>(std::size(parameters));
                signature_desc.pParameters = parameters;
                signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
                ID3DBlob *serialized = nullptr;
                ID3DBlob *messages = nullptr;
                HRESULT result = D3D12SerializeRootSignature(&signature_desc, D3D_ROOT_SIGNATURE_VERSION_1,
                                                             &serialized, &messages);
                if (FAILED(result))
                {
                    last_error_ = messages ? static_cast<const char *>(messages->GetBufferPointer())
                                           : "Unable to serialize the D3D12 2D root signature.";
                    if (messages)
                        messages->Release();
                    vertex_blob->Release();
                    pixel_blob->Release();
                    return false;
                }
                if (messages)
                    messages->Release();
                result = context_.device()->CreateRootSignature(0, serialized->GetBufferPointer(),
                                                                serialized->GetBufferSize(),
                                                                IID_PPV_ARGS(&root_signature_));
                serialized->Release();
                if (FAILED(result))
                {
                    vertex_blob->Release();
                    pixel_blob->Release();
                    last_error_ = "Unable to create the D3D12 2D root signature.";
                    return false;
                }

                default_vertex_blob_ = vertex_blob;
                default_pixel_blob_ = pixel_blob;
                for (std::size_t primitive = 0; primitive < primitive_pipeline_count; ++primitive)
                {
                    if (!get_default_pipeline(primitive, 0))
                    {
                        result = E_FAIL;
                        break;
                    }
                }
                if (FAILED(result))
                {
                    last_error_ = "Unable to create the D3D12 2D pipeline states.";
                    shutdown_2d();
                    return false;
                }

                const D3D12_HEAP_PROPERTIES upload_heap = heap_properties(D3D12_HEAP_TYPE_UPLOAD);
                const D3D12_RESOURCE_DESC vertex_desc =
                    buffer_description(sizeof(Vertex2D) * vertex_buffer_vertices);
                result = context_.device()->CreateCommittedResource(
                    &upload_heap, D3D12_HEAP_FLAG_NONE, &vertex_desc,
                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertex_buffer_));
                if (FAILED(result))
                {
                    last_error_ = "Unable to create the D3D12 2D vertex buffer.";
                    shutdown_2d();
                    return false;
                }
                D3D12_RANGE read_range{0, 0};
                result = vertex_buffer_->Map(0, &read_range, reinterpret_cast<void **>(&mapped_vertices_));
                if (FAILED(result))
                {
                    mapped_vertices_ = nullptr;
                    last_error_ = "Unable to map the D3D12 2D vertex buffer.";
                    shutdown_2d();
                    return false;
                }
                vertex_capacity_ = vertex_buffer_vertices;
                vertex_offset_ = 0;

                const std::uint8_t white_pixel[4] = {255, 255, 255, 255};
                if (!create_texture({1, 1, TextureFilter::nearest}, white_texture_) ||
                    !upload_texture(white_texture_, 1, 1, white_pixel))
                {
                    shutdown_2d();
                    return false;
                }
                initialized_2d_ = true;
                return true;
            }

            void shutdown_2d() override
            {
                if (context_.frame_open())
                {
                    flush_2d();
                    shutdown_2d_pending_ = true;
                    return;
                }
                context_.wait_idle();
                release_2d_resources();
            }

            void release_2d_resources()
            {
                batch_vertices_.clear();
                if (vertex_buffer_)
                {
                    if (mapped_vertices_)
                        vertex_buffer_->Unmap(0, nullptr);
                    vertex_buffer_->Release();
                }
                vertex_buffer_ = nullptr;
                mapped_vertices_ = nullptr;
                vertex_capacity_ = 0;
                for (auto *&pipeline : pipelines_)
                {
                    if (pipeline)
                        pipeline->Release();
                    pipeline = nullptr;
                }
                if (default_vertex_blob_)
                    default_vertex_blob_->Release();
                if (default_pixel_blob_)
                    default_pixel_blob_->Release();
                default_vertex_blob_ = nullptr;
                default_pixel_blob_ = nullptr;
                if (root_signature_)
                    root_signature_->Release();
                root_signature_ = nullptr;
                destroy_texture(white_texture_);
                white_texture_ = 0;
                initialized_2d_ = false;
                shutdown_2d_pending_ = false;
            }

            bool begin_2d(int width, int height) override
            {
                if (!initialise_2d() || !ensure_frame())
                    return false;
                flush_2d();
                projection_ = {width > 0 ? 2.0f / width : 0.0f, 0, 0, 0,
                               0, height > 0 ? -2.0f / height : 0.0f, 0, 0,
                               0, 0, -1, 0, -1, 1, 0, 1};
                D3D12_VIEWPORT viewport{0.0f, 0.0f,
                                        static_cast<float>((std::max)(width, 0)),
                                        static_cast<float>((std::max)(height, 0)), 0.0f, 1.0f};
                last_2d_width_ = width;
                last_2d_height_ = height;
                D3D12_RECT scissor{0, 0, (std::max)(width, 0), (std::max)(height, 0)};
                auto *commands = context_.command_list();
                commands->RSSetViewports(1, &viewport);
                commands->RSSetScissorRects(1, &scissor);
                commands->SetGraphicsRootSignature(root_signature_);
                ID3D12DescriptorHeap *heaps[] = {srv_heap_, sampler_heap_};
                commands->SetDescriptorHeaps(2, heaps);
                commands->SetGraphicsRoot32BitConstants(0, 16, projection_.data(), 0);
                return true;
            }

            bool begin_shader_2d(std::uint32_t program, int width, int height) override
            {
                if (width <= 0 || height <= 0 || !initialise_2d() || !ensure_frame())
                    return false;
                flush_2d();
                const auto found = programs_.find(program);
                if (found == programs_.end() || found->second.compute)
                    return false;
                active_program_ = program;
                float projection[16] = {};
                projection[0] = 2.0f / width;
                projection[5] = -2.0f / height;
                projection[10] = -1.0f;
                projection[12] = -1.0f + 2.0f * detail::screen_offset_x() / width;
                projection[13] = 1.0f - 2.0f * detail::screen_offset_y() / height;
                projection[15] = 1.0f;
                set_program_uniform(found->second, "uProjection", projection, sizeof(projection));
                set_sampler_unit(found->second, "uTexture", 0);
                return true;
            }
            void set_premultiplied_alpha(bool enabled) override
            {
                if (premultiplied_alpha_ != enabled)
                {
                    flush_2d();
                    premultiplied_alpha_ = enabled;
                }
            }
            void set_blend_mode(BlendMode mode) override
            {
                if (blend_mode_ != mode)
                {
                    flush_2d();
                    blend_mode_ = mode;
                }
            }
            bool clear_frame(float red, float green, float blue, float alpha) override
            {
                if (!ensure_frame())
                    return false;
                flush_2d();
                if (active_render_target_ == 0)
                {
                    context_.clear(red, green, blue, alpha);
                }
                else
                {
                    const auto target = render_targets_.find(active_render_target_);
                    if (target == render_targets_.end())
                        return false;
                    D3D12_CPU_DESCRIPTOR_HANDLE handle =
                        cpu_descriptor(render_target_heap_, render_target_descriptor_size_,
                                       target->second.descriptor);
                    const float colour[4] = {red, green, blue, alpha};
                    context_.command_list()->ClearRenderTargetView(handle, colour, 0, nullptr);
                }
                return true;
            }

            void set_clip_rect_2d(const ClipRect2D &clip) override
            {
                flush_2d();
                clip_ = clip;
                if (!clip.enabled && context_.frame_open() && last_2d_width_ > 0)
                {
                    D3D12_RECT scissor{0, 0, last_2d_width_, last_2d_height_};
                    context_.command_list()->RSSetScissorRects(1, &scissor);
                }
            }

            void flush_2d() override
            {
                if (batch_vertices_.empty() || !mapped_vertices_ || !context_.frame_open())
                    return;
                if (clip_.enabled)
                {
                    // The D3D12 2D viewport is sized to the drawing space, so no scaling is needed.
                    int x = 0, y = 0, w = 0, h = 0;
                    clip_rect_to_pixels(clip_, last_2d_width_, last_2d_height_, last_2d_width_, last_2d_height_,
                                        detail::screen_offset_x(), detail::screen_offset_y(), x, y, w, h);
                    D3D12_RECT scissor{x, y, x + w, y + h};
                    context_.command_list()->RSSetScissorRects(1, &scissor);
                }
                const std::size_t count = batch_vertices_.size();
                if (vertex_frame_serial_ != context_.frame_serial())
                {
                    vertex_frame_serial_ = context_.frame_serial();
                    vertex_offset_ = 0;
                }
                if (vertex_offset_ + count > vertex_capacity_)
                {
                    // The per-frame vertex region is full; report it instead of dropping geometry silently.
                    last_error_ = "D3D12 2D vertex buffer is full for this frame; some 2D geometry was dropped.";
                    batch_vertices_.clear();
                    return;
                }
                const std::size_t first_vertex = vertex_offset_;
                std::memcpy(mapped_vertices_ + first_vertex, batch_vertices_.data(), sizeof(Vertex2D) * count);
                vertex_offset_ += count;

                auto *commands = context_.command_list();
                std::size_t pipeline_index = 0;
                D3D12_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                if (batch_primitive_ == PrimitiveType::lines)
                {
                    pipeline_index = 1;
                    topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                }
                else if (batch_primitive_ == PrimitiveType::points)
                {
                    pipeline_index = 2;
                    topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
                }
                const std::size_t variant = blend_mode_ == BlendMode::additive        ? 2
                                            : blend_mode_ == BlendMode::multiply      ? 3
                                            : blend_mode_ == BlendMode::screen        ? 4
                                            : blend_mode_ == BlendMode::premultiplied ? 1
                                            : premultiplied_alpha_                    ? 1
                                                                                      : 0;
                const auto program = active_program_ != 0 ? programs_.find(active_program_) : programs_.end();
                const bool bound = program != programs_.end() && !program->second.compute
                                       ? bind_program_for_draw(program->second, pipeline_index, variant)
                                       : bind_default_for_draw(pipeline_index, variant);
                if (!bound)
                {
                    restore_image_textures();
                    batch_vertices_.clear();
                    return;
                }
                D3D12_VERTEX_BUFFER_VIEW vertex_view{};
                vertex_view.BufferLocation =
                    vertex_buffer_->GetGPUVirtualAddress() + sizeof(Vertex2D) * first_vertex;
                vertex_view.SizeInBytes = static_cast<UINT>(sizeof(Vertex2D) * count);
                vertex_view.StrideInBytes = sizeof(Vertex2D);
                commands->IASetVertexBuffers(0, 1, &vertex_view);
                commands->IASetPrimitiveTopology(topology);
                static int traced_draws = 0;
                if (traced_draws < 6 || (active_program_ != 0 && traced_draws < 40))
                {
                    ++traced_draws;
                    trace("draw 2d: %zu vertices%s", count, active_program_ != 0 ? " (user shader)" : "");
                }
                commands->DrawInstanced(static_cast<UINT>(count), 1, 0, 0);
                restore_image_textures();
                batch_vertices_.clear();
            }

            void submit_2d(PrimitiveType primitive_mode, const Vertex2D *vertices,
                           int count, std::uint32_t texture) override
            {
                if (!vertices || count <= 0 || !initialise_2d())
                    return;
                const std::uint32_t resolved_texture = texture != 0 ? texture : white_texture_;
                const PrimitiveType target_primitive = get_target_batch_primitive(primitive_mode);
                const int new_vertex_count = get_decomposed_vertex_count(primitive_mode, count);
                if (new_vertex_count <= 0)
                    return;
                if (!batch_vertices_.empty() &&
                    (batch_texture_ != resolved_texture ||
                     batch_primitive_ != target_primitive ||
                     batch_vertices_.size() + static_cast<std::size_t>(new_vertex_count) > max_batch_vertices))
                    flush_2d();
                if (batch_vertices_.empty())
                {
                    batch_texture_ = resolved_texture;
                    batch_primitive_ = target_primitive;
                }
                PrimitiveType actual_primitive = batch_primitive_;
                append_decomposed_vertices(primitive_mode, vertices, count,
                                           actual_primitive, batch_vertices_);
            }

            bool create_storage_buffer(std::size_t size, std::uint32_t &handle) override
            {
                handle = 0;
                if (size == 0 || !context_.device())
                    return false;
                StorageBuffer buffer;
                if (!create_storage_resource(size, buffer))
                    return false;
                handle = next_storage_buffer_++;
                if (handle == 0)
                    handle = next_storage_buffer_++;
                storage_buffers_.emplace(handle, buffer);
                return true;
            }
            void destroy_storage_buffer(std::uint32_t handle) override
            {
                const auto found = storage_buffers_.find(handle);
                if (found == storage_buffers_.end())
                    return;
                for (std::uint32_t &bound : bound_storage_buffers_)
                {
                    if (bound == handle)
                        bound = 0;
                }
                if (found->second.resource)
                {
                    if (context_.frame_open())
                        retired_objects_.push_back(found->second.resource);
                    else
                    {
                        context_.wait_idle();
                        found->second.resource->Release();
                    }
                }
                storage_buffers_.erase(found);
            }
            bool upload_storage_buffer(std::uint32_t handle, std::size_t size, const void *data,
                                       bool preserve) override
            {
                const auto found = storage_buffers_.find(handle);
                if (found == storage_buffers_.end() || size == 0 || (preserve && !data))
                    return found != storage_buffers_.end();
                if (!ensure_frame())
                    return false;
                flush_2d();
                StorageBuffer &buffer = found->second;
                if (size > buffer.size)
                {
                    StorageBuffer replacement;
                    if (!create_storage_resource(size, replacement))
                        return false;
                    retired_objects_.push_back(buffer.resource);
                    buffer = replacement;
                }
                if (!data)
                    return true;

                ID3D12Resource *staging = nullptr;
                const D3D12_HEAP_PROPERTIES upload_heap = heap_properties(D3D12_HEAP_TYPE_UPLOAD);
                const D3D12_RESOURCE_DESC staging_desc = buffer_description(size);
                if (FAILED(context_.device()->CreateCommittedResource(
                        &upload_heap, D3D12_HEAP_FLAG_NONE, &staging_desc, D3D12_RESOURCE_STATE_GENERIC_READ,
                        nullptr, IID_PPV_ARGS(&staging))))
                    return false;
                void *mapped = nullptr;
                const D3D12_RANGE no_read{0, 0};
                if (FAILED(staging->Map(0, &no_read, &mapped)))
                {
                    staging->Release();
                    return false;
                }
                std::memcpy(mapped, data, size);
                staging->Unmap(0, nullptr);
                retired_objects_.push_back(staging);

                auto *commands = context_.command_list();
                transition_resource(commands, buffer.resource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                    D3D12_RESOURCE_STATE_COPY_DEST);
                commands->CopyBufferRegion(buffer.resource, 0, staging, 0, size);
                transition_resource(commands, buffer.resource, D3D12_RESOURCE_STATE_COPY_DEST,
                                    D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                return true;
            }
            bool readback_storage_buffer(std::uint32_t handle, std::size_t offset, std::size_t size,
                                         void *out_data) override
            {
                const auto found = storage_buffers_.find(handle);
                if (found == storage_buffers_.end() || !out_data || size == 0 ||
                    offset + size > found->second.size)
                    return false;
                if (context_.frame_open())
                {
                    flush_2d();
                    suspend_active_render_target();
                    context_.present(vsync_enabled_);
                    release_retired_textures();
                }
                if (!context_.wait_idle())
                    return false;

                ID3D12Resource *readback = nullptr;
                const D3D12_HEAP_PROPERTIES readback_heap = heap_properties(D3D12_HEAP_TYPE_READBACK);
                const D3D12_RESOURCE_DESC readback_desc = buffer_description(size);
                if (FAILED(context_.device()->CreateCommittedResource(
                        &readback_heap, D3D12_HEAP_FLAG_NONE, &readback_desc, D3D12_RESOURCE_STATE_COPY_DEST,
                        nullptr, IID_PPV_ARGS(&readback))))
                    return false;
                ID3D12CommandAllocator *allocator = nullptr;
                ID3D12GraphicsCommandList *commands = nullptr;
                HRESULT result = context_.device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                           IID_PPV_ARGS(&allocator));
                if (SUCCEEDED(result))
                    result = context_.device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator,
                                                                  nullptr, IID_PPV_ARGS(&commands));
                if (SUCCEEDED(result))
                {
                    transition_resource(commands, found->second.resource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                        D3D12_RESOURCE_STATE_COPY_SOURCE);
                    commands->CopyBufferRegion(readback, 0, found->second.resource, offset, size);
                    transition_resource(commands, found->second.resource, D3D12_RESOURCE_STATE_COPY_SOURCE,
                                        D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                    result = commands->Close();
                }
                if (SUCCEEDED(result))
                {
                    ID3D12CommandList *lists[] = {commands};
                    context_.command_queue()->ExecuteCommandLists(1, lists);
                    if (!context_.wait_idle())
                        result = E_FAIL;
                }
                if (commands)
                    commands->Release();
                if (allocator)
                    allocator->Release();
                bool copied = false;
                if (SUCCEEDED(result))
                {
                    void *mapped = nullptr;
                    const D3D12_RANGE read_range{0, size};
                    if (SUCCEEDED(readback->Map(0, &read_range, &mapped)))
                    {
                        std::memcpy(out_data, mapped, size);
                        const D3D12_RANGE no_write{0, 0};
                        readback->Unmap(0, &no_write);
                        copied = true;
                    }
                }
                readback->Release();
                return copied;
            }
            void bind_storage_buffer(unsigned int binding, std::uint32_t handle) override
            {
                flush_2d();
                if (binding < max_storage_bindings)
                    bound_storage_buffers_[binding] = handle;
            }
            void bind_texture_unit(unsigned int unit, std::uint32_t texture) override
            {
                flush_2d();
                if (unit < max_texture_units)
                    bound_texture_units_[unit] = texture;
            }
            void bind_storage_texture(unsigned int unit, std::uint32_t texture) override
            {
                flush_2d();
                if (unit < max_image_units)
                    bound_image_units_[unit] = texture;
            }
            void storage_barrier() override
            {
                flush_2d();
                if (!context_.frame_open())
                    return;
                D3D12_RESOURCE_BARRIER barrier{};
                barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
                context_.command_list()->ResourceBarrier(1, &barrier);
            }

        private:
            int current_target_samples() const
            {
                if (active_render_target_ == 0)
                    return 1;
                const auto target = render_targets_.find(active_render_target_);
                return target == render_targets_.end() ? 1 : target->second.samples;
            }

            int supported_sample_count(int requested) const
            {
                int count = 1;
                while (count * 2 <= requested && count < 8)
                    count *= 2;
                for (; count > 1; count /= 2)
                {
                    bool supported = true;
                    for (const DXGI_FORMAT format : {DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT})
                    {
                        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels{};
                        levels.Format = format;
                        levels.SampleCount = static_cast<UINT>(count);
                        if (FAILED(context_.device()->CheckFeatureSupport(
                                D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &levels, sizeof(levels))) ||
                            levels.NumQualityLevels == 0)
                            supported = false;
                    }
                    if (supported)
                        return count;
                }
                return 1;
            }

            // Resolves the multisampled colour buffer into the target texture, leaving it shader-readable.
            void resolve_render_target(RenderTarget &target, Texture &texture)
            {
                ID3D12GraphicsCommandList *commands = context_.command_list();
                transition_resource(commands, target.msaa_colour, D3D12_RESOURCE_STATE_RENDER_TARGET,
                                    D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
                if (texture.state != D3D12_RESOURCE_STATE_RESOLVE_DEST)
                    transition_resource(commands, texture.resource, texture.state,
                                        D3D12_RESOURCE_STATE_RESOLVE_DEST);
                commands->ResolveSubresource(texture.resource, 0, target.msaa_colour, 0,
                                             DXGI_FORMAT_R8G8B8A8_UNORM);
                transition_resource(commands, texture.resource, D3D12_RESOURCE_STATE_RESOLVE_DEST,
                                    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                transition_resource(commands, target.msaa_colour, D3D12_RESOURCE_STATE_RESOLVE_SOURCE,
                                    D3D12_RESOURCE_STATE_RENDER_TARGET);
                texture.state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
            }

            bool initialise_mesh_pipeline()
            {
                if (mesh_root_signature_)
                    return true;
                if (!ensure_descriptor_heaps())
                    return false;
                const std::string vertex_source = load_shader("mesh3d.vert.hlsl");
                const std::string pixel_source = load_shader("mesh3d.frag.hlsl");
                const std::string instanced_source = load_shader("mesh3d_instanced.vert.hlsl");
                if (vertex_source.empty() || pixel_source.empty() || instanced_source.empty())
                {
                    last_error_ = "Unable to load the D3D12 3D mesh shaders.";
                    return false;
                }
                ID3DBlob *vertex_blob = nullptr;
                ID3DBlob *pixel_blob = nullptr;
                ID3DBlob *instanced_blob = nullptr;
                if (!compile_shader(vertex_source, "main", "vs_5_0", &vertex_blob, last_error_) ||
                    !compile_shader(pixel_source, "main", "ps_5_0", &pixel_blob, last_error_) ||
                    !compile_shader(instanced_source, "main", "vs_5_0", &instanced_blob, last_error_))
                {
                    if (vertex_blob)
                        vertex_blob->Release();
                    if (pixel_blob)
                        pixel_blob->Release();
                    if (instanced_blob)
                        instanced_blob->Release();
                    return false;
                }

                // Tables 1-6: colour SRV t0, colour sampler s0, normal SRV t1, normal sampler s1, and the PBR
                // occlusion-roughness-metalness SRV t2 and sampler s2. Parameters 7-9 bind PBR constants and
                // the sun and local shadow maps.
                D3D12_DESCRIPTOR_RANGE ranges[6]{};
                for (UINT index = 0; index < 6; ++index)
                {
                    ranges[index].RangeType = index % 2 == 0 ? D3D12_DESCRIPTOR_RANGE_TYPE_SRV
                                                              : D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                    ranges[index].NumDescriptors = 1;
                    ranges[index].BaseShaderRegister = index / 2;
                    ranges[index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                }
                D3D12_DESCRIPTOR_RANGE shadow_ranges[2]{};
                for (UINT index = 0; index < 2; ++index)
                {
                    shadow_ranges[index].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                    shadow_ranges[index].NumDescriptors = 1;
                    shadow_ranges[index].BaseShaderRegister = index + 3;
                    shadow_ranges[index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                }
                D3D12_ROOT_PARAMETER parameters[10]{};
                parameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
                parameters[0].Constants.ShaderRegister = 0;
                // 32 mesh constants plus 16 for the environment, eight tables, and one CBV use 58 of 64 DWORDs.
                parameters[0].Constants.Num32BitValues = 48;
                parameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
                for (UINT index = 0; index < 6; ++index)
                {
                    parameters[index + 1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                    parameters[index + 1].DescriptorTable.NumDescriptorRanges = 1;
                    parameters[index + 1].DescriptorTable.pDescriptorRanges = &ranges[index];
                    parameters[index + 1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                }
                parameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                parameters[7].Descriptor.ShaderRegister = 1;
                parameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                parameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                parameters[8].DescriptorTable.NumDescriptorRanges = 1;
                parameters[8].DescriptorTable.pDescriptorRanges = &shadow_ranges[0];
                parameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                parameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                parameters[9].DescriptorTable.NumDescriptorRanges = 1;
                parameters[9].DescriptorTable.pDescriptorRanges = &shadow_ranges[1];
                parameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                D3D12_ROOT_SIGNATURE_DESC signature_desc{};
                signature_desc.NumParameters = static_cast<UINT>(std::size(parameters));
                signature_desc.pParameters = parameters;
                signature_desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
                ID3DBlob *serialized = nullptr;
                ID3DBlob *messages = nullptr;
                HRESULT result = D3D12SerializeRootSignature(&signature_desc, D3D_ROOT_SIGNATURE_VERSION_1,
                                                             &serialized, &messages);
                if (FAILED(result))
                {
                    last_error_ = messages ? static_cast<const char *>(messages->GetBufferPointer())
                                           : "Unable to serialize the D3D12 mesh root signature.";
                    if (messages)
                        messages->Release();
                    vertex_blob->Release();
                    pixel_blob->Release();
                    instanced_blob->Release();
                    return false;
                }
                if (messages)
                    messages->Release();
                result = context_.device()->CreateRootSignature(
                    0, serialized->GetBufferPointer(), serialized->GetBufferSize(),
                    IID_PPV_ARGS(&mesh_root_signature_));
                serialized->Release();
                if (FAILED(result))
                {
                    vertex_blob->Release();
                    pixel_blob->Release();
                    instanced_blob->Release();
                    last_error_ = "Unable to create the D3D12 mesh root signature.";
                    return false;
                }

                // Pipeline states are created on demand per cull mode and render-target sample count.
                mesh_vertex_blob_ = vertex_blob;
                mesh_pixel_blob_ = pixel_blob;
                mesh_instanced_vertex_blob_ = instanced_blob;
                return true;
            }

            bool create_pipeline_state(const D3D12_GRAPHICS_PIPELINE_STATE_DESC &desc, ID3D12PipelineState *&slot)
            {
                if (FAILED(context_.device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&slot))))
                    return false;
                ++pipelines_created_;
                return true;
            }

            ID3D12PipelineState *get_mesh_pipeline(std::size_t cull, int samples, bool instanced = false, std::uint32_t blend = 0, bool wire = false)
            {
                ID3D12PipelineState *&slot =
                    (instanced ? mesh_instanced_pipelines_ : mesh_pipelines_)[(wire ? 9 * sample_slot_count : 0) + sample_slot(samples) * 9 + blend * 3 + cull];
                if (slot)
                    return slot;
                static const D3D12_INPUT_ELEMENT_DESC elements[] = {
                    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex3D, position)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex3D, normal)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex3D, colour)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex3D, uv)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex3D, tangent)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    // Slot 1: the instance's model matrix columns and tint, advancing once per instance.
                    {"INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
                    {"INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
                    {"INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
                    {"INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
                    {"INSTANCE", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1}};
                constexpr UINT vertex_element_count = 5;
                D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_desc{};
                pipeline_desc.pRootSignature = mesh_root_signature_;
                ID3DBlob *vertex_blob = instanced ? mesh_instanced_vertex_blob_ : mesh_vertex_blob_;
                pipeline_desc.VS = {vertex_blob->GetBufferPointer(), vertex_blob->GetBufferSize()};
                pipeline_desc.PS = {mesh_pixel_blob_->GetBufferPointer(), mesh_pixel_blob_->GetBufferSize()};
                pipeline_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
                if (blend != 0)
                {
                    // 1 alpha, 2 additive; blended draws test depth but do not write it.
                    D3D12_RENDER_TARGET_BLEND_DESC &target = pipeline_desc.BlendState.RenderTarget[0];
                    target.BlendEnable = TRUE;
                    target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
                    target.DestBlend = blend == 2 ? D3D12_BLEND_ONE : D3D12_BLEND_INV_SRC_ALPHA;
                    target.BlendOp = D3D12_BLEND_OP_ADD;
                    target.SrcBlendAlpha = D3D12_BLEND_ONE;
                    target.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
                    target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
                }
                pipeline_desc.SampleMask = UINT_MAX;
                pipeline_desc.RasterizerState.FillMode = wire ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
                pipeline_desc.RasterizerState.CullMode =
                    cull == 0 ? D3D12_CULL_MODE_BACK :
                    cull == 1 ? D3D12_CULL_MODE_FRONT : D3D12_CULL_MODE_NONE;
                pipeline_desc.RasterizerState.FrontCounterClockwise = TRUE;
                pipeline_desc.RasterizerState.DepthClipEnable = TRUE;
                pipeline_desc.DepthStencilState.DepthEnable = TRUE;
                pipeline_desc.DepthStencilState.DepthWriteMask =
                    blend == 0 ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
                pipeline_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
                pipeline_desc.DepthStencilState.StencilEnable = FALSE;
                pipeline_desc.InputLayout = {elements, instanced ? static_cast<UINT>(std::size(elements)) : vertex_element_count};
                pipeline_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                pipeline_desc.NumRenderTargets = 1;
                pipeline_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                pipeline_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
                pipeline_desc.SampleDesc.Count = static_cast<UINT>(samples);
                if (!create_pipeline_state(pipeline_desc, slot))
                {
                    slot = nullptr;
                    last_error_ = "Unable to create the D3D12 mesh pipeline state.";
                }
                return slot;
            }

            bool initialise_sky_pipeline()
            {
                if (sky_root_signature_)
                    return true;
                const std::string vertex_source = load_shader("sky.vert.hlsl");
                const std::string pixel_source = load_shader("sky.frag.hlsl");
                if (vertex_source.empty() || pixel_source.empty())
                {
                    last_error_ = "Unable to load the D3D12 sky shaders.";
                    return false;
                }
                ID3DBlob *vertex_blob = nullptr;
                ID3DBlob *pixel_blob = nullptr;
                // Optimisation level 0 keeps the loop-heavy sky shader from taking seconds to compile.
                if (!compile_shader(vertex_source, "main", "vs_5_0", &vertex_blob, last_error_) ||
                    !compile_shader(pixel_source, "main", "ps_5_0", &pixel_blob, last_error_,
                                    D3DCOMPILE_OPTIMIZATION_LEVEL0))
                {
                    if (vertex_blob)
                        vertex_blob->Release();
                    if (pixel_blob)
                        pixel_blob->Release();
                    return false;
                }
                // The sky rows are too large for root constants, so they live in a constant buffer.
                D3D12_ROOT_PARAMETER parameter{};
                parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                parameter.Descriptor.ShaderRegister = 0;
                parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
                D3D12_ROOT_SIGNATURE_DESC signature_desc{};
                signature_desc.NumParameters = 1;
                signature_desc.pParameters = &parameter;
                ID3DBlob *serialized = nullptr;
                ID3DBlob *messages = nullptr;
                HRESULT result = D3D12SerializeRootSignature(&signature_desc, D3D_ROOT_SIGNATURE_VERSION_1,
                                                             &serialized, &messages);
                if (FAILED(result))
                {
                    last_error_ = messages ? static_cast<const char *>(messages->GetBufferPointer())
                                           : "Unable to serialize the D3D12 sky root signature.";
                    if (messages)
                        messages->Release();
                    vertex_blob->Release();
                    pixel_blob->Release();
                    return false;
                }
                if (messages)
                    messages->Release();
                result = context_.device()->CreateRootSignature(
                    0, serialized->GetBufferPointer(), serialized->GetBufferSize(),
                    IID_PPV_ARGS(&sky_root_signature_));
                serialized->Release();
                if (FAILED(result))
                {
                    vertex_blob->Release();
                    pixel_blob->Release();
                    last_error_ = "Unable to create the D3D12 sky root signature.";
                    return false;
                }
                sky_vertex_blob_ = vertex_blob;
                sky_pixel_blob_ = pixel_blob;
                return true;
            }

            ID3D12PipelineState *get_sky_pipeline(int samples)
            {
                ID3D12PipelineState *&slot = sky_pipelines_[sample_slot(samples)];
                if (slot)
                    return slot;
                D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_desc{};
                pipeline_desc.pRootSignature = sky_root_signature_;
                pipeline_desc.VS = {sky_vertex_blob_->GetBufferPointer(), sky_vertex_blob_->GetBufferSize()};
                pipeline_desc.PS = {sky_pixel_blob_->GetBufferPointer(), sky_pixel_blob_->GetBufferSize()};
                pipeline_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
                pipeline_desc.SampleMask = UINT_MAX;
                pipeline_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
                pipeline_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
                pipeline_desc.RasterizerState.DepthClipEnable = TRUE;
                // The sky sits exactly on the far plane: pass at equal depth and never write it.
                pipeline_desc.DepthStencilState.DepthEnable = TRUE;
                pipeline_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
                pipeline_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
                pipeline_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                pipeline_desc.NumRenderTargets = 1;
                pipeline_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                pipeline_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
                pipeline_desc.SampleDesc.Count = static_cast<UINT>(samples);
                if (!create_pipeline_state(pipeline_desc, slot))
                {
                    slot = nullptr;
                    last_error_ = "Unable to create the D3D12 sky pipeline state.";
                }
                return slot;
            }

            static void release_mesh(Mesh &mesh)
            {
                if (mesh.vertices)
                    mesh.vertices->Release();
                if (mesh.indices)
                    mesh.indices->Release();
                mesh = {};
            }

            bool ensure_render_target_heap()
            {
                if (render_target_heap_)
                    return true;
                D3D12_DESCRIPTOR_HEAP_DESC desc{};
                desc.NumDescriptors = render_target_capacity;
                desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                if (FAILED(context_.device()->CreateDescriptorHeap(&desc,
                                                                   IID_PPV_ARGS(&render_target_heap_))))
                    return false;
                render_target_descriptor_size_ =
                    context_.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
                return true;
            }

            bool ensure_depth_stencil_heap()
            {
                if (depth_stencil_heap_)
                    return true;
                D3D12_DESCRIPTOR_HEAP_DESC desc{};
                desc.NumDescriptors = render_target_capacity;
                desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
                if (FAILED(context_.device()->CreateDescriptorHeap(&desc,
                                                                   IID_PPV_ARGS(&depth_stencil_heap_))))
                    return false;
                depth_stencil_descriptor_size_ =
                    context_.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
                return true;
            }

            std::uint32_t acquire_render_target_descriptor()
            {
                if (!free_render_target_descriptors_.empty())
                {
                    const std::uint32_t descriptor = free_render_target_descriptors_.back();
                    free_render_target_descriptors_.pop_back();
                    return descriptor;
                }
                if (next_render_target_descriptor_ >= render_target_capacity)
                    return render_target_capacity;
                return next_render_target_descriptor_++;
            }

            std::uint32_t acquire_depth_stencil_descriptor()
            {
                if (!free_depth_stencil_descriptors_.empty())
                {
                    const std::uint32_t descriptor = free_depth_stencil_descriptors_.back();
                    free_depth_stencil_descriptors_.pop_back();
                    return descriptor;
                }
                if (next_depth_stencil_descriptor_ >= render_target_capacity)
                    return render_target_capacity;
                return next_depth_stencil_descriptor_++;
            }

            bool readback_resource(ID3D12Resource *resource, D3D12_RESOURCE_STATES source_state,
                                   int width, int height, std::uint8_t *pixels)
            {
                if (!resource || !pixels || width <= 0 || height <= 0 ||
                    active_render_target_ != 0 || !context_.device() || !context_.command_queue())
                    return false;
                const D3D12_RESOURCE_DESC resource_desc = resource->GetDesc();
                if (resource_desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D ||
                    resource_desc.Width != static_cast<UINT64>(width) ||
                    resource_desc.Height != static_cast<UINT>(height) ||
                    resource_desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM)
                    return false;

                if (context_.frame_open())
                {
                    flush_2d();
                    context_.present(vsync_enabled_);
                    release_retired_textures();
                }
                if (!context_.wait_idle())
                    return false;

                D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};
                UINT row_count = 0;
                UINT64 row_size = 0;
                UINT64 readback_size = 0;
                context_.device()->GetCopyableFootprints(&resource_desc, 0, 1, 0,
                                                         &footprint, &row_count, &row_size, &readback_size);
                ID3D12Resource *readback = nullptr;
                const D3D12_RESOURCE_DESC readback_desc = buffer_description(readback_size);
                const D3D12_HEAP_PROPERTIES readback_heap = heap_properties(D3D12_HEAP_TYPE_READBACK);
                HRESULT result = context_.device()->CreateCommittedResource(
                    &readback_heap, D3D12_HEAP_FLAG_NONE, &readback_desc,
                    D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&readback));
                if (FAILED(result))
                    return false;

                ID3D12CommandAllocator *allocator = nullptr;
                ID3D12GraphicsCommandList *commands = nullptr;
                result = context_.device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                   IID_PPV_ARGS(&allocator));
                if (SUCCEEDED(result))
                    result = context_.device()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                                  allocator, nullptr, IID_PPV_ARGS(&commands));
                if (FAILED(result))
                {
                    if (commands)
                        commands->Release();
                    if (allocator)
                        allocator->Release();
                    readback->Release();
                    return false;
                }

                transition_resource(commands, resource, source_state, D3D12_RESOURCE_STATE_COPY_SOURCE);
                D3D12_TEXTURE_COPY_LOCATION source{};
                source.pResource = resource;
                source.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                source.SubresourceIndex = 0;
                D3D12_TEXTURE_COPY_LOCATION destination{};
                destination.pResource = readback;
                destination.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                destination.PlacedFootprint = footprint;
                commands->CopyTextureRegion(&destination, 0, 0, 0, &source, nullptr);
                transition_resource(commands, resource, D3D12_RESOURCE_STATE_COPY_SOURCE, source_state);
                result = commands->Close();
                if (SUCCEEDED(result))
                {
                    ID3D12CommandList *lists[] = {commands};
                    context_.command_queue()->ExecuteCommandLists(1, lists);
                    if (!context_.wait_idle())
                        result = E_FAIL;
                }
                commands->Release();
                allocator->Release();
                if (FAILED(result))
                {
                    readback->Release();
                    return false;
                }

                const D3D12_RANGE read_range{0, static_cast<SIZE_T>(readback_size)};
                std::uint8_t *mapped = nullptr;
                result = readback->Map(0, &read_range, reinterpret_cast<void **>(&mapped));
                if (FAILED(result))
                {
                    readback->Release();
                    return false;
                }
                for (UINT row = 0; row < row_count; ++row)
                {
                    std::memcpy(pixels + static_cast<std::size_t>(row) * width * 4,
                                mapped + footprint.Offset +
                                    static_cast<std::size_t>(row) * footprint.Footprint.RowPitch,
                                static_cast<std::size_t>(row_size));
                }
                const D3D12_RANGE written_range{0, 0};
                readback->Unmap(0, &written_range);
                readback->Release();
                return true;
            }

            bool ensure_frame()
            {
                if (!context_.frame_open() && !context_.begin_frame())
                {
                    last_error_ = "Unable to begin the D3D12 frame.";
                    return false;
                }
                if (active_render_target_ != 0)
                    return bind_active_render_target();
                return true;
            }

            bool bind_active_render_target()
            {
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end())
                    return false;
                const auto texture = textures_.find(target->second.texture);
                if (texture == textures_.end())
                    return false;
                if (target->second.samples == 1 && texture->second.state != D3D12_RESOURCE_STATE_RENDER_TARGET)
                {
                    transition_resource(context_.command_list(), texture->second.resource,
                                        texture->second.state, D3D12_RESOURCE_STATE_RENDER_TARGET);
                    texture->second.state = D3D12_RESOURCE_STATE_RENDER_TARGET;
                }
                D3D12_CPU_DESCRIPTOR_HANDLE handle =
                    cpu_descriptor(render_target_heap_, render_target_descriptor_size_,
                                   target->second.descriptor);
                D3D12_CPU_DESCRIPTOR_HANDLE depth_handle{};
                const bool has_depth = target->second.depth != nullptr;
                if (has_depth)
                    depth_handle = cpu_descriptor(depth_stencil_heap_, depth_stencil_descriptor_size_,
                                                  target->second.depth_descriptor);
                context_.command_list()->OMSetRenderTargets(1, &handle, FALSE,
                                                            has_depth ? &depth_handle : nullptr);
                D3D12_VIEWPORT viewport{0.0f, 0.0f, static_cast<float>(target->second.width),
                                        static_cast<float>(target->second.height), 0.0f, 1.0f};
                D3D12_RECT scissor{0, 0, target->second.width, target->second.height};
                context_.command_list()->RSSetViewports(1, &viewport);
                context_.command_list()->RSSetScissorRects(1, &scissor);
                return true;
            }

            void rebind_frame_targets()
            {
                if (active_render_target_ != 0)
                {
                    bind_active_render_target();
                    return;
                }
                context_.bind_backbuffer();
                if (last_2d_width_ > 0 && last_2d_height_ > 0)
                {
                    D3D12_VIEWPORT viewport{0.0f, 0.0f, static_cast<float>(last_2d_width_),
                                            static_cast<float>(last_2d_height_), 0.0f, 1.0f};
                    D3D12_RECT scissor{0, 0, last_2d_width_, last_2d_height_};
                    context_.command_list()->RSSetViewports(1, &viewport);
                    context_.command_list()->RSSetScissorRects(1, &scissor);
                }
            }

            void suspend_active_render_target()
            {
                if (active_render_target_ == 0)
                    return;
                const auto target = render_targets_.find(active_render_target_);
                if (target == render_targets_.end())
                    return;
                const auto texture = textures_.find(target->second.texture);
                if (texture == textures_.end())
                    return;
                if (target->second.samples > 1)
                {
                    resolve_render_target(target->second, texture->second);
                    return;
                }
                if (texture->second.state == D3D12_RESOURCE_STATE_RENDER_TARGET)
                {
                    transition_resource(context_.command_list(), texture->second.resource,
                                        D3D12_RESOURCE_STATE_RENDER_TARGET,
                                        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                    texture->second.state = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
                }
            }

            bool ensure_descriptor_heaps()
            {
                if (srv_heap_ && sampler_heap_ && srv_staging_heap_ && sampler_staging_heap_)
                    return true;
                D3D12_DESCRIPTOR_HEAP_DESC desc{};
                desc.NumDescriptors = descriptor_capacity + transient_descriptor_capacity;
                desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
                HRESULT result = context_.device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srv_heap_));
                if (FAILED(result))
                    return false;
                desc.NumDescriptors = persistent_sampler_capacity + transient_sampler_capacity;
                desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
                result = context_.device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&sampler_heap_));
                if (FAILED(result))
                {
                    srv_heap_->Release();
                    srv_heap_ = nullptr;
                    return false;
                }
                D3D12_DESCRIPTOR_HEAP_DESC staging{};
                staging.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                staging.NumDescriptors = descriptor_capacity;
                staging.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                result = context_.device()->CreateDescriptorHeap(&staging, IID_PPV_ARGS(&srv_staging_heap_));
                if (SUCCEEDED(result))
                {
                    staging.NumDescriptors = persistent_sampler_capacity;
                    staging.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
                    result = context_.device()->CreateDescriptorHeap(&staging, IID_PPV_ARGS(&sampler_staging_heap_));
                }
                if (FAILED(result))
                {
                    for (ID3D12DescriptorHeap **heap : {&srv_heap_, &sampler_heap_, &srv_staging_heap_, &sampler_staging_heap_})
                    {
                        if (*heap)
                            (*heap)->Release();
                        *heap = nullptr;
                    }
                    return false;
                }
                srv_descriptor_size_ =
                    context_.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                sampler_descriptor_size_ =
                    context_.device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
                return true;
            }

            std::uint32_t acquire_descriptor()
            {
                if (!free_descriptors_.empty())
                {
                    const std::uint32_t descriptor = free_descriptors_.back();
                    free_descriptors_.pop_back();
                    return descriptor;
                }
                if (next_descriptor_ >= descriptor_capacity)
                    return descriptor_capacity;
                return next_descriptor_++;
            }

            void release_texture(Texture &texture)
            {
                if (texture.resource)
                    texture.resource->Release();
                free_descriptors_.push_back(texture.descriptor);
                texture = {};
            }

            void release_retired_textures()
            {
                for (auto &texture : retired_textures_)
                    release_texture(texture);
                retired_textures_.clear();
                for (IUnknown *object : retired_objects_)
                    object->Release();
                retired_objects_.clear();
            }

            static D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor(ID3D12DescriptorHeap *heap,
                                                               std::uint32_t increment,
                                                               std::uint32_t index)
            {
                D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
                handle.ptr += static_cast<SIZE_T>(increment) * index;
                return handle;
            }

            static D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor(ID3D12DescriptorHeap *heap,
                                                               std::uint32_t increment,
                                                               std::uint32_t index)
            {
                D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
                handle.ptr += static_cast<UINT64>(increment) * index;
                return handle;
            }

            bool create_storage_resource(std::size_t size, StorageBuffer &buffer)
            {
                const std::size_t aligned = (size + 15) & ~static_cast<std::size_t>(15);
                D3D12_RESOURCE_DESC desc = buffer_description(aligned);
                desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
                const D3D12_HEAP_PROPERTIES default_heap = heap_properties(D3D12_HEAP_TYPE_DEFAULT);
                ID3D12Resource *resource = nullptr;
                if (FAILED(context_.device()->CreateCommittedResource(
                        &default_heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                        nullptr, IID_PPV_ARGS(&resource))))
                {
                    last_error_ = "Unable to create the D3D12 storage buffer.";
                    return false;
                }
                buffer.resource = resource;
                buffer.size = aligned;
                return true;
            }

            void release_program(ShaderProgram &program)
            {
                for (ID3DBlob *blob : {program.vertex_blob, program.pixel_blob, program.compute_blob})
                {
                    if (blob)
                        blob->Release();
                }
                for (ID3D12PipelineState *pipeline : program.pipelines)
                {
                    if (pipeline)
                        pipeline->Release();
                }
                if (program.compute_pipeline)
                    program.compute_pipeline->Release();
                if (program.root_signature)
                    program.root_signature->Release();
                program = {};
            }

            void retire_program(ShaderProgram &program)
            {
                if (!context_.frame_open())
                {
                    context_.wait_idle();
                    release_program(program);
                    return;
                }
                for (ID3D12PipelineState *pipeline : program.pipelines)
                {
                    if (pipeline)
                        retired_objects_.push_back(pipeline);
                }
                if (program.compute_pipeline)
                    retired_objects_.push_back(program.compute_pipeline);
                if (program.root_signature)
                    retired_objects_.push_back(program.root_signature);
                program.pipelines = {};
                program.compute_pipeline = nullptr;
                program.root_signature = nullptr;
                release_program(program);
            }

            bool store_program(ShaderProgram &program, std::uint32_t &handle)
            {
                program.vertex_constants.assign((std::max)(static_cast<std::size_t>(program.vertex.uniform_bytes), std::size_t{16}), 0);
                program.pixel_constants.assign((std::max)(static_cast<std::size_t>(program.pixel.uniform_bytes), std::size_t{16}), 0);
                program.sampler_units.assign(program.samplers.size(), 0);
                handle = next_program_++;
                if (handle == 0)
                    handle = next_program_++;
                programs_.emplace(handle, program);
                return true;
            }

            bool create_program_root_signature(ShaderProgram &program, std::string &error)
            {
                const D3D12_SHADER_VISIBILITY vertex_visibility =
                    program.compute ? D3D12_SHADER_VISIBILITY_ALL : D3D12_SHADER_VISIBILITY_VERTEX;
                const D3D12_SHADER_VISIBILITY pixel_visibility =
                    program.compute ? D3D12_SHADER_VISIBILITY_ALL : D3D12_SHADER_VISIBILITY_PIXEL;
                const std::size_t sampler_count = program.samplers.size();
                std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
                ranges.reserve(1 + sampler_count * 2);
                std::vector<D3D12_ROOT_PARAMETER> parameters;

                auto add_constants = [&](std::uint32_t reg, D3D12_SHADER_VISIBILITY visibility) -> int
                {
                    D3D12_ROOT_PARAMETER parameter{};
                    parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                    parameter.Descriptor.ShaderRegister = reg;
                    parameter.ShaderVisibility = visibility;
                    parameters.push_back(parameter);
                    return static_cast<int>(parameters.size() - 1);
                };
                auto add_table = [&](D3D12_DESCRIPTOR_RANGE_TYPE type, UINT count, UINT reg,
                                     D3D12_SHADER_VISIBILITY visibility, UINT space = 0) -> int
                {
                    D3D12_DESCRIPTOR_RANGE range{};
                    range.RangeType = type;
                    range.NumDescriptors = count;
                    range.BaseShaderRegister = reg;
                    range.RegisterSpace = space;
                    range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
                    ranges.push_back(range);
                    D3D12_ROOT_PARAMETER parameter{};
                    parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                    parameter.DescriptorTable.NumDescriptorRanges = 1;
                    parameter.DescriptorTable.pDescriptorRanges = nullptr;
                    parameter.ShaderVisibility = visibility;
                    parameters.push_back(parameter);
                    return static_cast<int>(parameters.size() - 1);
                };

                if (!program.compute && program.vertex.uniform_bytes > 0)
                    program.vertex_constants_parameter =
                        add_constants(translated_vertex_constants_register, vertex_visibility);
                if (program.pixel.uniform_bytes > 0)
                    program.pixel_constants_parameter =
                        add_constants(translated_pixel_constants_register, pixel_visibility);
                if (program.storage_registers > 0)
                    program.storage_parameter = add_table(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, program.storage_registers,
                                                          0, D3D12_SHADER_VISIBILITY_ALL);
                if (program.image_registers > 0)
                    program.image_parameter = add_table(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, program.image_registers,
                                                        translated_image_base_register, D3D12_SHADER_VISIBILITY_ALL);
                if (sampler_count > 0)
                {
                    program.texture_parameter_base =
                        add_table(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, static_cast<UINT>(sampler_count), 0,
                                  D3D12_SHADER_VISIBILITY_ALL);
                    program.sampler_parameter_base =
                        add_table(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, static_cast<UINT>(sampler_count), 0,
                                  D3D12_SHADER_VISIBILITY_ALL);
                }
                // Descriptor tables point at ranges stored in order; patch the pointers now that the vector is final.
                std::size_t range_index = 0;
                for (D3D12_ROOT_PARAMETER &parameter : parameters)
                {
                    if (parameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
                        parameter.DescriptorTable.pDescriptorRanges = &ranges[range_index++];
                }

                D3D12_ROOT_SIGNATURE_DESC signature_desc{};
                signature_desc.NumParameters = static_cast<UINT>(parameters.size());
                signature_desc.pParameters = parameters.data();
                signature_desc.Flags = program.compute ? D3D12_ROOT_SIGNATURE_FLAG_NONE
                                                       : D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
                ID3DBlob *serialized = nullptr;
                ID3DBlob *messages = nullptr;
                HRESULT result = D3D12SerializeRootSignature(&signature_desc, D3D_ROOT_SIGNATURE_VERSION_1,
                                                             &serialized, &messages);
                if (FAILED(result))
                {
                    error = messages ? static_cast<const char *>(messages->GetBufferPointer())
                                     : "Unable to serialize the D3D12 shader root signature.";
                    if (messages)
                        messages->Release();
                    return false;
                }
                if (messages)
                    messages->Release();
                result = context_.device()->CreateRootSignature(0, serialized->GetBufferPointer(),
                                                                serialized->GetBufferSize(),
                                                                IID_PPV_ARGS(&program.root_signature));
                serialized->Release();
                if (FAILED(result))
                {
                    error = "Unable to create the D3D12 shader root signature.";
                    return false;
                }
                return true;
            }

            bool set_program_uniform(ShaderProgram &program, const char *name, const void *data, std::size_t size)
            {
                if (!name || !data)
                    return false;
                flush_if_active(program);
                bool found = false;
                const auto vertex = program.vertex.uniforms.find(name);
                if (vertex != program.vertex.uniforms.end())
                {
                    std::memcpy(program.vertex_constants.data() + vertex->second.offset, data,
                                (std::min<std::size_t>)(size, vertex->second.size));
                    found = true;
                }
                const auto pixel = program.pixel.uniforms.find(name);
                if (pixel != program.pixel.uniforms.end())
                {
                    std::memcpy(program.pixel_constants.data() + pixel->second.offset, data,
                                (std::min<std::size_t>)(size, pixel->second.size));
                    found = true;
                }
                return found;
            }

            bool set_sampler_unit(ShaderProgram &program, const char *name, int unit)
            {
                if (!name)
                    return false;
                const auto &samplers = program.samplers;
                const auto found = std::find(samplers.begin(), samplers.end(), std::string(name));
                if (found == samplers.end())
                    return false;
                flush_if_active(program);
                program.sampler_units[static_cast<std::size_t>(found - samplers.begin())] = unit;
                return true;
            }

            void flush_if_active(const ShaderProgram &program)
            {
                if (active_program_ == 0 || batch_vertices_.empty())
                    return;
                const auto active = programs_.find(active_program_);
                if (active != programs_.end() && &active->second == &program)
                    flush_2d();
            }

            D3D12_GPU_VIRTUAL_ADDRESS allocate_constants(const void *data, std::size_t size)
            {
                if (!constant_buffer_)
                {
                    const D3D12_HEAP_PROPERTIES upload_heap = heap_properties(D3D12_HEAP_TYPE_UPLOAD);
                    const D3D12_RESOURCE_DESC desc = buffer_description(constant_buffer_bytes);
                    if (FAILED(context_.device()->CreateCommittedResource(
                            &upload_heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
                            nullptr, IID_PPV_ARGS(&constant_buffer_))))
                        return 0;
                    const D3D12_RANGE no_read{0, 0};
                    if (FAILED(constant_buffer_->Map(0, &no_read, reinterpret_cast<void **>(&mapped_constants_))))
                    {
                        constant_buffer_->Release();
                        constant_buffer_ = nullptr;
                        return 0;
                    }
                }
                if (constant_serial_ != context_.frame_serial())
                {
                    constant_serial_ = context_.frame_serial();
                    constant_offset_ = 0;
                }
                const std::size_t aligned = (((std::max<std::size_t>)(size, 16)) + 255) & ~static_cast<std::size_t>(255);
                if (constant_offset_ + aligned > constant_buffer_bytes)
                {
                    last_error_ = "The D3D12 shader constant buffer is full for this frame.";
                    return 0;
                }
                std::memcpy(mapped_constants_ + constant_offset_, data, size);
                const D3D12_GPU_VIRTUAL_ADDRESS address = constant_buffer_->GetGPUVirtualAddress() + constant_offset_;
                constant_offset_ += aligned;
                return address;
            }

            std::uint32_t allocate_transient_descriptors(std::uint32_t count)
            {
                if (transient_serial_ != context_.frame_serial())
                {
                    transient_serial_ = context_.frame_serial();
                    transient_offset_ = 0;
                }
                if (transient_offset_ + count > transient_descriptor_capacity)
                    return UINT_MAX;
                const std::uint32_t base = descriptor_capacity + transient_offset_;
                transient_offset_ += count;
                return base;
            }

            std::uint32_t allocate_transient_samplers(std::uint32_t count)
            {
                if (transient_sampler_serial_ != context_.frame_serial())
                {
                    transient_sampler_serial_ = context_.frame_serial();
                    transient_sampler_offset_ = 0;
                }
                if (transient_sampler_offset_ + count > transient_sampler_capacity)
                    return UINT_MAX;
                const std::uint32_t base = persistent_sampler_capacity + transient_sampler_offset_;
                transient_sampler_offset_ += count;
                return base;
            }

            // Returns image textures bound for load/store to their previous resource state.
            void restore_image_textures()
            {
                if (image_restores_.empty())
                    return;
                auto *commands = context_.command_list();
                for (const ImageRestore &entry : image_restores_)
                {
                    const auto texture = textures_.find(entry.texture);
                    if (texture == textures_.end() || texture->second.resource != entry.resource)
                        continue;
                    D3D12_RESOURCE_BARRIER barrier{};
                    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
                    barrier.UAV.pResource = entry.resource;
                    commands->ResourceBarrier(1, &barrier);
                    if (entry.state != D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
                        transition_resource(commands, entry.resource, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                                            entry.state);
                    texture->second.state = entry.state;
                }
                image_restores_.clear();
            }

            bool bind_program_resources(ShaderProgram &program, bool graphics)
            {
                auto *commands = context_.command_list();
                auto set_table = [&](int parameter, D3D12_GPU_DESCRIPTOR_HANDLE handle)
                {
                    if (graphics)
                        commands->SetGraphicsRootDescriptorTable(static_cast<UINT>(parameter), handle);
                    else
                        commands->SetComputeRootDescriptorTable(static_cast<UINT>(parameter), handle);
                };
                if (program.storage_parameter >= 0)
                {
                    const std::uint32_t base = allocate_transient_descriptors(program.storage_registers);
                    if (base == UINT_MAX)
                    {
                        last_error_ = "The D3D12 transient descriptor heap is full for this frame.";
                        return false;
                    }
                    for (std::uint32_t index = 0; index < program.storage_registers; ++index)
                    {
                        D3D12_UNORDERED_ACCESS_VIEW_DESC view{};
                        view.Format = DXGI_FORMAT_R32_TYPELESS;
                        view.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                        view.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
                        view.Buffer.NumElements = 1;
                        ID3D12Resource *resource = nullptr;
                        if (index < max_storage_bindings)
                        {
                            const auto buffer = storage_buffers_.find(bound_storage_buffers_[index]);
                            if (buffer != storage_buffers_.end())
                            {
                                resource = buffer->second.resource;
                                view.Buffer.NumElements = static_cast<UINT>(buffer->second.size / 4);
                            }
                        }
                        context_.device()->CreateUnorderedAccessView(
                            resource, nullptr, &view, cpu_descriptor(srv_heap_, srv_descriptor_size_, base + index));
                    }
                    set_table(program.storage_parameter, gpu_descriptor(srv_heap_, srv_descriptor_size_, base));
                }
                if (program.image_parameter >= 0)
                {
                    const std::uint32_t base = allocate_transient_descriptors(program.image_registers);
                    if (base == UINT_MAX)
                    {
                        last_error_ = "The D3D12 transient descriptor heap is full for this frame.";
                        return false;
                    }
                    for (std::uint32_t index = 0; index < program.image_registers; ++index)
                    {
                        D3D12_UNORDERED_ACCESS_VIEW_DESC view{};
                        view.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                        view.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                        ID3D12Resource *resource = nullptr;
                        const auto texture = textures_.find(bound_image_units_[index]);
                        if (texture != textures_.end())
                        {
                            resource = texture->second.resource;
                            const bool pending = std::any_of(
                                image_restores_.begin(), image_restores_.end(),
                                [&](const ImageRestore &entry) { return entry.resource == resource; });
                            if (!pending)
                            {
                                image_restores_.push_back({resource, texture->second.state, texture->first});
                                if (texture->second.state != D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
                                    transition_resource(commands, resource, texture->second.state,
                                                        D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                                texture->second.state = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
                            }
                        }
                        context_.device()->CreateUnorderedAccessView(
                            resource, nullptr, &view, cpu_descriptor(srv_heap_, srv_descriptor_size_, base + index));
                    }
                    set_table(program.image_parameter, gpu_descriptor(srv_heap_, srv_descriptor_size_, base));
                }
                if (!program.samplers.empty())
                {
                    const std::uint32_t count = static_cast<std::uint32_t>(program.samplers.size());
                    const std::uint32_t texture_base = allocate_transient_descriptors(count);
                    const std::uint32_t sampler_base = allocate_transient_samplers(count);
                    if (texture_base == UINT_MAX || sampler_base == UINT_MAX)
                    {
                        last_error_ = "The D3D12 transient descriptor heap is full for this frame.";
                        return false;
                    }
                    for (std::uint32_t slot = 0; slot < count; ++slot)
                    {
                        const int unit = program.sampler_units[slot];
                        std::uint32_t handle = 0;
                        if (graphics && unit == 0)
                            handle = batch_texture_;
                        else if (unit >= 0 && static_cast<std::uint32_t>(unit) < max_texture_units)
                            handle = bound_texture_units_[unit];
                        auto texture = textures_.find(handle != 0 ? handle : white_texture_);
                        if (texture == textures_.end() ||
                            texture->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                            texture = textures_.find(white_texture_);
                        if (texture == textures_.end())
                            return false;
                        context_.device()->CopyDescriptorsSimple(
                            1, cpu_descriptor(srv_heap_, srv_descriptor_size_, texture_base + slot),
                            cpu_descriptor(srv_staging_heap_, srv_descriptor_size_, texture->second.descriptor),
                            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                        context_.device()->CopyDescriptorsSimple(
                            1, cpu_descriptor(sampler_heap_, sampler_descriptor_size_, sampler_base + slot),
                            cpu_descriptor(sampler_staging_heap_, sampler_descriptor_size_, texture->second.sampler),
                            D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
                    }
                    set_table(program.texture_parameter_base,
                              gpu_descriptor(srv_heap_, srv_descriptor_size_, texture_base));
                    set_table(program.sampler_parameter_base,
                              gpu_descriptor(sampler_heap_, sampler_descriptor_size_, sampler_base));
                }
                return true;
            }

            static D3D12_GRAPHICS_PIPELINE_STATE_DESC make_2d_pipeline_desc(
                ID3D12RootSignature *root_signature, ID3DBlob *vertex, ID3DBlob *pixel, std::size_t primitive,
                std::size_t variant, const D3D12_INPUT_ELEMENT_DESC *elements, UINT element_count, int samples)
            {
                D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
                desc.pRootSignature = root_signature;
                desc.VS = {vertex->GetBufferPointer(), vertex->GetBufferSize()};
                desc.PS = {pixel->GetBufferPointer(), pixel->GetBufferSize()};
                desc.BlendState = blend_description(variant);
                desc.SampleMask = UINT_MAX;
                desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
                desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
                desc.RasterizerState.DepthClipEnable = TRUE;
                desc.DepthStencilState.DepthEnable = FALSE;
                desc.DepthStencilState.StencilEnable = FALSE;
                desc.InputLayout = {elements, element_count};
                desc.PrimitiveTopologyType = primitive == 1   ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE
                                             : primitive == 2 ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT
                                                              : D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                desc.NumRenderTargets = 1;
                desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
                desc.SampleDesc.Count = static_cast<UINT>(samples);
                return desc;
            }

            ID3D12PipelineState *get_default_pipeline(std::size_t primitive, std::size_t variant)
            {
                const int samples = current_target_samples();
                ID3D12PipelineState *&slot =
                    pipelines_[sample_slot(samples) * pipeline_set_size + variant * primitive_pipeline_count + primitive];
                if (slot)
                    return slot;
                static const D3D12_INPUT_ELEMENT_DESC elements[] = {
                    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex2D, x)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex2D, u)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex2D, r)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};
                const D3D12_GRAPHICS_PIPELINE_STATE_DESC desc =
                    make_2d_pipeline_desc(root_signature_, default_vertex_blob_, default_pixel_blob_, primitive,
                                          variant, elements, static_cast<UINT>(std::size(elements)), samples);
                if (!create_pipeline_state(desc, slot))
                    slot = nullptr;
                return slot;
            }

            ID3D12PipelineState *get_program_pipeline(ShaderProgram &program, std::size_t primitive,
                                                      std::size_t variant)
            {
                const int samples = current_target_samples();
                ID3D12PipelineState *&slot =
                    program.pipelines[sample_slot(samples) * pipeline_set_size + variant * primitive_pipeline_count + primitive];
                if (slot)
                    return slot;
                // The translated vertex shaders read their inputs from locations 0 (position), 1 (uv), 2 (colour).
                static const D3D12_INPUT_ELEMENT_DESC elements[] = {
                    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex2D, x)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex2D, u)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
                     static_cast<UINT>(offsetof(Vertex2D, r)), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};
                D3D12_GRAPHICS_PIPELINE_STATE_DESC desc =
                    make_2d_pipeline_desc(program.root_signature, program.vertex_blob, program.pixel_blob,
                                          primitive, variant, elements, static_cast<UINT>(std::size(elements)), samples);
                if (!create_pipeline_state(desc, slot))
                    trace("graphics pipeline for user shader FAILED (dxc=%d)", dxc_available(context_.device()) ? 1 : 0);
                if (!slot && !program.fxc_fallback && dxc_available(context_.device()) &&
                    program.samplers.size() <= max_program_samplers_sm5)
                {
                    ID3DBlob *vertex_fallback = nullptr;
                    ID3DBlob *pixel_fallback = nullptr;
                    std::string fallback_error;
                    if (compile_program_shader_fxc(program.vertex.hlsl, "vs", &vertex_fallback, fallback_error) &&
                        compile_program_shader_fxc(program.pixel.hlsl, "ps", &pixel_fallback, fallback_error))
                    {
                        // Pipelines already built keep their own copy of the old bytecode.
                        program.vertex_blob->Release();
                        program.pixel_blob->Release();
                        program.vertex_blob = vertex_fallback;
                        program.pixel_blob = pixel_fallback;
                        program.fxc_fallback = true;
                        desc = make_2d_pipeline_desc(program.root_signature, program.vertex_blob, program.pixel_blob,
                                                     primitive, variant, elements, static_cast<UINT>(std::size(elements)),
                                                     samples);
                        create_pipeline_state(desc, slot);
                    }
                    else
                    {
                        if (vertex_fallback)
                            vertex_fallback->Release();
                        if (pixel_fallback)
                            pixel_fallback->Release();
                    }
                }
                if (!slot)
                    last_error_ = "Unable to create the D3D12 pipeline state for a user shader.";
                return slot;
            }

            bool bind_default_for_draw(std::size_t primitive, std::size_t variant)
            {
                auto texture = textures_.find(batch_texture_ != 0 ? batch_texture_ : white_texture_);
                if (texture == textures_.end() ||
                    texture->second.state != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
                    texture = textures_.find(white_texture_);
                ID3D12PipelineState *pipeline = get_default_pipeline(primitive, variant);
                if (texture == textures_.end() || !pipeline)
                    return false;
                auto *commands = context_.command_list();
                commands->SetGraphicsRootSignature(root_signature_);
                ID3D12DescriptorHeap *heaps[] = {srv_heap_, sampler_heap_};
                commands->SetDescriptorHeaps(2, heaps);
                commands->SetGraphicsRoot32BitConstants(0, 16, projection_.data(), 0);
                commands->SetPipelineState(pipeline);
                commands->SetGraphicsRootDescriptorTable(
                    1, gpu_descriptor(srv_heap_, srv_descriptor_size_, texture->second.descriptor));
                commands->SetGraphicsRootDescriptorTable(
                    2, gpu_descriptor(sampler_heap_, sampler_descriptor_size_, texture->second.sampler));
                return true;
            }

            bool bind_program_for_draw(ShaderProgram &program, std::size_t primitive, std::size_t variant)
            {
                ID3D12PipelineState *pipeline = get_program_pipeline(program, primitive, variant);
                if (!pipeline)
                    return false;
                auto *commands = context_.command_list();
                commands->SetGraphicsRootSignature(program.root_signature);
                ID3D12DescriptorHeap *heaps[] = {srv_heap_, sampler_heap_};
                commands->SetDescriptorHeaps(2, heaps);
                commands->SetPipelineState(pipeline);
                if (program.vertex_constants_parameter >= 0)
                {
                    const D3D12_GPU_VIRTUAL_ADDRESS address =
                        allocate_constants(program.vertex_constants.data(), program.vertex_constants.size());
                    if (!address)
                        return false;
                    commands->SetGraphicsRootConstantBufferView(
                        static_cast<UINT>(program.vertex_constants_parameter), address);
                }
                if (program.pixel_constants_parameter >= 0)
                {
                    const D3D12_GPU_VIRTUAL_ADDRESS address =
                        allocate_constants(program.pixel_constants.data(), program.pixel_constants.size());
                    if (!address)
                        return false;
                    commands->SetGraphicsRootConstantBufferView(
                        static_cast<UINT>(program.pixel_constants_parameter), address);
                }
                return bind_program_resources(program, true);
            }

            bool unsupported()
            {
                last_error_ = "This D3D12 renderer feature is not implemented yet.";
                return false;
            }

            SDL_Window *window_ = nullptr;
            D3D12Context context_;
#ifdef SIMLIB_GUI_IMGUI
            static constexpr UINT imgui_descriptor_count = 64;
            ID3D12DescriptorHeap *imgui_heap_ = nullptr;
            UINT imgui_descriptor_size_ = 0;
            std::vector<UINT> imgui_free_;
            bool imgui_ready_ = false;
#endif
            std::string last_error_;
            bool vsync_enabled_ = true;
            bool initialized_2d_ = false;
            bool shutdown_2d_pending_ = false;
            ID3D12DescriptorHeap *srv_heap_ = nullptr;
            ID3D12DescriptorHeap *srv_staging_heap_ = nullptr;
            ID3D12DescriptorHeap *sampler_staging_heap_ = nullptr;
            ID3D12DescriptorHeap *sampler_heap_ = nullptr;
            std::unordered_map<std::uint32_t, std::uint32_t> sampler_slots_;
            ID3D12DescriptorHeap *render_target_heap_ = nullptr;
            ID3D12DescriptorHeap *depth_stencil_heap_ = nullptr;
            ID3D12RootSignature *root_signature_ = nullptr;
            std::array<ID3D12PipelineState *, pipeline_set_size * sample_slot_count> pipelines_{};
            ID3DBlob *default_vertex_blob_ = nullptr;
            ID3DBlob *default_pixel_blob_ = nullptr;
            std::unordered_map<std::uint32_t, ShaderProgram> programs_;
            std::unordered_map<std::uint32_t, StorageBuffer> storage_buffers_;
            std::uint32_t next_program_ = 1;
            std::uint32_t next_storage_buffer_ = 1;
            std::uint32_t active_program_ = 0;
            std::array<std::uint32_t, max_storage_bindings> bound_storage_buffers_{};
            std::array<std::uint32_t, max_texture_units> bound_texture_units_{};
            std::vector<std::uint32_t> render_target_stack_;
            std::array<std::uint32_t, max_image_units> bound_image_units_{};
            struct ImageRestore
            {
                ID3D12Resource *resource;
                D3D12_RESOURCE_STATES state;
                std::uint32_t texture;
            };
            std::vector<ImageRestore> image_restores_;
            std::vector<IUnknown *> retired_objects_;
            ID3D12Resource *constant_buffer_ = nullptr;
            std::uint8_t *mapped_constants_ = nullptr;
            std::size_t constant_offset_ = 0;
            std::uint64_t constant_serial_ = 0;
            std::uint32_t transient_offset_ = 0;
            std::uint32_t transient_sampler_offset_ = 0;
            std::uint64_t transient_serial_ = 0;
            std::uint64_t transient_sampler_serial_ = 0;
            BlendMode blend_mode_ = BlendMode::normal;
            bool premultiplied_alpha_ = false;
            ID3D12RootSignature *mesh_root_signature_ = nullptr;
            std::array<ID3D12PipelineState *, 18 * sample_slot_count> mesh_pipelines_{}; // The second half is wireframe.
            std::array<ID3D12PipelineState *, 18 * sample_slot_count> mesh_instanced_pipelines_{};
            std::uint64_t pipelines_created_ = 0;
            std::vector<QueuedMeshDraw> queued_draws_;
            std::size_t inline_mesh_run_ = 0;
            std::vector<RecordWorker> workers_;
            std::size_t workers_used_ = 0;
            std::uint64_t worker_serial_ = 0;
            ID3DBlob *mesh_instanced_vertex_blob_ = nullptr;
            std::unordered_map<std::uint32_t, D3D12InstanceBuffer> instance_buffers_;
            std::uint32_t next_instance_buffer_ = 1;
            ID3DBlob *mesh_vertex_blob_ = nullptr;
            ID3DBlob *mesh_pixel_blob_ = nullptr;
            ID3D12RootSignature *sky_root_signature_ = nullptr;
            std::array<ID3D12PipelineState *, sample_slot_count> sky_pipelines_{};
            ID3DBlob *sky_vertex_blob_ = nullptr;
            ID3DBlob *sky_pixel_blob_ = nullptr;
            ID3D12Resource *vertex_buffer_ = nullptr;
            Vertex2D *mapped_vertices_ = nullptr;
            std::size_t vertex_capacity_ = 0;
            std::size_t vertex_offset_ = 0;
            std::uint64_t vertex_frame_serial_ = 0;
            std::uint32_t srv_descriptor_size_ = 0;
            std::uint32_t sampler_descriptor_size_ = 0;
            std::uint32_t render_target_descriptor_size_ = 0;
            std::uint32_t depth_stencil_descriptor_size_ = 0;
            std::uint32_t next_texture_ = 1;
            std::uint32_t next_descriptor_ = 0;
            std::uint32_t next_render_target_ = 1;
            std::uint32_t next_render_target_descriptor_ = 0;
            std::uint32_t next_depth_stencil_descriptor_ = 0;
            std::uint32_t next_mesh_ = 1;
            std::uint32_t white_texture_ = 0;
            std::uint32_t active_render_target_ = 0;
            std::array<float, 16> projection_{};
            ClipRect2D clip_;
            int last_2d_width_ = 0;
            int last_2d_height_ = 0;
            std::vector<std::uint32_t> free_descriptors_;
            std::vector<std::uint32_t> free_render_target_descriptors_;
            std::vector<std::uint32_t> free_depth_stencil_descriptors_;
            std::unordered_map<std::uint32_t, Texture> textures_;
            std::unordered_map<std::uint32_t, RenderTarget> render_targets_;
            std::unordered_map<std::uint32_t, Mesh> meshes_;
            std::vector<Texture> retired_textures_;
            std::vector<Vertex2D> batch_vertices_;
            PrimitiveType batch_primitive_ = PrimitiveType::triangles;
            std::uint32_t batch_texture_ = 0;
        };
    }

    std::unique_ptr<Renderer> create_d3d12_renderer()
    {
        return std::make_unique<D3D12Renderer>();
    }
}

#endif // _WIN32
