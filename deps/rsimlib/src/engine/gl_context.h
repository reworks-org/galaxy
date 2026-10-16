#pragma once

#include <SDL2/SDL_video.h>

#include <string>

namespace sl::detail
{
    /** Owns the SDL/OpenGL context; the default framebuffer is the window itself. */
    class GLContext
    {
    public:
        GLContext() = default;
        ~GLContext();

        GLContext(const GLContext &) = delete;
        GLContext &operator=(const GLContext &) = delete;

        /** Request a core 4.3 context; must be called before the SDL window is created. */
        static void configure_window_attributes();

        bool initialise(SDL_Window *window, std::string &error);
        void shutdown();
        bool resize(int width, int height, std::string &error);
        bool set_vsync(bool enabled);
        bool vsync_active() const;
        void present();
        void clear(float red, float green, float blue, float alpha);
        bool is_valid() const;

        SDL_GLContext native_context() const { return context_; }

    private:
        SDL_Window *window_ = nullptr;
        SDL_GLContext context_ = nullptr;
    };
}
