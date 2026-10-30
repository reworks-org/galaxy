/** @file
 * @brief Implements font discovery, text rendering, and text caching.
 */

#include "font.h"

#include "display.h"
#include "error.h"
#include "gl2d.h"

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
namespace
{
	/** Format printf-style arguments into a string of any length. */
	std::string format_va(const char *format, va_list arguments)
	{
		va_list copy;
		va_copy(copy, arguments);
		const int length = std::vsnprintf(nullptr, 0, format, copy);
		va_end(copy);
		if (length <= 0)
			return std::string();
		std::string result(static_cast<std::size_t>(length) + 1, '\0');
		std::vsnprintf(&result[0], result.size(), format, arguments);
		result.resize(static_cast<std::size_t>(length));
		return result;
	}
} // namespace


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
        /** Top-left of the glyph in atlas pixels. */
        int atlas_x = 0;
        int atlas_y = 0;
        bool exists = false;
    };

    /** A glyph cache texture that is filled on demand, so any code point the font provides can be drawn. */
    struct FontAtlas
    {
        Font *font = nullptr;
        std::uint32_t texture_id = 0;
        int atlas_width = 512;
        int atlas_height = 256;
        int ascent = 0;
        int descent = 0;
        int line_skip = 0;
        std::unordered_map<std::uint32_t, GlyphInfo> glyphs;
        std::vector<std::uint8_t> pixels;
        int cursor_x = 1;
        int cursor_y = 1;
        int row_height = 0;
        bool dirty = false;
        bool resized = false;
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

    /** Decode one UTF-8 sequence starting at index; invalid bytes become U+FFFD. */
    static std::uint32_t decode_utf8(const std::string &text, std::size_t &index)
    {
        const unsigned char lead = static_cast<unsigned char>(text[index++]);
        if (lead < 0x80)
            return lead;
        int extra = 0;
        std::uint32_t code = 0;
        if ((lead & 0xE0) == 0xC0)
            extra = 1, code = lead & 0x1F;
        else if ((lead & 0xF0) == 0xE0)
            extra = 2, code = lead & 0x0F;
        else if ((lead & 0xF8) == 0xF0)
            extra = 3, code = lead & 0x07;
        else
            return 0xFFFD;
        for (int i = 0; i < extra; ++i)
        {
            if (index >= text.size() || (static_cast<unsigned char>(text[index]) & 0xC0) != 0x80)
                return 0xFFFD;
            code = (code << 6) | (static_cast<unsigned char>(text[index++]) & 0x3F);
        }
        return code;
    }

    /** Find a glyph, rasterising it into the atlas on first use. Returns null when it cannot be stored. */
    static const GlyphInfo *font_atlas_glyph(FontAtlas &atlas, detail::Renderer &renderer, std::uint32_t code)
    {
        const auto cached = atlas.glyphs.find(code);
        if (cached != atlas.glyphs.end())
            return cached->second.exists ? &cached->second : nullptr;

        GlyphInfo info;
        std::uint32_t source = code;
		if (code != 32 && !TTF_FontHasGlyph(atlas.font, code))
            source = TTF_FontHasGlyph(atlas.font, '?') ? '?' : 0;
        if (source != 0)
        {
            int minx = 0, maxx = 0, miny = 0, maxy = 0, advance = 0;
            if (TTF_GetGlyphMetrics(atlas.font, source, &minx, &maxx, &miny, &maxy, &advance) == 0)
            {
                info.min_x = minx;
                info.max_x = maxx;
                info.min_y = miny;
                info.max_y = maxy;
                info.advance = advance;
                info.exists = true;
            }
        }

        SDL_Surface *rgba = nullptr;
        if (info.exists && source != 32)
        {
            if (SDL_Surface *surface = TTF_RenderGlyph_Blended(atlas.font, source, SDL_Color{255, 255, 255, 255}))
            {
                rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
                SDL_DestroySurface(surface);
            }
        }

        if (rgba && rgba->w > 0 && rgba->h > 0)
        {
            if (atlas.cursor_x + rgba->w + 1 >= atlas.atlas_width)
            {
                atlas.cursor_x = 1;
                atlas.cursor_y += atlas.row_height + 1;
                atlas.row_height = 0;
            }
            while (atlas.cursor_y + rgba->h + 1 > atlas.atlas_height)
            {
                if (atlas.atlas_height * 2 > renderer.max_texture_size())
                {
                    SDL_DestroySurface(rgba);
                    atlas.glyphs[code] = GlyphInfo{};
                    return nullptr;
                }
                atlas.atlas_height *= 2;
                atlas.pixels.resize(static_cast<std::size_t>(atlas.atlas_width) * atlas.atlas_height * 4, 0);
                atlas.resized = true;
            }
            info.width = rgba->w;
            info.height = rgba->h;
            info.atlas_x = atlas.cursor_x;
            info.atlas_y = atlas.cursor_y;
            const auto *src = static_cast<const std::uint8_t *>(rgba->pixels);
            for (int row = 0; row < rgba->h; ++row)
            {
                for (int col = 0; col < rgba->w; ++col)
                {
                    const std::uint8_t a = src[static_cast<std::size_t>(row) * rgba->pitch + col * 4 + 3];
                    std::uint8_t *dst = &atlas.pixels[(static_cast<std::size_t>(info.atlas_y + row) * atlas.atlas_width +
                                                       (info.atlas_x + col)) * 4];
                    dst[0] = dst[1] = dst[2] = a > 0 ? 255 : 0;
                    dst[3] = a;
                }
            }
            atlas.cursor_x += rgba->w + 1;
            atlas.row_height = std::max(atlas.row_height, rgba->h);
            atlas.dirty = true;
        }
        if (rgba)
            SDL_DestroySurface(rgba);

        const GlyphInfo &stored = atlas.glyphs[code] = info;
        return stored.exists ? &stored : nullptr;
    }

    /** Push newly rasterised glyphs to the GPU. Pending text is flushed first so it never sees a resized texture. */
    static bool sync_font_atlas(FontAtlas &atlas, detail::Renderer &renderer)
    {
        if (!atlas.dirty && !atlas.resized && atlas.texture_id != 0)
            return true;
        detail::gl2d_flush();
        if (atlas.resized || atlas.texture_id == 0)
        {
            if (atlas.texture_id != 0)
                renderer.destroy_texture(atlas.texture_id);
            atlas.texture_id = 0;
            if (!renderer.create_texture({atlas.atlas_width, atlas.atlas_height, detail::TextureFilter::linear}, atlas.texture_id))
                return false;
        }
        if (!renderer.upload_texture(atlas.texture_id, atlas.atlas_width, atlas.atlas_height, atlas.pixels.data()))
            return false;
        atlas.dirty = atlas.resized = false;
        return true;
    }

    static FontAtlas *get_or_create_font_atlas(Font *font)
    {
        if (!font)
            return nullptr;
        auto it = g_font_atlases.find(font);
        if (it != g_font_atlases.end())
            return it->second.get();

        detail::Renderer *renderer = detail::active_renderer();
        if (!renderer || renderer->max_texture_size() < 512)
            return nullptr;

        auto atlas = std::make_unique<FontAtlas>();
        atlas->font = font;
        atlas->ascent = TTF_GetFontAscent(font);
        atlas->descent = TTF_GetFontDescent(font);
        atlas->line_skip = TTF_GetFontLineSkip(font);
        atlas->pixels.assign(static_cast<std::size_t>(atlas->atlas_width) * atlas->atlas_height * 4, 0);
        for (std::uint32_t ch = 32; ch <= 126; ++ch)
            font_atlas_glyph(*atlas, *renderer, ch);
        if (!sync_font_atlas(*atlas, *renderer))
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
        SDL_Surface *rendered = TTF_RenderText_Blended(font, text.c_str(), text.size(), sdlColor);
        if (!rendered)
        {
            sl::detail::set_error(SDL_GetError());
            return std::nullopt;
        }

        SDL_Surface *rgba = SDL_ConvertSurface(rendered, SDL_PIXELFORMAT_RGBA32);
        SDL_DestroySurface(rendered);
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
            SDL_DestroySurface(rgba);
            return std::nullopt;
        }

        SDL_DestroySurface(rgba);
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

        // Reused across calls so a text run costs one submission and no allocation.
        static thread_local std::vector<detail::GLVertex> glyph_vertices;
        glyph_vertices.clear();
        glyph_vertices.reserve(text.size() * 6);

        detail::Renderer *renderer = detail::active_renderer();
        // Rasterise any new glyphs and upload them before building vertices, because an upload can resize the atlas.
        for (std::size_t i = 0; i < text.size();)
        {
            const std::uint32_t code = decode_utf8(text, i);
            if (code >= 32 && !(code >= 127 && code < 160))
                font_atlas_glyph(*atlas, *renderer, code);
        }
        if (!sync_font_atlas(*atlas, *renderer))
            return;
        const float inv_w = 1.0f / static_cast<float>(atlas->atlas_width);
        const float inv_h = 1.0f / static_cast<float>(atlas->atlas_height);

        for (std::size_t i = 0; i < text.size();)
        {
            const std::uint32_t c = decode_utf8(text, i);
            if (c == '\r')
                continue;
            if (c == '\n')
            {
                pen_x = static_cast<float>(x);
                pen_y += static_cast<float>(atlas->line_skip);
                continue;
            }
            if (c < 32 || (c >= 127 && c < 160))
                continue;

            const auto found = atlas->glyphs.find(c);
            if (found == atlas->glyphs.end() || !found->second.exists)
                continue;
            const GlyphInfo &info = found->second;

            if (info.width > 0 && info.height > 0)
            {
                const float x0 = pen_x + static_cast<float>(info.min_x < 0 ? info.min_x : 0);
                const float y0 = pen_y;
                const float x1 = x0 + static_cast<float>(info.width);
                const float y1 = y0 + static_cast<float>(info.height);
                const float u0 = static_cast<float>(info.atlas_x) * inv_w;
                const float v0 = static_cast<float>(info.atlas_y) * inv_h;
                const float u1 = static_cast<float>(info.atlas_x + info.width) * inv_w;
                const float v1 = static_cast<float>(info.atlas_y + info.height) * inv_h;

                const detail::GLVertex quad[4] = {
                    {x0, y0, u0, v0, r, g, b, a},
                    {x1, y0, u1, v0, r, g, b, a},
                    {x1, y1, u1, v1, r, g, b, a},
                    {x0, y1, u0, v1, r, g, b, a},
                };
                glyph_vertices.insert(glyph_vertices.end(), {quad[0], quad[1], quad[2], quad[0], quad[2], quad[3]});
            }

            pen_x += static_cast<float>(info.advance);
        }
        if (!glyph_vertices.empty())
            detail::gl2d_submit(detail::PrimitiveType::triangles, glyph_vertices.data(), static_cast<int>(glyph_vertices.size()), atlas->texture_id);
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

    // The bundled DejaVu fonts are the last resort for systems with none of the usual ones (a bare Wine prefix, say).
    std::vector<std::filesystem::path> bundled_font_candidates(const char *name)
    {
        std::vector<std::filesystem::path> paths;
        if (const char *base = SDL_GetBasePath())
        {
            paths.push_back(std::filesystem::path(base) / "assets" / "fonts" / name);
        }
        paths.push_back(std::filesystem::path("assets") / "fonts" / name);
        paths.push_back(std::filesystem::path("..") / "assets" / "fonts" / name);
        return paths;
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
        for (const auto &bundled : bundled_font_candidates("DejaVuSansMono.ttf"))
            candidates.push_back(bundled);

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

        auto *rw = SDL_IOFromConstMem(fontBytes->data(), static_cast<int>(fontBytes->size()));
        if (!rw)
        {
            return nullptr;
        }

        return TTF_OpenFontIO(rw, 1, pointSize);
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
        std::vector<std::filesystem::path> search = candidates;
        for (const auto &bundled : bundled_font_candidates("DejaVuSans.ttf"))
            search.push_back(bundled);

        for (const std::filesystem::path &path : search)
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
            sl::detail::set_error(SDL_GetError());
        return font;
    }

    Font *open_font_from_memory(const std::uint8_t *data, std::size_t size, int pointSize)
    {
        if (!data || size == 0 || size > std::numeric_limits<int>::max())
            return nullptr;
        auto *rw = SDL_IOFromConstMem(data, static_cast<int>(size));
		Font* font = rw ? TTF_OpenFontIO(rw, 1, pointSize) : nullptr;
        if (!font)
            sl::detail::set_error(SDL_GetError());
        return font;
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
        return font && TTF_GetStringSize(font, text.c_str(), text.size(), &width, &height) == 0 ? width : 0;
    }

    int text_height(Font *font)
    {
        return font ? TTF_GetFontHeight(font) : 0;
    }

    struct TextCache
    {
        std::string text;
        Font *font = nullptr;
        Colour colour{};
        TextTexture texture;
        bool has_texture = false;
    };

    namespace
    {
        TextPanelStyle text_panel;
    }

    void set_text_panel_style(const TextPanelStyle &style)
    {
        text_panel = style;
    }

    TextPanelStyle text_panel_style()
    {
        return text_panel;
    }

    // Draws text, on the text panel when one is enabled.
    void draw_panel_text(Font *font, int x, int y, const Colour &colour, const std::string &text, int width, int height)
    {
        if (!text_panel.enabled || !font || text.empty())
        {
            draw_text_atlas(font, x, y, colour, text, width, height);
            return;
        }
        fill_rect(x - text_panel.padding_x, y - text_panel.padding_y, text_length(font, text) + 2 * text_panel.padding_x,
                  text_height(font) + 2 * text_panel.padding_y, width, height, text_panel.background);
        draw_text_atlas(font, x, y, text_panel.text, text, width, height);
    }

    void draw_formatted_text(Font *font, int x, int y, const Colour &colour, const std::string &text)
    {
        draw_panel_text(font, x, y, colour, text, screen_width(), screen_height());
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
            draw_text_atlas(font, x, y, foreground, text, windowWidth, windowHeight);
            return;
        }

        draw_panel_text(font, x, y, foreground, text, windowWidth, windowHeight);
    }

    void textout(Font *font, int x, int y, const Colour &colour, const std::string &text)
    {
        gl_printf(font, x, y, colour, {0, 0, 0, 0}, screen_width(), screen_height(), text);
    }

    void textprintf(Font *font, int x, int y, const Colour &colour, const char *format, ...)
    {
        if (!format)
            return;
        va_list arguments;
        va_start(arguments, format);
        const std::string formatted = format_va(format, arguments);
        va_end(arguments);
        const char *buffer = formatted.c_str();
        textout(font, x, y, colour, buffer);
    }

    void gprintf(int x, int y, const Colour &colour, const char *fmt, ...)
    {
        if (!fmt)
            return;
        va_list args;
        va_start(args, fmt);
        const std::string formatted = format_va(fmt, args);
        va_end(args);
        const char *buffer = formatted.c_str();
        draw_formatted_text(get_default_monospace_font(), x, y, colour, buffer);
    }

    void gprintf_center(int y, const Colour &colour, const char *fmt, ...)
    {
        if (!fmt)
            return;
        Font *font = get_default_monospace_font();
        va_list args;
        va_start(args, fmt);
        const std::string formatted = format_va(fmt, args);
        va_end(args);
        const char *buffer = formatted.c_str();
        const int text_width = text_length(font, buffer);
        const int x = (screen->width - text_width) / 2;
        draw_formatted_text(font, x, y, colour, buffer);
    }
    /** Format and draw text with the given font at a screen position. */
    void gprintf(int x, int y,Font *font, const Colour &colour, const char *fmt, ...)
    {
        if (!fmt)
            return;
        va_list args;
        va_start(args, fmt);
        const std::string formatted = format_va(fmt, args);
        va_end(args);
        const char *buffer = formatted.c_str();
        draw_formatted_text(font, x, y, colour, buffer);
    }
    /** Format and draw text with the given font, horizontally centred on screen. */
    void gprintf_center(int y, Font *font, const Colour &colour, const char *fmt, ...)
    {
         if (!fmt)
            return;
        va_list args;
        va_start(args, fmt);
        const std::string formatted = format_va(fmt, args);
        va_end(args);
        const char *buffer = formatted.c_str();
        const int text_width = text_length(font, buffer);
        const int x = (screen->width - text_width) / 2;
        draw_formatted_text(font, x, y, colour, buffer);     
    }

    void draw_renderer_label()
    {
        if (!screen)
            return;
        Font *font = get_default_monospace_font();
        if (!font)
            return;
        const std::string label = "Renderer = " + graphics_backend_name();
        const int margin = 6;
        const int x = screen->width - text_length(font, label) - margin;
        const int y = screen->height - text_height(font) - margin;
        draw_formatted_text(font, x, y, {200, 200, 200}, label);
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

        const int width = screen_width();
        const int height = screen_height();
        const Colour effective = text_panel.enabled ? text_panel.text : colour;
        if (!cache)
        {
            draw_panel_text(font, x, y, colour, text, width, height);
            return;
        }

        const bool same = cache->has_texture && cache->font == font && cache->text == text &&
                          cache->colour.red == effective.red && cache->colour.green == effective.green &&
                          cache->colour.blue == effective.blue && cache->colour.alpha == effective.alpha;
        if (!same)
        {
            if (cache->has_texture)
            {
                destroy_text_texture(cache->texture);
                cache->has_texture = false;
            }
            const auto texture = build_text_texture(font, text, effective);
            if (!texture)
                return;
            cache->texture = *texture;
            cache->has_texture = true;
            cache->font = font;
            cache->text = text;
            cache->colour = effective;
        }

        if (text_panel.enabled)
        {
            fill_rect(x - text_panel.padding_x, y - text_panel.padding_y, cache->texture.width + 2 * text_panel.padding_x,
                      cache->texture.height + 2 * text_panel.padding_y, width, height, text_panel.background);
        }
        if (detail::Renderer *renderer = detail::active_renderer())
            renderer->set_premultiplied_alpha(false);
        draw_text_texture(cache->texture, x, y, width, height);
    }

    namespace
    {
        // Byte length of the UTF-8 sequence starting at the lead byte.
        std::size_t utf8_length(unsigned char lead)
        {
            return lead >= 0xF0 ? 4 : lead >= 0xE0 ? 3 : lead >= 0xC0 ? 2 : 1;
        }

        void wrap_paragraph(Font *font, const std::string &paragraph, int max_width, std::vector<std::string> &lines)
        {
            if (max_width <= 0 || paragraph.empty())
            {
                lines.push_back(paragraph);
                return;
            }
            std::string line;
            std::size_t position = 0;
            while (position < paragraph.size())
            {
                // Next word including its trailing spaces.
                std::size_t end = paragraph.find(' ', position);
                end = end == std::string::npos ? paragraph.size() : end;
                std::size_t word_end = end;
                while (end < paragraph.size() && paragraph[end] == ' ')
                    ++end;
                std::string word = paragraph.substr(position, end - position);
                const std::string trimmed = paragraph.substr(position, word_end - position);
                position = end;

                if (!line.empty() && text_length(font, line + trimmed) > max_width)
                {
                    lines.push_back(line);
                    line.clear();
                }
                if (line.empty() && text_length(font, trimmed) > max_width)
                {
                    // A word wider than the block is split between characters.
                    std::size_t at = 0;
                    while (at < word.size())
                    {
                        std::string piece;
                        while (at < word.size())
                        {
                            const std::size_t length = utf8_length(static_cast<unsigned char>(word[at]));
                            const std::string next = piece + word.substr(at, length);
                            if (!piece.empty() && text_length(font, next) > max_width)
                                break;
                            piece = next;
                            at += length;
                        }
                        if (at < word.size())
                            lines.push_back(piece);
                        else
                            line = piece;
                    }
                    continue;
                }
                line += word;
            }
            // Trailing spaces would skew centred and right-aligned lines.
            while (!line.empty() && line.back() == ' ')
                line.pop_back();
            lines.push_back(line);
        }
    } // namespace

    std::vector<std::string> wrap_text(Font *font, const std::string &text, int max_width)
    {
        std::vector<std::string> lines;
        if (!font)
            return lines;
        std::size_t start = 0;
        while (true)
        {
            const std::size_t newline = text.find('\n', start);
            std::string paragraph = text.substr(start, newline == std::string::npos ? std::string::npos : newline - start);
            if (!paragraph.empty() && paragraph.back() == '\r')
                paragraph.pop_back();
            wrap_paragraph(font, paragraph, max_width, lines);
            if (newline == std::string::npos)
                break;
            start = newline + 1;
        }
        return lines;
    }

    int text_block_width(Font *font, const std::string &text, const TextStyle &style)
    {
        if (style.wrap_width > 0)
            return style.wrap_width;
        int widest = 0;
        for (const std::string &line : wrap_text(font, text, 0))
            widest = std::max(widest, text_length(font, line));
        return widest;
    }

    int text_block_height(Font *font, const std::string &text, const TextStyle &style)
    {
        if (!font)
            return 0;
        const int lines = static_cast<int>(wrap_text(font, text, style.wrap_width).size());
        return lines * text_height(font) + std::max(0, lines - 1) * style.line_spacing;
    }

    void draw_text_block(Font *font, int x, int y, const Colour &colour, const std::string &text, const TextStyle &style)
    {
        if (!font || text.empty())
            return;
        const std::vector<std::string> lines = wrap_text(font, text, style.wrap_width);
        const int block_width = text_block_width(font, text, style);
        const int line_height = text_height(font);
        const int width = screen_width();
        const int height = screen_height();
        Colour main_colour = colour;
        if (text_panel.enabled)
        {
            fill_rect(x - text_panel.padding_x, y - text_panel.padding_y, block_width + 2 * text_panel.padding_x,
                      text_block_height(font, text, style) + 2 * text_panel.padding_y, width, height, text_panel.background);
            main_colour = text_panel.text;
        }
        int line_y = y;
        for (const std::string &line : lines)
        {
            if (!line.empty())
            {
                const int line_width = text_length(font, line);
                const int line_x = style.align == TextAlign::center ? x + (block_width - line_width) / 2
                                   : style.align == TextAlign::right ? x + block_width - line_width
                                                                     : x;
                if (style.shadow)
                    draw_text_atlas(font, line_x + style.shadow_offset_x, line_y + style.shadow_offset_y, style.shadow_colour, line, width, height);
                if (style.outline)
                {
                    const int size = std::max(1, style.outline_size);
                    for (int dy = -size; dy <= size; ++dy)
                        for (int dx = -size; dx <= size; ++dx)
                            if (dx != 0 || dy != 0)
                                draw_text_atlas(font, line_x + dx, line_y + dy, style.outline_colour, line, width, height);
                }
                draw_text_atlas(font, line_x, line_y, main_colour, line, width, height);
            }
            line_y += line_height + style.line_spacing;
        }
    }

} // namespace sl