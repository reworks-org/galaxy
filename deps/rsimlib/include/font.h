#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "draw.h"

#include <optional>
#include <string>
#include <vector>

namespace sl
{
    class Archive;
}

namespace sl
{

    /** Opaque alias hiding the SDL_ttf font type from callers. */
    using Font = TTF_Font;

    /** Load the first available platform monospace font into memory. */
    std::optional<std::vector<unsigned char>> load_font();
    /** Get the default monospace font. */
    Font *get_default_monospace_font();
    /** Open the loaded font bytes as an SDL_ttf font. */
    Font *open_monospace_font(int pointSize);
    /** Open the first available platform sans-serif TrueType font. */
    Font *open_sans_font(int pointSize);
    /** Open a TrueType or OpenType font from an explicit file path. */
    Font *open_font(const std::string &path, int pointSize);
    /** Open a font from memory. */
    Font *open_font_from_memory(const std::uint8_t *data, std::size_t size, int pointSize);
    /** Open a font entry from a ZIP archive. */
    Font *open_font(const Archive &archive, const std::string &name, int pointSize);
    /** Close a font returned by an open_font or open_*_font function. */
    void close_font(Font *font);
    /** Return the rendered width of UTF-8 text in pixels. */
    int text_length(Font *font, const std::string &text);
    /** Return the rendered height of a font in pixels. */
    int text_height(Font *font);

    /** Render text with a solid background rectangle in screen coordinates. */
    void gl_printf(
        Font *font,
        int x,
        int y,
        const Colour &foreground,
        const Colour &background,
        int windowWidth,
        int windowHeight,
        const std::string &text);

    /** Draw UTF-8 text at a screen position without a background. */
    void textout(Font *font, int x, int y, const Colour &colour, const std::string &text);
    /** Format and draw UTF-8 text at a screen position without a background. */
    void textprintf(Font *font, int x, int y, const Colour &colour, const char *format, ...);

    /** Format and draw text with the default monospace font at a screen position. */
    void gprintf(int x, int y, const Colour &colour, const char *fmt, ...);
    /** Format and draw text with the default monospace font, horizontally centred on screen. */
    void gprintf_center(int y, const Colour &colour, const char *fmt, ...);
    /** Format and draw text with the given font at a screen position. */
    void gprintf(int x, int y,Font *font, const Colour &colour, const char *fmt, ...);
    /** Format and draw text with the given font, horizontally centred on screen. */
    void gprintf_center(int y, Font *font, const Colour &colour, const char *fmt, ...);

    
    
    /** Opaque cache holding one GPU text texture, rebuilt only when its text/font/colour change. */
    struct TextCache;

    /** Create an empty text cache. */
    TextCache *create_text_cache();
    /** Destroy a text cache and any GPU resources it holds. */
    void destroy_text_cache(TextCache *cache);
    /**
     * Draw text using a cache, without a background. The GPU texture is only
     * rebuilt when the text, font, or colour differ from the previous call.
     */
    void textout_cached(TextCache *cache, Font *font, int x, int y, const Colour &colour, const std::string &text);

} // namespace sl