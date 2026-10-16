/** @file
 * @brief Implements Dear ImGui initialization, event handling, and rendering.
 */

#include "gui.h"

#include "display.h"
#include "gl2d.h"
#include "renderer.h"
#include "vulkan_context.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_vulkan.h>

namespace sl
{
    namespace
    {
        VkDescriptorPool vulkan_descriptor_pool = VK_NULL_HANDLE;
        bool vulkan_gui_ready = false;
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
        if (graphics_backend() == GraphicsBackend::opengl && get_gl_context())
        {
            ImGui_ImplSDL2_InitForOpenGL(get_window(), get_gl_context());
            ImGui_ImplOpenGL3_Init("#version 430 core");
        }
        else if (graphics_backend() == GraphicsBackend::vulkan)
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
    }

    void gui_handle_event(const Event &event)
    {
        ImGui_ImplSDL2_ProcessEvent(static_cast<const SDL_Event *>(detail::event_handle(event)));
    }

    void gui_new_frame()
    {
        if (graphics_backend() == GraphicsBackend::opengl)
            ImGui_ImplOpenGL3_NewFrame();
        else if (graphics_backend() == GraphicsBackend::vulkan && vulkan_gui_ready)
            ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void gui_render()
    {
        detail::gl2d_flush();
        ImGui::Render();
        if (graphics_backend() == GraphicsBackend::opengl)
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        else if (graphics_backend() == GraphicsBackend::vulkan && vulkan_gui_ready)
        {
            if (detail::VulkanContext *context = detail::active_renderer()->vulkan_context())
                ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), context->command_buffer());
        }
    }

    void gui_shutdown()
    {
        if (graphics_backend() == GraphicsBackend::opengl)
            ImGui_ImplOpenGL3_Shutdown();
        else if (graphics_backend() == GraphicsBackend::vulkan && vulkan_gui_ready)
        {
            if (detail::VulkanContext *context = detail::active_renderer()->vulkan_context())
                vkDeviceWaitIdle(context->device());
            ImGui_ImplVulkan_Shutdown();
            if (detail::VulkanContext *context = detail::active_renderer()->vulkan_context())
                context->destroy_imgui_descriptor_pool(vulkan_descriptor_pool);
            vulkan_gui_ready = false;
        }
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
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
