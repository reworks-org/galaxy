/** @file
 * @brief Implements font discovery, text rendering, and text caching.
 */

#include "font.h"

#include "display.h"
#include "error.h"
#include "gl2d.h"
#include "resource.h"

#include <filesystem>
#include <fstream>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <array>
#include <memory>

namespace sl
{
    /** Temporary renderer texture used to draw one rendered text string. */
    struct TextTexture
    {
        std::uint32_t id = 0;
        int width = 0;
        int height = 0;
    };

    struct GlyphInfo
    {
        int min_x = 0;
        int max_x = 0;
        int min_y = 0;
        int max_y = 0;
        int advance = 0;
        int width = 0;
        int height = 0;
        float u0 = 0.0f;
        float v0 = 0.0f;
        float u1 = 0.0f;
        float v1 = 0.0f;
        bool exists = false;
    };

    struct FontAtlas
    {
        Font *font = nullptr;
        std::uint32_t texture_id = 0;
        int atlas_width = 0;
        int atlas_height = 0;
        int ascent = 0;
        int descent = 0;
        int line_skip = 0;
        std::array<GlyphInfo, 128> glyphs{};
    };

    static std::unordered_map<Font *, std::unique_ptr<FontAtlas>> g_font_atlases;

    static void destroy_font_atlas(Font *font)
    {
        auto it = g_font_atlases.find(font);
        if (it != g_font_atlases.end())
        {
            if (it->second->texture_id != 0)
            {
                if (detail::Renderer *renderer = detail::active_renderer())
                    renderer->destroy_texture(it->second->texture_id);
            }
            g_font_atlases.erase(it);
        }
    }

    static FontAtlas *get_or_create_font_atlas(Font *font)
    {
        if (!font)
            return nullptr;
        auto it = g_font_atlases.find(font);
        if (it != g_font_atlases.end())
        {
            return it->second.get();
        }

        detail::Renderer *renderer = detail::active_renderer();
        if (!renderer)
            return nullptr;

        auto atlas = std::make_unique<FontAtlas>();
        atlas->font = font;
        atlas->ascent = TTF_FontAscent(font);
        atlas->descent = TTF_FontDescent(font);
        atlas->line_skip = TTF_FontLineSkip(font);

        struct RenderedGlyph
        {
            SDL_Surface *surface = nullptr;
            int min_x = 0, max_x = 0, min_y = 0, max_y = 0, advance = 0;
            int dst_x = 0, dst_y = 0;
        };
        std::vector<RenderedGlyph> rendered_glyphs(128);

        const SDL_Color white{255, 255, 255, 255};
        int current_x = 1;
        int current_y = 1;
        int row_height = 0;
        const int atlas_w = 512;
        int max_needed_h = 64;

        for (int ch = 32; ch <= 126; ++ch)
        {
            int minx = 0, maxx = 0, miny = 0, maxy = 0, advance = 0;
            TTF_GlyphMetrics(font, static_cast<Uint16>(ch), &minx, &maxx, &miny, &maxy, &advance);
            rendered_glyphs[ch].min_x = minx;
            rendered_glyphs[ch].max_x = maxx;
            rendered_glyphs[ch].min_y = miny;
            rendered_glyphs[ch].max_y = maxy;
            rendered_glyphs[ch].advance = advance;

            if (ch == 32)
            {
                continue;
            }

            SDL_Surface *glyph_surf = TTF_RenderGlyph_Blended(font, static_cast<Uint16>(ch), white);
            if (!glyph_surf)
                continue;

            SDL_Surface *rgba = SDL_ConvertSurfaceFormat(glyph_surf, SDL_PIXELFORMAT_RGBA32, 0);
            SDL_FreeSurface(glyph_surf);
            if (!rgba)
                continue;

            rendered_glyphs[ch].surface = rgba;

            if (current_x + rgba->w + 1 >= atlas_w)
            {
                current_x = 1;
                current_y += row_height + 1;
                row_height = 0;
            }

            rendered_glyphs[ch].dst_x = current_x;
            rendered_glyphs[ch].dst_y = current_y;
            current_x += rgba->w + 1;
            row_height = std::max(row_height, rgba->h);
            max_needed_h = std::max(max_needed_h, current_y + row_height + 1);
        }

        int atlas_h = 64;
        while (atlas_h < max_needed_h)
            atlas_h *= 2;

        atlas->atlas_width = atlas_w;
        atlas->atlas_height = atlas_h;

        std::vector<std::uint8_t> atlas_pixels(static_cast<std::size_t>(atlas_w) * atlas_h * 4, 0);

        for (int ch = 32; ch <= 126; ++ch)
        {
            GlyphInfo &info = atlas->glyphs[ch];
            info.min_x = rendered_glyphs[ch].min_x;
            info.max_x = rendered_glyphs[ch].max_x;
            info.min_y = rendered_glyphs[ch].min_y;
            info.max_y = rendered_glyphs[ch].max_y;
            info.advance = rendered_glyphs[ch].advance;
            info.exists = true;

            if (ch == 32 || !rendered_glyphs[ch].surface)
            {
                continue;
            }

            SDL_Surface *s = rendered_glyphs[ch].surface;
            info.width = s->w;
            info.height = s->h;
            const int dst_x = rendered_glyphs[ch].dst_x;
            const int dst_y = rendered_glyphs[ch].dst_y;

            info.u0 = static_cast<float>(dst_x) / atlas_w;
            info.v0 = static_cast<float>(dst_y) / atlas_h;
            info.u1 = static_cast<float>(dst_x + s->w) / atlas_w;
            info.v1 = static_cast<float>(dst_y + s->h) / atlas_h;

            const auto *src_pixels = static_cast<const std::uint8_t *>(s->pixels);
            for (int row = 0; row < s->h; ++row)
            {
                for (int col = 0; col < s->w; ++col)
                {
                    const std::size_t src_idx = static_cast<std::size_t>(row) * s->pitch + col * 4;
                    const std::size_t dst_idx = (static_cast<std::size_t>(dst_y + row) * atlas_w + (dst_x + col)) * 4;
                    const std::uint8_t a = src_pixels[src_idx + 3];
                    atlas_pixels[dst_idx + 0] = a > 0 ? 255 : 0;
                    atlas_pixels[dst_idx + 1] = a > 0 ? 255 : 0;
                    atlas_pixels[dst_idx + 2] = a > 0 ? 255 : 0;
                    atlas_pixels[dst_idx + 3] = a;
                }
            }

            SDL_FreeSurface(s);
            rendered_glyphs[ch].surface = nullptr;
        }

        if (!renderer->create_texture({atlas_w, atlas_h, detail::TextureFilter::linear}, atlas->texture_id) ||
            !renderer->upload_texture(atlas->texture_id, atlas_w, atlas_h, atlas_pixels.data()))
        {
            if (atlas->texture_id != 0)
                renderer->destroy_texture(atlas->texture_id);
            return nullptr;
        }

        FontAtlas *result = atlas.get();
        g_font_atlases[font] = std::move(atlas);
        return result;
    }

    /** Convert a UTF-8 string into a temporary GPU text texture. */
    std::optional<TextTexture> build_text_texture(Font *font, const std::string &text, const Colour &colour)
    {
        const SDL_Color sdlColor{colour.red, colour.green, colour.blue, colour.alpha};
        SDL_Surface *rendered = TTF_RenderUTF8_Blended(font, text.c_str(), sdlColor);
        if (!rendered)
        {
            sl::detail::set_error(TTF_GetError());
            return std::nullopt;
        }

        SDL_Surface *rgba = SDL_ConvertSurfaceFormat(rendered, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(rendered);
        if (!rgba)
        {
            sl::detail::set_error(SDL_GetError());
            return std::nullopt;
        }

        TextTexture texture;
        texture.width = rgba->w;
        texture.height = rgba->h;

        detail::Renderer *renderer = detail::active_renderer();
        if (!renderer || !renderer->create_texture({texture.width, texture.height, detail::TextureFilter::linear}, texture.id) ||
            !renderer->upload_texture(texture.id, texture.width, texture.height,
                                      static_cast<const std::uint8_t *>(rgba->pixels)))
        {
            if (renderer)
                renderer->destroy_texture(texture.id);
            SDL_FreeSurface(rgba);
            return std::nullopt;
        }

        SDL_FreeSurface(rgba);
        return texture;
    }

    /** Release a temporary text texture. */
    void destroy_text_texture(TextTexture &texture)
    {
        if (texture.id != 0)
        {
            if (detail::Renderer *renderer = detail::active_renderer())
                renderer->destroy_texture(texture.id);
            texture.id = 0;
        }
    }

    /** Draw a temporary text texture in screen coordinates. */
    void draw_text_texture(const TextTexture &texture, int x, int y, int windowWidth, int windowHeight)
    {
        if (texture.id == 0)
        {
            return;
        }

        detail::gl2d_begin(windowWidth, windowHeight);

        const float x0 = static_cast<float>(x);
        const float y0 = static_cast<float>(y);
        const float x1 = static_cast<float>(x + texture.width);
        const float y1 = static_cast<float>(y + texture.height);

        const detail::GLVertex vertices[4] = {
            {x0, y0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f},
            {x1, y0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f},
            {x1, y1, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
            {x0, y1, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        };
        detail::gl2d_submit(detail::PrimitiveType::triangle_fan, vertices, 4, texture.id);
    }

    void draw_text_atlas(Font *font, int x, int y, const Colour &colour, const std::string &text, int windowWidth, int windowHeight)
    {
        if (!font || text.empty())
            return;

        bool is_all_ascii = true;
        for (unsigned char c : text)
        {
            if (c != '\n' && c != '\r' && (c < 32 || c > 126))
            {
                is_all_ascii = false;
                break;
            }
        }

        if (!is_all_ascii)
        {
            const auto texture = build_text_texture(font, text, colour);
            if (texture)
            {
                draw_text_texture(*texture, x, y, windowWidth, windowHeight);
                TextTexture mutableTex = *texture;
                destroy_text_texture(mutableTex);
            }
            return;
        }

        FontAtlas *atlas = get_or_create_font_atlas(font);
        if (!atlas || atlas->texture_id == 0)
        {
            const auto texture = build_text_texture(font, text, colour);
            if (texture)
            {
                draw_text_texture(*texture, x, y, windowWidth, windowHeight);
                TextTexture mutableTex = *texture;
                destroy_text_texture(mutableTex);
            }
            return;
        }

        detail::gl2d_begin(windowWidth, windowHeight);
        if (detail::Renderer *renderer = detail::active_renderer())
            renderer->set_premultiplied_alpha(false);

        const float r = static_cast<float>(colour.red) / 255.0f;
        const float g = static_cast<float>(colour.green) / 255.0f;
        const float b = static_cast<float>(colour.blue) / 255.0f;
        const float a = static_cast<float>(colour.alpha) / 255.0f;

        float pen_x = static_cast<float>(x);
        float pen_y = static_cast<float>(y);

        for (std::size_t i = 0; i < text.size(); ++i)
        {
            unsigned char c = static_cast<unsigned char>(text[i]);
            if (c == '\r')
                continue;
            if (c == '\n')
            {
                pen_x = static_cast<float>(x);
                pen_y += static_cast<float>(atlas->line_skip);
                continue;
            }
            if (c < 32 || c > 126)
                continue;

            const GlyphInfo &info = atlas->glyphs[c];
            if (!info.exists)
                continue;

            if (info.width > 0 && info.height > 0)
            {
                const float x0 = pen_x + static_cast<float>(info.min_x < 0 ? info.min_x : 0);
                const float y0 = pen_y;
                const float x1 = x0 + static_cast<float>(info.width);
                const float y1 = y0 + static_cast<float>(info.height);

                const detail::GLVertex quad[4] = {
                    {x0, y0, info.u0, info.v0, r, g, b, a},
                    {x1, y0, info.u1, info.v0, r, g, b, a},
                    {x1, y1, info.u1, info.v1, r, g, b, a},
                    {x0, y1, info.u0, info.v1, r, g, b, a},
                };
                detail::gl2d_submit(detail::PrimitiveType::triangle_fan, quad, 4, atlas->texture_id);
            }

            pen_x += static_cast<float>(info.advance);
        }
    }

    /** Draw a solid rectangle behind rendered text. */
    void fill_rect(int x, int y, int width, int height, int windowWidth, int windowHeight, const Colour &colour)
    {
        if (width <= 0 || height <= 0)
        {
            return;
        }

        detail::gl2d_begin(windowWidth, windowHeight);

        const float x0 = static_cast<float>(x);
        const float y0 = static_cast<float>(y);
        const float x1 = static_cast<float>(x + width);
        const float y1 = static_cast<float>(y + height);
        const float r = static_cast<float>(colour.red) / 255.0f;
        const float g = static_cast<float>(colour.green) / 255.0f;
        const float b = static_cast<float>(colour.blue) / 255.0f;
        const float a = static_cast<float>(colour.alpha) / 255.0f;

        const detail::GLVertex vertices[4] = {
            {x0, y0, 0.0f, 0.0f, r, g, b, a},
            {x1, y0, 0.0f, 0.0f, r, g, b, a},
            {x1, y1, 0.0f, 0.0f, r, g, b, a},
            {x0, y1, 0.0f, 0.0f, r, g, b, a},
        };
        detail::gl2d_submit(detail::PrimitiveType::triangle_fan, vertices, 4);
    }

    std::optional<std::vector<unsigned char>> load_font()
    {
        std::vector<std::filesystem::path> candidates;

#ifdef _WIN32
        candidates = {
            R"(C:\Windows\Fonts\consola.ttf)",
            R"(C:\Windows\Fonts\lucon.ttf)",
            R"(C:\Windows\Fonts\cour.ttf)"};
#else
        candidates = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
            "/usr/share/fonts/truetype/liberation2/LiberationMono-Regular.ttf",
            "/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf"};
#endif

        for (const auto &fontPath : candidates)
        {
            if (!std::filesystem::exists(fontPath))
            {
                continue;
            }

            std::ifstream file(fontPath, std::ios::binary);
            if (!file)
            {
                continue;
            }

            std::vector<unsigned char> bytes(
                (std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

            if (!bytes.empty())
            {
                return bytes;
            }
        }

        return std::nullopt;
    }

    Font *sl_default_monospace_font = nullptr;

    Font *get_default_monospace_font()
    {
        return sl_default_monospace_font;
    }
    Font *open_monospace_font(int pointSize)
    {
        // TTF_OpenFontRW reads glyph data from this buffer for the font's entire
        // lifetime, so it must outlive this call rather than being freed here
        static const std::optional<std::vector<unsigned char>> fontBytes = load_font();
        if (!fontBytes)
        {
            return nullptr;
        }

        SDL_RWops *rw = SDL_RWFromConstMem(fontBytes->data(), static_cast<int>(fontBytes->size()));
        if (!rw)
        {
            return nullptr;
        }

        return TTF_OpenFontRW(rw, 1, pointSize);
    }

    Font *open_sans_font(int pointSize)
    {
#ifdef _WIN32
        static const std::vector<std::filesystem::path> candidates = {
            R"(C:\Windows\Fonts\segoeui.ttf)",
            R"(C:\Windows\Fonts\arial.ttf)"};
#else
        static const std::vector<std::filesystem::path> candidates = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
            "/usr/share/fonts/truetype/noto/NotoSans-Regular.ttf"};
#endif

        for (const std::filesystem::path &path : candidates)
        {
            if (std::filesystem::exists(path))
            {
                return open_font(path.string(), pointSize);
            }
        }
        return nullptr;
    }

    Font *open_font(const std::string &path, int pointSize)
    {
        Font *font = TTF_OpenFont(path.c_str(), pointSize);
        if (!font)
            sl::detail::set_error(TTF_GetError());
        return font;
    }

    Font *open_font_from_memory(const std::uint8_t *data, std::size_t size, int pointSize)
    {
        if (!data || size == 0 || size > std::numeric_limits<int>::max())
            return nullptr;
        SDL_RWops *rw = SDL_RWFromConstMem(data, static_cast<int>(size));
        Font *font = rw ? TTF_OpenFontRW(rw, 1, pointSize) : nullptr;
        if (!font)
            sl::detail::set_error(TTF_GetError());
        return font;
    }

    Font *open_font(const Archive &archive, const std::string &name, int pointSize)
    {
        const auto bytes = archive.read(name);
        return open_font_from_memory(bytes.data(), bytes.size(), pointSize);
    }

    void close_font(Font *font)
    {
        if (font)
        {
            destroy_font_atlas(font);
            TTF_CloseFont(font);
        }
    }

    int text_length(Font *font, const std::string &text)
    {
        int width = 0;
        int height = 0;
        return font && TTF_SizeUTF8(font, text.c_str(), &width, &height) == 0 ? width : 0;
    }

    int text_height(Font *font)
    {
        return font ? TTF_FontHeight(font) : 0;
    }

    struct TextCache
    {
        std::string text;
        Font *font = nullptr;
        Colour colour{};
        TextTexture texture;
        bool has_texture = false;
    };

    void draw_formatted_text(Font *font, int x, int y, const Colour &colour, const std::string &text)
    {
        draw_text_atlas(font, x, y, colour, text, screen_width(), screen_height());
    }

    void gl_printf(
        Font *font,
        int x,
        int y,
        const Colour &foreground,
        const Colour &background,
        int windowWidth,
        int windowHeight,
        const std::string &text)
    {
        if (!font || text.empty())
        {
            return;
        }

        if (background.alpha > 0)
        {
            const int width = text_length(font, text);
            const int height = text_height(font);
            fill_rect(x, y, width, height, windowWidth, windowHeight, background);
        }

        draw_text_atlas(font, x, y, foreground, text, windowWidth, windowHeight);
    }

    void textout(Font *font, int x, int y, const Colour &colour, const std::string &text)
    {
        gl_printf(font, x, y, colour, {0, 0, 0, 0}, screen_width(), screen_height(), text);
    }

    void textprintf(Font *font, int x, int y, const Colour &colour, const char *format, ...)
    {
        if (!format)
            return;
        char buffer[1024];
        va_list arguments;
        va_start(arguments, format);
        std::vsnprintf(buffer, sizeof(buffer), format, arguments);
        va_end(arguments);
        textout(font, x, y, colour, buffer);
    }

    void gprintf(int x, int y, const Colour &colour, const char *fmt, ...)
    {
        if (!fmt)
            return;
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        draw_formatted_text(get_default_monospace_font(), x, y, colour, buffer);
    }

    void gprintf_center(int y, const Colour &colour, const char *fmt, ...)
    {
        if (!fmt)
            return;
        Font *font = get_default_monospace_font();
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        const int text_width = text_length(font, buffer);
        const int x = (screen->width - text_width) / 2;
        draw_formatted_text(font, x, y, colour, buffer);
    }
    /** Format and draw text with the given font at a screen position. */
    void gprintf(int x, int y,Font *font, const Colour &colour, const char *fmt, ...)
    {
        if (!fmt)
            return;
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        draw_formatted_text(font, x, y, colour, buffer);
    }
    /** Format and draw text with the given font, horizontally centred on screen. */
    void gprintf_center(int y, Font *font, const Colour &colour, const char *fmt, ...)
    {
         if (!fmt)
            return;
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        const int text_width = text_length(font, buffer);
        const int x = (screen->width - text_width) / 2;
        draw_formatted_text(font, x, y, colour, buffer);     
    }

    TextCache *create_text_cache()
    {
        return new TextCache();
    }

    void destroy_text_cache(TextCache *cache)
    {
        if (!cache)
        {
            return;
        }
        if (cache->has_texture)
        {
            destroy_text_texture(cache->texture);
        }
        delete cache;
    }

    void textout_cached(TextCache *cache, Font *font, int x, int y, const Colour &colour, const std::string &text)
    {
        if (!font || text.empty())
        {
            return;
        }

        draw_text_atlas(font, x, y, colour, text, screen_width(), screen_height());
    }

} // namespace sl