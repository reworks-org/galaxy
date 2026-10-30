#pragma once

#include <SDL3/SDL_video.h>

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

        bool initialise(SDL_Window *window, std::string &error);
        void shutdown();
        bool resize(int width, int height, std::string &error);
        void present();
        void clear(float red, float green, float blue, float alpha);
        bool is_valid() const;

        SDL_GLContext native_context() const { return context_; }

    private:
        SDL_Window *window_ = nullptr;
        SDL_GLContext context_ = nullptr;
    };
}
