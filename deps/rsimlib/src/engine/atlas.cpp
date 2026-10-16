#include "atlas.h"
/** @file
 * @brief Implements texture-atlas tile lookup and blitting helpers.
 */

namespace sl
{

Atlas create_atlas(Bitmap *bitmap, int tile_width, int tile_height, int spacing, int margin)
{
    Atlas atlas;
    if (!bitmap || tile_width <= 0 || tile_height <= 0 || spacing < 0 || margin < 0)
    {
        return atlas;
    }

    const int usable_width = bitmap->width - 2 * margin + spacing;
    const int usable_height = bitmap->height - 2 * margin + spacing;
    const int columns = usable_width / (tile_width + spacing);
    const int rows = usable_height / (tile_height + spacing);
    if (columns <= 0 || rows <= 0)
    {
        return atlas;
    }

    atlas.bitmap = bitmap;
    atlas.tile_width = tile_width;
    atlas.tile_height = tile_height;
    atlas.spacing = spacing;
    atlas.margin = margin;
    atlas.columns = columns;
    atlas.rows = rows;
    atlas.tile_count = columns * rows;
    return atlas;
}
sl::Atlas load_atlas_from_file(const std::string &atlas_file, int tile_width, int tile_height)
{
    sl::Bitmap *bitmap = sl::load_bitmap(atlas_file);
    sl::Atlas atlas = sl::create_atlas(bitmap, tile_width, tile_height);
    return atlas;
}
bool atlas_tile_rect(const Atlas &atlas, int tile_index, int &x, int &y, int &width, int &height)
{
    if (!atlas.bitmap || tile_index < 0 || tile_index >= atlas.tile_count)
    {
        return false;
    }

    const int column = tile_index % atlas.columns;
    const int row = tile_index / atlas.columns;
    x = atlas.margin + column * (atlas.tile_width + atlas.spacing);
    y = atlas.margin + row * (atlas.tile_height + atlas.spacing);
    width = atlas.tile_width;
    height = atlas.tile_height;
    return true;
}

void atlas_blit(const Atlas &atlas, Bitmap *destination, int tile_index, int destinationX, int destinationY)
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    if (!atlas_tile_rect(atlas, tile_index, x, y, width, height))
    {
        return;
    }
    masked_blit(atlas.bitmap, destination, x, y, destinationX, destinationY, width, height);
}

void atlas_stretch_blit(const Atlas &atlas, Bitmap *destination, int tile_index, int destinationX, int destinationY,
                        int destinationWidth, int destinationHeight)
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    if (!atlas_tile_rect(atlas, tile_index, x, y, width, height))
    {
        return;
    }
    stretch_blit(atlas.bitmap, destination, x, y, width, height, destinationX, destinationY, destinationWidth,
                 destinationHeight);
}

void atlas_rotate_stretch_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees,
                                 float destinationWidth, float destinationHeight, float pivotX, float pivotY)
{
    int sourceX = 0;
    int sourceY = 0;
    int width = 0;
    int height = 0;
    if (!atlas_tile_rect(atlas, tile_index, sourceX, sourceY, width, height))
    {
        return;
    }
    draw_sprite_region_rotated(atlas.bitmap, sourceX, sourceY, width, height, x, y, destinationWidth,
                               destinationHeight, pivotX, pivotY, angleDegrees);
}

void atlas_rotate_stretch_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees,
                                 float destinationWidth, float destinationHeight)
{
    atlas_rotate_stretch_sprite(atlas, tile_index, x, y, angleDegrees, destinationWidth, destinationHeight,
                                destinationWidth * 0.5f, destinationHeight * 0.5f);
}

void atlas_rotate_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees, float pivotX, float pivotY)
{
    atlas_rotate_stretch_sprite(atlas, tile_index, x, y, angleDegrees, static_cast<float>(atlas.tile_width),
                                static_cast<float>(atlas.tile_height), pivotX, pivotY);
}

void atlas_rotate_sprite(const Atlas &atlas, int tile_index, float x, float y, float angleDegrees)
{
    atlas_rotate_sprite(atlas, tile_index, x, y, angleDegrees, atlas.tile_width * 0.5f, atlas.tile_height * 0.5f);
}

} // namespace sl
