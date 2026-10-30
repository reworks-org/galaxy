/** @file
 * @brief Implements Dear ImGui initialization, event handling, and rendering.
 */

#include "gui.h"

#include "display.h"
#include "gl2d.h"
#include "renderer.h"
#ifdef SIMLIB_RENDERER_VULKAN
#include "vulkan_context.h"
#endif

#ifdef SIMLIB_GUI_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#ifdef SIMLIB_RENDERER_GL
#include <backends/imgui_impl_opengl3.h>
#endif
#ifdef SIMLIB_RENDERER_VULKAN
#include <backends/imgui_impl_vulkan.h>
#endif
#endif

#ifndef SIMLIB_GUI_IMGUI
namespace sl
{
    // Built with SIMLIB_GUI=none: the GUI API is kept as no-ops so callers still link.
    void gui_init() {}
    void gui_handle_event(const Event &) {}
    void gui_new_frame() {}
    void gui_render() {}
    void gui_shutdown() {}
    void gui_show_demo_window() {}
    void gui_show_metrics_window(bool &) {}
} // namespace sl
#else
namespace sl
{
    namespace
    {
#ifdef SIMLIB_RENDERER_VULKAN
        VkDescriptorPool vulkan_descriptor_pool = VK_NULL_HANDLE;
        bool vulkan_gui_ready = false;
#endif
        bool d3d12_gui_ready = false;
        bool gui_ready = false;
    }

    void gui_init()
    {
        if (!get_window())
        {
            return;
        }
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
#ifdef SIMLIB_RENDERER_GL
        if (graphics_backend() == GraphicsBackend::opengl && get_gl_context())
        {
            ImGui_ImplSDL2_InitForOpenGL(get_window(), get_gl_context());
            ImGui_ImplOpenGL3_Init("#version 430 core");
        }
        else
#endif
#ifdef SIMLIB_RENDERER_VULKAN
        if (graphics_backend() == GraphicsBackend::vulkan)
        {
            detail::VulkanContext *context = detail::active_renderer()->vulkan_context();
            std::string error;
            if (!context || !context->create_imgui_descriptor_pool(vulkan_descriptor_pool, error))
            {
                ImGui::DestroyContext();
                return;
            }
            ImGui_ImplVulkan_InitInfo init_info{};
            init_info.ApiVersion = VK_API_VERSION_1_3;
            init_info.Instance = context->instance();
            init_info.PhysicalDevice = context->physical_device();
            init_info.Device = context->device();
            init_info.QueueFamily = context->graphics_queue_family();
            init_info.Queue = context->graphics_queue();
            init_info.DescriptorPool = vulkan_descriptor_pool;
            init_info.RenderPass = context->render_pass();
            init_info.MinImageCount = 2;
            init_info.ImageCount = context->swapchain_image_count();
            init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            vulkan_gui_ready = ImGui_ImplSDL2_InitForVulkan(get_window()) && ImGui_ImplVulkan_Init(&init_info);
            if (!vulkan_gui_ready)
            {
                ImGui_ImplSDL2_Shutdown();
                context->destroy_imgui_descriptor_pool(vulkan_descriptor_pool);
                ImGui::DestroyContext();
                return;
            }
            ImGui_ImplVulkan_CreateFontsTexture();
        }
        else
#endif
        if (graphics_backend() == GraphicsBackend::d3d12)
        {
            if (detail::active_renderer() && ImGui_ImplSDL2_InitForD3D(get_window()) &&
                detail::active_renderer()->gui_backend_init())
            {
                d3d12_gui_ready = true;
            }
            else
            {
                ImGui_ImplSDL2_Shutdown();
                ImGui::DestroyContext();
                return;
            }
        }
        gui_ready = true;
    }

    void gui_handle_event(const Event &event)
    {
        if (gui_ready)
            ImGui_ImplSDL2_ProcessEvent(static_cast<const SDL_Event *>(detail::event_handle(event)));
    }

    void gui_new_frame()
    {
        if (!gui_ready)
            return;
#ifdef SIMLIB_RENDERER_GL
        if (graphics_backend() == GraphicsBackend::opengl)
            ImGui_ImplOpenGL3_NewFrame();
        else
#endif
#ifdef SIMLIB_RENDERER_VULKAN
        if (graphics_backend() == GraphicsBackend::vulkan && vulkan_gui_ready)
            ImGui_ImplVulkan_NewFrame();
        else
#endif
        if (d3d12_gui_ready)
            detail::active_renderer()->gui_backend_new_frame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void gui_render()
    {
        if (!gui_ready)
            return;
        detail::gl2d_flush();
        ImGui::Render();
#ifdef SIMLIB_RENDERER_GL
        if (graphics_backend() == GraphicsBackend::opengl)
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        else
#endif
#ifdef SIMLIB_RENDERER_VULKAN
        if (graphics_backend() == GraphicsBackend::vulkan && vulkan_gui_ready)
        {
            if (detail::VulkanContext *context = detail::active_renderer()->vulkan_context())
                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), context->command_buffer());
        }
        else
#endif
        if (d3d12_gui_ready)
            detail::active_renderer()->gui_backend_render();
    }

    void gui_shutdown()
    {
        if (!gui_ready)
            return;
#ifdef SIMLIB_RENDERER_GL
        if (graphics_backend() == GraphicsBackend::opengl)
        {
            ImGui_ImplOpenGL3_Shutdown();
        }
        else
#endif
        if (d3d12_gui_ready)
        {
            detail::active_renderer()->gui_backend_shutdown();
            d3d12_gui_ready = false;
        }
#ifdef SIMLIB_RENDERER_VULKAN
        else if (graphics_backend() == GraphicsBackend::vulkan && vulkan_gui_ready)
        {
            if (detail::VulkanContext *context = detail::active_renderer()->vulkan_context())
                vkDeviceWaitIdle(context->device());
            ImGui_ImplVulkan_Shutdown();
            if (detail::VulkanContext *context = detail::active_renderer()->vulkan_context())
                context->destroy_imgui_descriptor_pool(vulkan_descriptor_pool);
            vulkan_gui_ready = false;
        }
#endif
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        gui_ready = false;
    }

    void gui_show_demo_window()
    {
        ImGui::ShowDemoWindow();
    }

    void gui_show_metrics_window(bool &show_metrics)
    {
         ImGui::ShowMetricsWindow(&show_metrics);
    }
} // namespace sl
#endif // SIMLIB_GUI_IMGUI
