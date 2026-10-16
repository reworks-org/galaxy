#pragma once

#include "draw.h"

namespace sl
{

    /** A bitmap sliced into a uniform grid of equally sized tiles. */
    struct Atlas
    {
        /** Source bitmap; the atlas does not own it. */
        Bitmap *bitmap = nullptr;
        /** Width of one tile in pixels. */
        int tile_width = 0;
        /** Height of one tile in pixels. */
        int tile_height = 0;
        /** Gap between adjacent tiles in pixels. */
        int spacing = 0;
        /** Border between the bitmap edge and the first tile, in pixels. */
        int margin = 0;
        /** Number of complete tile columns. */
        int columns = 0;
        /** Number of complete tile rows. */
        int rows = 0;
        /** Total number of tiles, in row-major order. */
        int tile_count = 0;
    };

    /** Slice a bitmap into a tile grid; the atlas does not take ownership of the bitmap. */
    Atlas create_atlas(Bitmap *bitmap, int tile_width, int tile_height, int spacing = 0, int margin = 0);
    sl::Atlas load_atlas_from_file(const std::string &atlas_file, int tile_width, int tile_height);
    /** Compute a tile's source rectangle within the atlas bitmap; returns false for an out-of-range index. */
    bool atlas_tile_rect(const Atlas &atlas, int tile_index, int &x, int &y, int &width, int &height);
    /** Draw one atlas tile at the destination position, skipping fully transparent pixels. */
    void atlas_blit(const Atlas &atlas, Bitmap *destination, int tile_index, int destinationX, int destinationY);
    /** Draw one atlas tile scaled into a destination rectangle. */
    void atlas_stretch_blit(const Atlas &atlas, Bitmap *destination, int tile_index, int destinationX, int destinationY, int destinationWidth, int destinationHeight);
    /** Draw one tile with its unrotated top-left at (x, y), rotated clockwise by angleDegrees around the tile centre. */
    void atlas_rotate_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees);
    /** As above, but rotating around (pivotX, pivotY), measured in pixels from the tile's top-left corner. */
    void atlas_rotate_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees, float pivotX, float pivotY);
    /** Draw one tile scaled to destinationWidth x destinationHeight, rotated clockwise around the centre of the scaled tile. */
    void atlas_rotate_stretch_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees, float destinationWidth, float destinationHeight);
    /** As above, but rotating around (pivotX, pivotY), measured in destination pixels from the scaled tile's top-left corner. */
    void atlas_rotate_stretch_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees, float destinationWidth, float destinationHeight, float pivotX, float pivotY);

} // namespace sl
