/** @file
 * @brief Implements SDL window, OpenGL context, and display lifecycle functions.
 */

#include "display.h"
#include "async_internal.h"
#include "parallel_record.h"

#include "draw.h"
#include "font.h"
#include "error.h"
#include "renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_TTF/SDL_ttf.h>
#include <glad/glad.h>

#include <memory>
#include <string_view>

namespace sl
{

    extern Font *sl_default_monospace_font;

    namespace
    {

        SDL_Window *window = nullptr;
        std::unique_ptr<detail::Renderer> renderer;
#if defined(_WIN32) && defined(SIMLIB_RENDERER_D3D12)
        constexpr GraphicsBackend default_backend = GraphicsBackend::d3d12;
#elif defined(SIMLIB_RENDERER_GL)
        constexpr GraphicsBackend default_backend = GraphicsBackend::opengl;
#elif defined(SIMLIB_RENDERER_VULKAN)
        constexpr GraphicsBackend default_backend = GraphicsBackend::vulkan;
#else
#error "At least one renderer (SIMLIB_RENDERERS) must be enabled"
#endif
        GraphicsBackend selected_backend = default_backend;
        bool ttfInitialized = false;
        int width = 0;
        int height = 0;
        int logicalWidth = 0;
        int logicalHeight = 0;
        int renderTargetWidth = 0;
        int renderTargetHeight = 0;
        float screenOffsetX = 0.0f;
        float screenOffsetY = 0.0f;
    } // namespace

    bool set_graphics_backend(GraphicsBackend backend)
    {
        if (window)
        {
            sl::detail::set_error("Graphics backend cannot change while the display is active");
            return false;
        }
        if (!graphics_backend_available(backend))
        {
            sl::detail::set_error(std::string(backend == GraphicsBackend::opengl   ? "OpenGL"
                                              : backend == GraphicsBackend::vulkan ? "Vulkan"
                                                                                   : "Direct3D 12") +
                                  " is not available in this build (see SIMLIB_RENDERERS)");
            return false;
        }
        selected_backend = backend;
        return true;
    }

    bool configure_graphics_backend_from_args(int argc, char *argv[])
    {
        GraphicsBackend requested = default_backend;
        for (int index = 1; index < argc; ++index)
        {
            const std::string_view argument(argv[index]);
            if (argument == "--gl")
                requested = GraphicsBackend::opengl;
            else if (argument == "--vulkan")
                requested = GraphicsBackend::vulkan;
            else if (argument == "--d3d12")
                requested = GraphicsBackend::d3d12;
            else
            {
                std::fprintf(stderr, "Unknown graphics backend argument: %s\n", argument.data());
                return false;
            }
        }
        if (!set_graphics_backend(requested))
        {
            std::fprintf(stderr, "%s\n", sl::last_error().c_str());
            return false;
        }
        return true;
    }

    bool graphics_backend_available(GraphicsBackend backend)
    {
        switch (backend)
        {
        case GraphicsBackend::opengl:
#ifdef SIMLIB_RENDERER_GL
            return true;
#else
            return false;
#endif
        case GraphicsBackend::vulkan:
#ifdef SIMLIB_RENDERER_VULKAN
            return true;
#else
            return false;
#endif
        case GraphicsBackend::d3d12:
#if defined(_WIN32) && defined(SIMLIB_RENDERER_D3D12)
            return true;
#else
            return false;
#endif
        }
        return false;
    }

    GraphicsBackend graphics_backend()
    {
        return selected_backend;
    }

    std::string graphics_backend_name()
    {
        switch (selected_backend)
        {
        case GraphicsBackend::opengl:
            return "OpenGL";
        case GraphicsBackend::vulkan:
            return "Vulkan";
        case GraphicsBackend::d3d12:
            return "D3D12";
        }
        return "Unknown";
    }

    bool set_gfx_mode(const char* title, SDL_WindowFlags windowflags, int driver, int requestedWidth, int requestedHeight, int virtualWidth, int virtualHeight)
    {
        const bool use_desktop_resolution =
            driver == GFX_AUTODETECT_FULLSCREEN && requestedWidth == 0 && requestedHeight == 0;
        if ((driver != GFX_AUTODETECT_WINDOWED && driver != GFX_AUTODETECT_FULLSCREEN) ||
            (!use_desktop_resolution && (requestedWidth <= 0 || requestedHeight <= 0)))
        {
            sl::detail::set_error("Invalid graphics mode or dimensions");
            return false;
        }

        display_shutdown();

        if (use_desktop_resolution)
        {
			auto display_id = SDL_GetPrimaryDisplay();
			auto desktop_mode = SDL_GetDesktopDisplayMode(display_id);

            requestedWidth = desktop_mode->w;
            requestedHeight = desktop_mode->h;
        }

        ttfInitialized = true;

        renderer = detail::create_renderer(selected_backend);
        if (!renderer)
        {
            sl::detail::set_error("Requested graphics backend is not implemented");
            ttfInitialized = false;
            return false;
        }

        window = SDL_CreateWindow(
            title,
            requestedWidth,
            requestedHeight,
            windowflags);
        if (!window)
        {
            sl::detail::set_error(SDL_GetError());
            
            ttfInitialized = false;
            
            return false;
        }

        std::string renderer_error;
        if (!renderer->initialise(window, renderer_error))
        {
            sl::detail::set_error(renderer_error);
            renderer->shutdown();
            renderer.reset();
            SDL_DestroyWindow(window);
            window = nullptr;
            ttfInitialized = false;
            return false;
        }

        SDL_GetWindowSize(window, &width, &height);
        // 0 means "follow the window size"; only an explicit virtual size is fixed
        logicalWidth = virtualWidth > 0 ? virtualWidth : 0;
        logicalHeight = virtualHeight > 0 ? virtualHeight : 0;
        if (!renderer->resize(width, height, renderer_error))
        {
            sl::detail::set_error(renderer_error);
            renderer->shutdown();
            renderer.reset();
            SDL_DestroyWindow(window);
            window = nullptr;
            ttfInitialized = false;
            return false;
        }
        detail::initialise_screen(logicalWidth > 0 ? logicalWidth : width, logicalHeight > 0 ? logicalHeight : height);
        detail::refresh_mouse_cursor();
        sl_default_monospace_font = open_monospace_font(12);
        if (!sl_default_monospace_font)
        {
            sl::detail::set_error("Unable to load the default monospace font");
            display_shutdown();
            return false;
        }
        return true;
    }

    void display_handle_event(const SDL_Event& event)
    {
		if (event.type == SDL_EVENT_WINDOW_RESIZED || event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
        {
			width  = event.window.data1;
			height = event.window.data2;
            if (renderer)
            {
                std::string error;
                if (!renderer->resize(width, height, error) && !error.empty())
                    sl::detail::set_error(error);
            }
            detail::resize_screen(logicalWidth > 0 ? logicalWidth : width, logicalHeight > 0 ? logicalHeight : height);
		}
    }

    namespace
    {
        bool mouseVisible = true;
    } // namespace

    namespace detail
    {
        void refresh_mouse_cursor()
        {
            // a custom bitmap is drawn by the library, so the OS cursor is hidden while it is in use
            const bool softwareCursor = mouseVisible && mouse_bitmap() != nullptr;
			if (mouseVisible && !softwareCursor)
			{
				SDL_ShowCursor();
			}
			else
			{
				SDL_HideCursor();
            }
        }
    } // namespace detail

    void show_mouse()
    {
        mouseVisible = true;
        detail::refresh_mouse_cursor();
    }

    void hide_mouse()
    {
        mouseVisible = false;
        detail::refresh_mouse_cursor();
    }

    void set_window_title(const char *title)
    {
        if (window && title)
            SDL_SetWindowTitle(window, title);
    }

    bool set_fullscreen(bool enabled)
    {
        if (!window || SDL_SetWindowFullscreen(window, enabled) != 0)
        {
            return false;
        }
        // fullscreen toggles don't reliably deliver a window_resized event, so sync the viewport now
        SDL_GetWindowSize(window, &width, &height);
        if (renderer)
        {
            std::string error;
            if (!renderer->resize(width, height, error) && !error.empty())
                sl::detail::set_error(error);
        }
        detail::resize_screen(logicalWidth > 0 ? logicalWidth : width, logicalHeight > 0 ? logicalHeight : height);
        return true;
    }

    bool is_fullscreen()
    {
        return window && (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) != 0;
    }
    bool toggle_fullscreen()
    {
        if (!window)
            return false;
        bool currently_fullscreen = is_fullscreen();
        return set_fullscreen(!currently_fullscreen);
    }
    int screen_width()
    {
        if (renderTargetWidth > 0)
        {
            return renderTargetWidth;
        }
        return logicalWidth > 0 ? logicalWidth : width;
    }

    int screen_height()
    {
        if (renderTargetHeight > 0)
        {
            return renderTargetHeight;
        }
        return logicalHeight > 0 ? logicalHeight : height;
    }

    int virtual_screen_width()
    {
        return logicalWidth > 0 ? logicalWidth : width;
    }

    int virtual_screen_height()
    {
        return logicalHeight > 0 ? logicalHeight : height;
    }

    void restore_window_viewport()
    {
        if (renderer && selected_backend == GraphicsBackend::opengl)
        {
            std::string error;
            renderer->resize(width, height, error);
        }
    }

    namespace detail
    {
        Renderer *active_renderer()
        {
            return renderer.get();
        }

        void set_render_target_size(int targetWidth, int targetHeight)
        {
            renderTargetWidth = targetWidth;
            renderTargetHeight = targetHeight;
        }

        void set_screen_offset(float x, float y)
        {
            screenOffsetX = x;
            screenOffsetY = y;
        }

        float screen_offset_x() { return screenOffsetX; }
        float screen_offset_y() { return screenOffsetY; }

    } // namespace detail

    void clear_to_colour(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
    {
        if (renderer)
        {
            renderer->clear_frame(
                static_cast<float>(red) / 255.0f,
                static_cast<float>(green) / 255.0f,
                static_cast<float>(blue) / 255.0f,
                static_cast<float>(alpha) / 255.0f);
            return;
        }
        sl::detail::set_error("No active renderer is available to clear the display");
    }

    void show_video_bitmap()
    {
        if (renderer)
        {
            if (mouseVisible)
                detail::draw_mouse_cursor();
            renderer->present();
        }
    }

    void wait_for_graphics()
    {
        if (renderer)
        {
            renderer->wait_idle();
        }
    }

    namespace detail
    {
        static unsigned shutdown_count = 0;
        unsigned display_generation() { return shutdown_count; }
    } // namespace detail

    void display_shutdown()
    {
        ++detail::shutdown_count;
        wait_for_graphics();
        detail::async_load_shutdown();
        detail::parallel_record_shutdown();
        detail::parallel_record_shutdown();

        if (sl_default_monospace_font && ttfInitialized)
        {
            close_font(sl_default_monospace_font);
            sl_default_monospace_font = nullptr;
        }

        detail::destroy_screen();
        if (renderer)
        {
            detail::release_transient_instance_buffers();
            renderer->shutdown();
            renderer.reset();
        }
        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        if (ttfInitialized)
        {

            
            ttfInitialized = false;
        }
        width = 0;
        height = 0;
        logicalWidth = 0;
        logicalHeight = 0;
        renderTargetWidth = 0;
        renderTargetHeight = 0;
        
    }

    SDL_Window *get_window()
    {
        return window;
    }

    SDL_GLContext get_gl_context()
    {
        return renderer ? renderer->native_context() : nullptr;
    }

} // namespace sl