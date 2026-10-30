#pragma once

#include <SDL3/SDL.h>

#include <string>

namespace sl
{
    struct Bitmap;

    /** Select the windowed auto-detected graphics driver. */
    constexpr int GFX_AUTODETECT_WINDOWED = 1;
    /** Select the auto-detected desktop fullscreen graphics driver. */
    constexpr int GFX_AUTODETECT_FULLSCREEN = 2;

    /** Rendering backends understood by the engine. */
    enum class GraphicsBackend
    {
        opengl,
        vulkan,
        d3d12
    };

    /**
     * Select a backend before set_gfx_mode(); D3D12 is Windows-only. Default: D3D12 on Windows, OpenGL elsewhere.
     * The backend is fixed for the life of the process: switching at runtime is unsupported because GPU
     * resources (bitmaps, render targets, meshes) are not migrated between backends.
     */
    bool set_graphics_backend(GraphicsBackend backend);
    /** Select OpenGL, Vulkan, or D3D12 from command-line arguments (--gl, --vulkan, --d3d12). */
    bool configure_graphics_backend_from_args(int argc, char *argv[]);
    /** Return whether a backend implementation is available in this build. */
    bool graphics_backend_available(GraphicsBackend backend);
    /** Return the selected graphics backend. */
    GraphicsBackend graphics_backend();
    /** Return a human-readable name for the selected graphics backend (e.g. "OpenGL", "Vulkan"). */
    std::string graphics_backend_name();

    /** Create a window (windowed or desktop fullscreen) and initialize the drawing screen using the selected backend.
     * For fullscreen mode, width and height may both be omitted to use the primary display's desktop resolution.
     */
	bool set_gfx_mode(const char* title, SDL_WindowFlags windowflags, int driver, int width = 0, int height = 0, int virtualWidth = 0, int virtualHeight = 0);
    /** Apply window-related SDL events, including resize events. */
    void display_handle_event(const SDL_Event &event);
    /** Set the window title. */
    void set_window_title(const char *title);
    /** Enable or disable desktop fullscreen mode. */
    bool set_fullscreen(bool enabled);
    /** Return whether the display is currently fullscreen. */
    bool is_fullscreen();
    /** Toggle the fullscreen state of the display. */
    bool toggle_fullscreen();
    /** Return the current drawable width in pixels. */
    int screen_width();
    /** Return the current drawable height in pixels. */
    int screen_height();
    /** Return the configured logical screen width. */
    int virtual_screen_width();
    /** Return the configured logical screen height. */
    int virtual_screen_height();
    /** Restore the GL viewport to the actual window size (used after rendering to an offscreen target). */
    void restore_window_viewport();
    
    /** Show the mouse cursor (the custom bitmap if one is set, otherwise the OS cursor). */
    void show_mouse();
    /** Hide the mouse cursor. */
    void hide_mouse();
    /** Use a bitmap as the mouse cursor, drawn so the pixel at (offset_x, offset_y), measured from the bitmap's top-left, sits on the mouse position; nullptr restores the OS cursor. */
    void set_mouse_bitmap(Bitmap *mouse_bitmap, int offset_x = 0, int offset_y = 0);
    namespace detail
    {
        /** Counts completed display_shutdown() calls; lets long-lived objects tell whether their GPU resources are gone. */
        unsigned display_generation();
        /** Return the custom mouse cursor bitmap, or nullptr. */
        Bitmap *mouse_bitmap();
        /** Apply the current mouse visibility and cursor bitmap to the OS cursor. */
        void refresh_mouse_cursor();
        /** Draw the custom mouse cursor, if any, at the mouse position. */
        void draw_mouse_cursor();
        /** Override screen_width()/screen_height() while an offscreen render target is bound; 0 clears it. */
        void set_render_target_size(int width, int height);
        void set_screen_offset(float x, float y);
        float screen_offset_x();
        float screen_offset_y();
        /** Set the transient 2D camera offset used by backend projections. */
        void set_screen_offset(float x, float y);
        float screen_offset_x();
        float screen_offset_y();
    } // namespace detail

    /** Clear the current framebuffer using an RGBA colour. */
    void clear_to_colour(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);
    /** Present the current OpenGL framebuffer to the window. */
    void show_video_bitmap();
    /** Wait until all submitted graphics work has completed. */
    void wait_for_graphics();
    /** Release the OpenGL context, window, and SDL video state. */
    void display_shutdown();
    /** Return the active SDL window, or nullptr if not initialized. */
    SDL_Window *get_window();
    /** Return the active OpenGL context, or nullptr if not initialized. */
    SDL_GLContext get_gl_context();

} // namespace sl