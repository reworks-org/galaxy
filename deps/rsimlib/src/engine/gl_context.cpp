/** @file
 * @brief Owns the SDL/OpenGL context lifecycle: creation, viewport, and swap.
 */
#include "gl_context.h"

#include <glad/glad.h>

#include <SDL3/SDL.h>
#include <cstdio>

namespace sl::detail
{
    GLContext::~GLContext()
    {
        shutdown();
    }

    bool GLContext::initialise(SDL_Window *window, std::string &error)
    {
        window_ = window;
        context_ = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window_, context_);
        if (!context_)
        {
            error = SDL_GetError();
            window_ = nullptr;
            return false;
        }
#ifdef _WIN32
		gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
#endif
        std::fprintf(stderr, "OpenGL renderer: %s (%s)\n", reinterpret_cast<const char *>(glGetString(GL_RENDERER)),
                reinterpret_cast<const char *>(glGetString(GL_VERSION)));
        return true;
    }

    void GLContext::shutdown()
    {
        if (context_)
        {
            SDL_GL_DestroyContext(context_);
            context_ = nullptr;
        }
        window_ = nullptr;
    }

    bool GLContext::resize(int width, int height, std::string &error)
    {
        if (!context_ || width <= 0 || height <= 0)
        {
            error = "Invalid OpenGL viewport dimensions.";
            return false;
        }
        glViewport(0, 0, width, height);
        return true;
    }

    void GLContext::present()
    {
        if (window_)
        {
            SDL_GL_SwapWindow(window_);
        }
    }

    void GLContext::clear(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    bool GLContext::is_valid() const
    {
        return context_ != nullptr;
    }
}
