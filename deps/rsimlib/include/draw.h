#pragma once

#include <SDL2/SDL_stdinc.h>

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace sl
{
    class Archive;
    struct Bitmap;
}

namespace sl
{

    /** Callback signature for point-plotter algorithms (e.g. do_line, do_circle). */
    using PixelProc = std::function<void(Bitmap *bitmap, int x, int y, int d)>;

    /** An RGBA colour with 8-bit channels. */
    struct Colour
    {
        /** Red channel, in the range 0-255. */
        Uint8 red;
        /** Green channel, in the range 0-255. */
        Uint8 green;
        /** Blue channel, in the range 0-255. */
        Uint8 blue;
        /** Alpha channel, in the range 0-255; defaults to opaque. */
        Uint8 alpha = 255;
    };
    // Whites & Grays
    inline constexpr Colour White{255, 255, 255};
    inline constexpr Colour WhiteSmoke{245, 245, 245};
    inline constexpr Colour Gainsboro{220, 220, 220};
    inline constexpr Colour LightGray{211, 211, 211};
    inline constexpr Colour Silver{192, 192, 192};
    inline constexpr Colour DarkGray{169, 169, 169};
    inline constexpr Colour Gray{190, 190, 190};    // Standard X11 Gray
    inline constexpr Colour WebGray{128, 128, 128}; // W3C HTML Gray
    inline constexpr Colour DimGray{105, 105, 105};
    inline constexpr Colour Black{0, 0, 0};

    // Reds & Pinks
    inline constexpr Colour Red{255, 0, 0};
    inline constexpr Colour DarkRed{139, 0, 0};
    inline constexpr Colour Crimson{220, 20, 60};
    inline constexpr Colour Firebrick{178, 34, 34};
    inline constexpr Colour IndianRed{205, 92, 92};
    inline constexpr Colour LightCoral{240, 128, 128};
    inline constexpr Colour Salmon{250, 128, 114};
    inline constexpr Colour Pink{255, 192, 203};
    inline constexpr Colour LightPink{255, 182, 193};
    inline constexpr Colour HotPink{255, 105, 180};
    inline constexpr Colour DeepPink{255, 20, 147};

    // Oranges & Yellows
    inline constexpr Colour Orange{255, 165, 0};
    inline constexpr Colour DarkOrange{255, 140, 0};
    inline constexpr Colour OrangeRed{255, 69, 0};
    inline constexpr Colour Tomato{255, 99, 71};
    inline constexpr Colour Coral{255, 127, 80};
    inline constexpr Colour Gold{255, 215, 0};
    inline constexpr Colour Yellow{255, 255, 0};
    inline constexpr Colour LightYellow{255, 255, 224};
    inline constexpr Colour LemonChiffon{255, 250, 205};

    // Greens (Great for your console text!)
    inline constexpr Colour Green{0, 255, 0}; // X11 pure green / Lime
    inline constexpr Colour WebGreen{0, 128, 0};
    inline constexpr Colour DarkGreen{0, 100, 0};
    inline constexpr Colour LimeGreen{50, 205, 50};
    inline constexpr Colour ForestGreen{34, 139, 34};
    inline constexpr Colour SeaGreen{46, 139, 87};
    inline constexpr Colour MediumSeaGreen{60, 179, 113};
    inline constexpr Colour SpringGreen{0, 255, 127};
    inline constexpr Colour LawnGreen{124, 252, 0};
    inline constexpr Colour Chartreuse{127, 255, 0};
    inline constexpr Colour GreenYellow{173, 255, 47};
    inline constexpr Colour PaleGreen{152, 251, 152};

    // Cyans & Blues
    inline constexpr Colour Cyan{0, 255, 255}; // Same as Aqua
    inline constexpr Colour DarkCyan{0, 139, 139};
    inline constexpr Colour Teal{0, 128, 128};
    inline constexpr Colour Turquoise{64, 224, 208};
    inline constexpr Colour Aquamarine{127, 255, 212};
    inline constexpr Colour DeepSkyBlue{0, 191, 255};
    inline constexpr Colour SkyBlue{135, 206, 235};
    inline constexpr Colour LightBlue{173, 216, 230};
    inline constexpr Colour DodgerBlue{30, 144, 255};
    inline constexpr Colour RoyalBlue{65, 105, 225};
    inline constexpr Colour Blue{0, 0, 255};
    inline constexpr Colour MediumBlue{0, 0, 205};
    inline constexpr Colour DarkBlue{0, 0, 139};
    inline constexpr Colour Navy{0, 0, 128};
    inline constexpr Colour MidnightBlue{25, 25, 112};

    // Purples & Violets
    inline constexpr Colour Magenta{255, 0, 255}; // Same as Fuchsia
    inline constexpr Colour Violet{238, 130, 238};
    inline constexpr Colour Orchid{218, 112, 214};
    inline constexpr Colour DarkOrchid{153, 50, 204};
    inline constexpr Colour Purple{160, 32, 240}; // X11 Purple
    inline constexpr Colour WebPurple{128, 0, 128};
    inline constexpr Colour Indigo{75, 0, 130};
    inline constexpr Colour SlateBlue{106, 90, 205};

    // Browns & Earth Tones
    inline constexpr Colour Chocolate{210, 105, 30};
    inline constexpr Colour Peru{205, 133, 63};
    inline constexpr Colour Goldenrod{218, 165, 32};
    inline constexpr Colour DarkGoldenrod{184, 134, 11};
    inline constexpr Colour Tan{210, 180, 140};
    inline constexpr Colour Burlywood{222, 184, 135};
    inline constexpr Colour Brown{165, 42, 42};
    inline constexpr Colour Maroon{176, 48, 96}; // X11 Maroon
    inline constexpr Colour WebMaroon{128, 0, 0};
    inline constexpr Colour Olive{128, 128, 0};
    enum class BitmapKind
    {
        /** A bitmap that can retain pixel data in system memory. */
        Bitmap,
        /** The display framebuffer managed by the display subsystem. */
        Screen
    };

    /**
     * A bitmap may retain pixels in RAM, on the GPU, or both. Pixel data is RGBA8.
     * The pixel origin and all primitive coordinates are in the top-left corner.
     */
    struct Bitmap
    {
        /** Bitmap width in pixels. */
        int width = 0;
        /** Bitmap height in pixels. */
        int height = 0;
        /** CPU-side RGBA8 pixels, when retained or acquired. */
        std::vector<Uint8> pixels;
        /** Backend texture handle; zero when no GPU texture exists. */
        std::uint32_t gpu_texture = 0;
        /** Non-zero if this bitmap is an offscreen render target (see create_render_target()). */
        std::uint32_t fbo = 0;
        /** True when CPU pixels contain changes not yet uploaded to the GPU. */
        bool ram_dirty = false;
        /** True when the GPU texture contains changes not yet downloaded to RAM. */
        bool gpu_dirty = false;
        /** Distinguishes ordinary bitmaps from the display framebuffer. */
        BitmapKind kind = BitmapKind::Bitmap;
    };

    /** The current display framebuffer, owned by the display subsystem. */
    extern Bitmap *screen;

    /** Create a RAM-backed bitmap, or return nullptr for invalid dimensions. */
    Bitmap *create_bitmap(int width, int height);
    /** Create a GPU-backed bitmap, or return nullptr if allocation fails. */
    Bitmap *create_video_bitmap(int width, int height);
    /**
     * Create an offscreen render target: a GPU texture with a framebuffer attached,
     * usable as a draw destination for sprites/particles/text (see begin_render_target()).
     * Note: draw_sprite() automatically accounts for backend-specific framebuffer
     * texture orientation when compositing a render target.
     */
    Bitmap *create_render_target(int width, int height);
    /** Redirect subsequent sprite/particle/text drawing to a render target created with create_render_target(). */
    bool begin_render_target(Bitmap *target);
    /** Stop rendering to a target and restore drawing to the window. */
    void end_render_target();
    /** Clear the currently bound render target (call between begin_render_target() and end_render_target()). */
    void clear_render_target(Colour colour);
    /** Load an image file into a bitmap, or return nullptr on failure. */
    Bitmap *load_bitmap(const std::string &path);
    /** Load an image from memory. */
    Bitmap *load_bitmap_from_memory(const std::uint8_t *data, std::size_t size);
    /** Load an image entry from a ZIP archive. */
    Bitmap *load_bitmap(const Archive &archive, const std::string &name);
    /** Save a bitmap as an uncompressed PNG, appending .png when no extension is supplied. */
    bool save_bitmap(Bitmap *bitmap, const std::string &path);
    /** Destroy a bitmap created by this module. */
    void destroy_bitmap(Bitmap *bitmap);

    /** Acquire a bitmap for CPU access. */
    bool acquire_bitmap(Bitmap *bitmap);
    /** Release a bitmap acquired for CPU access. */
    bool release_bitmap(Bitmap *bitmap);

    /** Fill a bitmap with one colour. */
    void clear_to_colour(Bitmap *bitmap, Colour colour);
    /** Set one pixel if its coordinates are inside the bitmap. */
    void putpixel(Bitmap *bitmap, int x, int y, Colour colour);
    /** Read one pixel, returning a zero colour for invalid coordinates. */
    Colour getpixel(Bitmap *bitmap, int x, int y);
    /** One screen-space point for draw_points(), e.g. a GPU-simulated particle. */
    struct PointVertex
    {
        /** Horizontal screen coordinate in pixels. */
        float x = 0.0f;
        /** Vertical screen coordinate in pixels. */
        float y = 0.0f;
        /** Point colour. */
        Colour colour;
    };
    /** Draw many single-pixel points to the screen in one batched GPU submission. */
    void draw_points(Bitmap *bitmap, const PointVertex *points, int count);
    /** Colour blending mode used by subsequent draw calls on the screen. */
    enum class BlendMode
    {
        normal,
        additive
    };
    /** Select the blend mode used when compositing subsequent draws onto the screen. */
    void set_blend_mode(BlendMode mode);
    /** Draw an outline circle. */
    void circle(Bitmap *bitmap, float x, float y, float radius, Colour colour, float thickness = 1.0f);
    /** Draw a filled circle. */
    void circlefill(Bitmap *bitmap, float x, float y, float radius, Colour colour);
    /** Draw an outline rectangle using inclusive corner coordinates. */
    void rect(Bitmap *bitmap, float left, float top, float right, float bottom, Colour colour, float thickness = 1.0f);
    /** Draw a filled rectangle using inclusive corner coordinates. */
    void rectfill(Bitmap *bitmap, float left, float top, float right, float bottom, Colour colour);
    /** Draw an outline ellipse. */
    void ellipse(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Colour colour, float thickness = 1.0f);
    /** Draw a filled ellipse. */
    void ellipsefill(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Colour colour);
    /** Draw an outline triangle. */
    void triangle(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Colour colour, float thickness = 1.0f);
    /** Draw a filled triangle. */
    void trianglefill(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Colour colour);
    /** Draw a line between two points. */
    void line(Bitmap *bitmap, float x1, float y1, float x2, float y2, Colour colour, float thickness = 1.0f);
    /** Draw a circular arc outline. Angles startAngle and endAngle are in degrees. */
    void arc(Bitmap *bitmap, float x, float y, float startAngle, float endAngle, float radius, Colour colour, float thickness = 1.0f);
    /** Draw a 4-point cubic Bézier spline curve. Points array format: [x0, y0, x1, y1, x2, y2, x3, y3]. */
    void spline(Bitmap *bitmap, const int points[8], Colour colour, float thickness = 1.0f);
    /** Draw a 4-point cubic Bézier spline curve with floating-point coordinates. */
    void spline(Bitmap *bitmap, const float points[8], Colour colour, float thickness = 1.0f);

    /** Calculate points along a 4-point cubic Bézier spline into xout and yout arrays. Returns count calculated. */
    int calc_spline(const int points[8], int npts, int *xout, int *yout);
    /** Calculate floating-point coordinates along a 4-point cubic Bézier spline. */
    int calc_spline(const float points[8], int npts, float *xout, float *yout);

    /** Allegro-style rasterizer callback plotters: invoke proc(bitmap, x, y, d) for each point along the shape. */
    void do_line(Bitmap *bitmap, int x1, int y1, int x2, int y2, int d, const PixelProc &proc);
    void do_circle(Bitmap *bitmap, int x, int y, int radius, int d, const PixelProc &proc);
    void do_ellipse(Bitmap *bitmap, int x, int y, int radiusX, int radiusY, int d, const PixelProc &proc);
    void do_arc(Bitmap *bitmap, int x, int y, float startAngle, float endAngle, float radius, int d, const PixelProc &proc);
    void do_spline(Bitmap *bitmap, const int points[8], int d, const PixelProc &proc);

    /** Flood-fill an enclosed area of a bitmap starting at (x, y) with a replacement colour. */
    void flood_fill(Bitmap *bitmap, int x, int y, Colour colour);

    /** Draw a textured circle outline on the screen bitmap. */
    void circle(Bitmap *bitmap, float x, float y, float radius, Bitmap *texture, float thickness = 1.0f);
    /** Draw a textured filled circle on the screen bitmap. */
    void circlefill(Bitmap *bitmap, float x, float y, float radius, Bitmap *texture);
    /** Draw a textured rectangle outline on the screen bitmap. */
    void rect(Bitmap *bitmap, float left, float top, float right, float bottom, Bitmap *texture, float thickness = 1.0f);
    /** Draw a textured filled rectangle on the screen bitmap. */
    void rectfill(Bitmap *bitmap, float left, float top, float right, float bottom, Bitmap *texture);
    /** Draw a textured ellipse outline on the screen bitmap. */
    void ellipse(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Bitmap *texture, float thickness = 1.0f);
    /** Draw a textured filled ellipse on the screen bitmap. */
    void ellipsefill(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Bitmap *texture);
    /** Draw a textured triangle outline on the screen bitmap. */
    void triangle(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Bitmap *texture, float thickness = 1.0f);
    /** Draw a textured filled triangle on the screen bitmap. */
    void trianglefill(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Bitmap *texture);
    /** Copy pixels while treating fully transparent source pixels as transparent. */
    void masked_blit(Bitmap *source, Bitmap *destination, int sourceX, int sourceY, float destinationX, float destinationY, int width, int height);
    /** Scale and copy a bitmap region into a destination region. */
    void stretch_blit(Bitmap *source, Bitmap *destination, int sourceX, int sourceY, int sourceWidth, int sourceHeight, float destinationX, float destinationY, int destinationWidth, int destinationHeight);
    /** Create a bitmap containing a copied rectangular region. */
    Bitmap *create_sub_bitmap(Bitmap *parent, int x, int y, int width, int height);
    /** Copy a rectangular bitmap region without scaling. */
    void blit(Bitmap *source, Bitmap *destination, int sourceX, int sourceY, float destinationX, float destinationY, int width, int height);

    /** Synchronize a bitmap's RAM pixels to its GPU texture. */
    bool upload_bitmap(Bitmap *bitmap);
    /** Synchronize a bitmap's GPU texture to RAM pixels. */
    bool download_bitmap(Bitmap *bitmap);
    /** Bind a bitmap's GPU texture to an active texture unit (0..N-1). Pass nullptr to unbind. */
    bool bind_texture(unsigned int unit, Bitmap *bitmap);
    /** Draw a bitmap at the supplied top-left screen position. Render targets are backend-corrected. */
    void draw_sprite(Bitmap *bitmap, float x, float y);
    /** Draw a stretched sprite */
    void draw_sprite_stretched(Bitmap *bitmap, float x, float y, int width, int height);
    /** Draw a sprite centred at (centerX, centerY), rotated clockwise by angleDegrees. */
    void draw_sprite_rotated(Bitmap *bitmap, float centerX, float centerY, float angleDegrees);
    /** Draw a stretched sprite centred at (centerX, centerY), rotated clockwise by angleDegrees. */
    void draw_sprite_rotated_stretched(Bitmap *bitmap, float centerX, float centerY, float angleDegrees, int width, int height);
    /** Draw a bitmap region with its unrotated top-left at (x, y), rotated clockwise by angleDegrees around a pivot offset (in destination pixels) from that corner; the region is scaled to destinationWidth x destinationHeight. */
    void draw_sprite_region_rotated(Bitmap *bitmap, int sourceX, int sourceY, int width, int height, float x, float y, float destinationWidth, float destinationHeight, float pivotX, float pivotY, float angleDegrees);
    /** Draw a horizontally flipped bitmap. */
    void draw_sprite_h_flip(Bitmap *bitmap, float x, float y);
    /** Draw a vertically flipped bitmap. */
    void draw_sprite_v_flip(Bitmap *bitmap, float x, float y);

    namespace detail
    {

        /** Create the display-owned screen bitmap. */
        void initialise_screen(int width, int height);
        /** Resize the display-owned screen bitmap. */
        void resize_screen(int width, int height);
        /** Destroy the display-owned screen bitmap. */
        void destroy_screen();

    } // namespace detail

} // namespace sl