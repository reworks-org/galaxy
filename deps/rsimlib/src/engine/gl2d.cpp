#include "gl2d.h"
#include "display.h"

#include <SDL2/SDL_opengl.h>

namespace sl::detail
{
    void gl2d_ortho_matrix(int windowWidth, int windowHeight, float *outMatrix16)
    {
        const float width = static_cast<float>(windowWidth);
        const float height = static_cast<float>(windowHeight);
        for (int index = 0; index < 16; ++index)
            outMatrix16[index] = 0.0f;
        outMatrix16[0] = width > 0.0f ? 2.0f / width : 0.0f;
        outMatrix16[5] = height > 0.0f ? -2.0f / height : 0.0f;
        outMatrix16[10] = -1.0f;
        outMatrix16[12] = -1.0f + (width > 0.0f ? 2.0f * detail::screen_offset_x() / width : 0.0f);
        outMatrix16[13] = 1.0f - (height > 0.0f ? 2.0f * detail::screen_offset_y() / height : 0.0f);
        outMatrix16[12] = -1.0f + (width > 0.0f ? 2.0f * detail::screen_offset_x() / width : 0.0f);
        outMatrix16[13] = 1.0f - (height > 0.0f ? 2.0f * detail::screen_offset_y() / height : 0.0f);
        outMatrix16[15] = 1.0f;
    }

    bool gl2d_init()
    {
        Renderer *renderer = active_renderer();
        return renderer && renderer->initialise_2d();
    }

    void gl2d_shutdown()
    {
        if (Renderer *renderer = active_renderer())
            renderer->shutdown_2d();
    }

    void gl2d_begin(int windowWidth, int windowHeight)
    {
        if (Renderer *renderer = active_renderer())
            renderer->begin_2d(windowWidth, windowHeight);
    }

    void gl2d_submit(std::uint32_t primitiveMode, const Vertex2D *vertices, int count, std::uint32_t texture)
    {
        if (Renderer *renderer = active_renderer())
        {
            PrimitiveType mode = PrimitiveType::triangle_fan;
            switch (primitiveMode)
            {
            case GL_POINTS:
                mode = PrimitiveType::points;
                break;
            case GL_LINES:
                mode = PrimitiveType::lines;
                break;
            case GL_LINE_LOOP:
                mode = PrimitiveType::line_loop;
                break;
            case GL_TRIANGLES:
                mode = PrimitiveType::triangles;
                break;
            case GL_TRIANGLE_FAN:
                mode = PrimitiveType::triangle_fan;
                break;
            }
            renderer->submit_2d(mode, vertices, count, texture);
        }
    }

    void gl2d_submit(PrimitiveType primitiveMode, const Vertex2D *vertices, int count, std::uint32_t texture)
    {
        if (Renderer *renderer = active_renderer())
        {
            renderer->submit_2d(primitiveMode, vertices, count, texture);
        }
    }

    void gl2d_flush()
    {
        if (Renderer *renderer = active_renderer())
        {
            renderer->flush_2d();
        }
    }
}
