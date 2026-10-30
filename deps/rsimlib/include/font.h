#pragma once

#include <SDL3/SDL.h>
#include <SDL3_TTF/SDL_ttf.h>

#include "draw.h"

#include <optional>
#include <string>
#include <vector>

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

    /**
     * Optional backing panel for text, so it stays readable over busy scenes. While enabled, text drawn by gprintf(),
     * gprintf_center(), textout(), textprintf(), textout_cached() and draw_renderer_label() sits on a translucent
     * rectangle and is drawn in `text` instead of the colour passed in. gl_printf() with its own background is unchanged.
     */
    struct TextPanelStyle
    {
        bool enabled = false;
        /** Panel fill; the alpha makes it translucent. */
        Colour background{38, 34, 58, 210};
        /** Text colour used in place of the caller's. */
        Colour text{255, 240, 170, 255};
        /** Space between the text and the panel edge, in pixels. */
        int padding_x = 5;
        int padding_y = 1;
    };
    /** Set the text panel style used by later text calls. */
    void set_text_panel_style(const TextPanelStyle &style);
    /** The style set by set_text_panel_style(); disabled by default. */
    TextPanelStyle text_panel_style();

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

    
    
    /** Draw "Renderer = <backend>" in the bottom-right corner of the screen with the default monospace font. */
    void draw_renderer_label();
    /**
     * Draw a compact stats readout (FPS, frame time, draw counts from get_frame_stats()) with its top-left corner at
     * (x, y) on the current target. Pass nullptr for the default monospace font. Call it last in the frame.
     */
    void draw_frame_stats(Font *font, int x, int y);

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

    /** Horizontal alignment of lines within a text block. */
    enum class TextAlign
    {
        left,
        center,
        right
    };
    /** Layout and decoration options for draw_text_block(). */
    struct TextStyle
    {
        TextAlign align = TextAlign::left;
        /** Wrap lines at this width in pixels (0 = only break at '\n'). The block is this wide for alignment. */
        int wrap_width = 0;
        /** Extra pixels between lines. */
        int line_spacing = 0;
        /** Draw a drop shadow behind the text. */
        bool shadow = false;
        Colour shadow_colour{0, 0, 0, 160};
        int shadow_offset_x = 1;
        int shadow_offset_y = 1;
        /** Draw an outline of outline_size pixels around the glyphs. */
        bool outline = false;
        Colour outline_colour{0, 0, 0, 255};
        int outline_size = 1;
    };
    /** Split text into lines no wider than max_width pixels, breaking at spaces and '\n' (and inside words too long to fit). max_width <= 0 only splits at '\n'. */
    std::vector<std::string> wrap_text(Font *font, const std::string &text, int max_width);
    /** Width in pixels of the block draw_text_block() would produce. */
    int text_block_width(Font *font, const std::string &text, const TextStyle &style = {});
    /** Height in pixels of the block draw_text_block() would produce. */
    int text_block_height(Font *font, const std::string &text, const TextStyle &style = {});
    /**
     * Draw wrapped, aligned text with an optional shadow or outline. (x, y) is the block's top-left corner and the
     * block is style.wrap_width wide (or as wide as its longest line when wrap_width is 0). When the text panel is
     * enabled one panel is drawn behind the whole block and the panel text colour is used.
     */
    void draw_text_block(Font *font, int x, int y, const Colour &colour, const std::string &text, const TextStyle &style = {});

} // namespace sl