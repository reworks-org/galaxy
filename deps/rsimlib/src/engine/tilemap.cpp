/** @file
 * @brief Implements tile maps: layers, tile metadata, animation, batched drawing and the Tiled JSON loader.
 */
#include "tilemap.h"

#include "display.h"
#include "draw.h"
#include "error.h"
#include "isometric.h"
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstring>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <limits>
#include <map>
#define MINIZ_HEADER_FILE_ONLY
#include <miniz.h>

namespace sl
{
    namespace
    {
        constexpr std::uint32_t flip_mask = tile_flip_horizontal | tile_flip_vertical | tile_flip_diagonal;

        /** Index of the tileset owning a GID, or -1. Tilesets are kept sorted by first_gid. */
        int find_tileset(const TileMap *map, std::uint32_t gid)
        {
            for (int index = static_cast<int>(map->tilesets.size()) - 1; index >= 0; --index)
            {
                if (static_cast<std::uint32_t>(map->tilesets[index].first_gid) <= gid)
                    return index;
            }
            return -1;
        }

        /** Resolve a tileset-local tile through its animation at the map's current clock. */
        int animated_tile(const TileSet &set, int local, double clock_ms)
        {
            const auto info = set.tiles.find(local);
            if (info == set.tiles.end() || info->second.animation.empty())
                return local;
            double total = 0.0;
            for (const TileAnimationFrame &frame : info->second.animation)
                total += std::max(frame.duration_ms, 1);
            double time = std::fmod(clock_ms, total);
            for (const TileAnimationFrame &frame : info->second.animation)
            {
                time -= std::max(frame.duration_ms, 1);
                if (time < 0.0)
                    return frame.tile;
            }
            return local;
        }

        /** Resolve a cell to its source bitmap and rectangle; false when the tile cannot be drawn. */
        bool resolve_tile(const TileMap *map, std::uint32_t cell, Bitmap *&source, int &source_x, int &source_y,
                          int &width, int &height)
        {
            const std::uint32_t gid = cell & tile_id_mask;
            if (gid == 0)
                return false;
            const int set_index = find_tileset(map, gid);
            if (set_index < 0)
                return false;
            const TileSet &set = map->tilesets[set_index];
            const int local = animated_tile(set, static_cast<int>(gid) - set.first_gid, map->animation_time_ms);
            source_x = source_y = width = height = 0;
            source = set.atlas.bitmap;
            if (!set.images.empty())
            {
                const auto image = set.images.find(local);
                if (image == set.images.end() || !image->second)
                    return false;
                source = image->second;
                width = source->width;
                height = source->height;
                return true;
            }
            return atlas_tile_rect(set.atlas, local, source_x, source_y, width, height);
        }

        /** Combine a layer tint with its opacity (and any extra alpha factor) into one draw colour. */
        Colour layer_colour(Colour tint, float opacity, float extra = 1.0f)
        {
            const float alpha = std::clamp(opacity * extra, 0.0f, 1.0f) * (tint.alpha / 255.0f);
            tint.alpha = static_cast<Uint8>(std::lround(alpha * 255.0f));
            return tint;
        }

        void draw_cell(const TileMap *map, std::uint32_t cell, float x, float bottom_y, Colour tint = White)
        {
            Bitmap *source = nullptr;
            int source_x = 0, source_y = 0, width = 0, height = 0;
            if (!resolve_tile(map, cell, source, source_x, source_y, width, height))
                return;
            draw_sprite_region_flipped(source, source_x, source_y, width, height,
                                       x, bottom_y - static_cast<float>(height),
                                       static_cast<float>(width), static_cast<float>(height),
                                       (cell & tile_flip_horizontal) != 0, (cell & tile_flip_vertical) != 0,
                                       (cell & tile_flip_diagonal) != 0, tint);
        }

        void draw_image_layer(const ImageLayer &layer, float camera_x, float camera_y, int view_width, int view_height)
        {
            if (!layer.visible || !layer.bitmap || layer.bitmap->width <= 0 || layer.bitmap->height <= 0)
                return;
            const float width = static_cast<float>(layer.bitmap->width);
            const float height = static_cast<float>(layer.bitmap->height);
            float x = std::floor(layer.offset_x - camera_x * layer.parallax_x);
            float y = std::floor(layer.offset_y - camera_y * layer.parallax_y);
            if (layer.repeat_x)
            {
                x = std::fmod(x, width);
                if (x > 0.0f)
                    x -= width;
            }
            if (layer.repeat_y)
            {
                y = std::fmod(y, height);
                if (y > 0.0f)
                    y -= height;
            }
            const Colour colour = layer_colour(layer.tint, layer.opacity);
            for (float row = y; row < static_cast<float>(view_height); row += height)
            {
                for (float column = x; column < static_cast<float>(view_width); column += width)
                {
                    if (column + width > 0.0f && row + height > 0.0f)
                        draw_sprite_region_flipped(layer.bitmap, 0, 0, layer.bitmap->width, layer.bitmap->height, column,
                                                   row, width, height, false, false, false, colour);
                    if (!layer.repeat_x)
                        break;
                }
                if (!layer.repeat_y)
                    break;
            }
        }

        void draw_object_layer(const TileMap *map, const ObjectLayer &layer, float camera_x, float camera_y)
        {
            if (!layer.visible)
                return;
            const float scroll_x = std::floor(camera_x * layer.parallax_x - layer.offset_x);
            const float scroll_y = std::floor(camera_y * layer.parallax_y - layer.offset_y);
            const Colour colour = layer_colour(layer.tint, layer.opacity);
            for (const TileObject &object : layer.objects)
            {
                Bitmap *source = nullptr;
                int source_x = 0, source_y = 0, width = 0, height = 0;
                if (!object.visible || !resolve_tile(map, object.gid, source, source_x, source_y, width, height))
                    continue;
                SpriteParams params;
                params.x = object.x - scroll_x;
                params.y = object.y - scroll_y;
                // Tiled anchors tile objects at their bottom-left corner.
                params.origin_x = 0.0f;
                params.origin_y = 1.0f;
                params.scale_x = object.width > 0.0f ? object.width / static_cast<float>(width) : 1.0f;
                params.scale_y = object.height > 0.0f ? object.height / static_cast<float>(height) : 1.0f;
                params.rotation = object.rotation;
                params.flip_x = (object.gid & tile_flip_horizontal) != 0;
                params.flip_y = (object.gid & tile_flip_vertical) != 0;
                params.source_x = source_x;
                params.source_y = source_y;
                params.source_width = width;
                params.source_height = height;
                params.tint = colour;
                draw_sprite_ex(source, params);
            }
        }

        void draw_decals(const TileMap *map, int slot, float camera_x, float camera_y)
        {
            for (const TileDecal &decal : map->decals)
            {
                if (decal.after_layer != slot)
                    continue;
                Bitmap *source = nullptr;
                int source_x = 0, source_y = 0, width = 0, height = 0;
                if (!resolve_tile(map, decal.gid, source, source_x, source_y, width, height))
                    continue;
                float fade = 1.0f;
                if (decal.lifetime > 0.0f && decal.fade > 0.0f)
                    fade = std::clamp((decal.lifetime - decal.age) / decal.fade, 0.0f, 1.0f);
                SpriteParams params;
                params.x = decal.x - std::floor(camera_x);
                params.y = decal.y - std::floor(camera_y);
                params.scale_x = params.scale_y = decal.scale;
                params.rotation = decal.rotation;
                params.flip_x = (decal.gid & tile_flip_horizontal) != 0;
                params.flip_y = (decal.gid & tile_flip_vertical) != 0;
                params.source_x = source_x;
                params.source_y = source_y;
                params.source_width = width;
                params.source_height = height;
                params.tint = layer_colour(decal.tint, 1.0f, fade);
                draw_sprite_ex(source, params);
            }
        }

        /** Draw everything stacked directly above tile layer slot (-1: below all tile layers). */
        void draw_slot(const TileMap *map, int slot, float camera_x, float camera_y, int view_width, int view_height)
        {
            for (const ImageLayer &layer : map->image_layers)
            {
                if (layer.after_layer == slot)
                    draw_image_layer(layer, camera_x, camera_y, view_width, view_height);
            }
            for (const ObjectLayer &layer : map->object_layers)
            {
                if (layer.after_layer == slot)
                    draw_object_layer(map, layer, camera_x, camera_y);
            }
            draw_decals(map, slot, camera_x, camera_y);
        }

        int max_tile_height(const TileMap *map)
        {
            int result = map->tile_height;
            for (const TileSet &set : map->tilesets)
                result = std::max(result, set.atlas.tile_height);
            return result;
        }

        int max_tile_width(const TileMap *map)
        {
            int result = map->tile_width;
            for (const TileSet &set : map->tilesets)
                result = std::max(result, set.atlas.tile_width);
            return result;
        }

        int pixel_to_tile(float coordinate, int tile_size)
        {
            if (std::isnan(coordinate))
                return 0;
            const double cell = std::floor(static_cast<double>(coordinate) / tile_size);
            if (cell <= std::numeric_limits<int>::min())
                return std::numeric_limits<int>::min();
            if (cell >= std::numeric_limits<int>::max())
                return std::numeric_limits<int>::max();
            return static_cast<int>(cell);
        }

        std::uint64_t chunk_key(int chunk_x, int chunk_y)
        {
            return (static_cast<std::uint64_t>(static_cast<std::uint32_t>(chunk_x)) << 32) |
                   static_cast<std::uint32_t>(chunk_y);
        }

        int floor_div(int value, int divisor)
        {
            int quotient = value / divisor;
            if ((value % divisor != 0) && ((value < 0) != (divisor < 0)))
                --quotient;
            return quotient;
        }

        std::uint32_t layer_cell(const TileLayer &layer, int column, int row)
        {
            if (layer.infinite)
            {
                const int chunk_x = floor_div(column, tile_chunk_size);
                const int chunk_y = floor_div(row, tile_chunk_size);
                const auto found = layer.chunks.find(chunk_key(chunk_x, chunk_y));
                if (found == layer.chunks.end())
                    return 0;
                return found->second[static_cast<std::size_t>(row - chunk_y * tile_chunk_size) * tile_chunk_size +
                                     (column - chunk_x * tile_chunk_size)];
            }
            if (column < 0 || row < 0 || column >= layer.width || row >= layer.height)
                return 0;
            return layer.cells[static_cast<std::size_t>(row) * layer.width + column];
        }

        bool layer_set_cell(TileLayer &layer, int column, int row, std::uint32_t cell)
        {
            if (layer.infinite)
            {
                const int chunk_x = floor_div(column, tile_chunk_size);
                const int chunk_y = floor_div(row, tile_chunk_size);
                auto found = layer.chunks.find(chunk_key(chunk_x, chunk_y));
                if (found == layer.chunks.end())
                {
                    if (cell == 0)
                        return true;
                    found = layer.chunks.emplace(chunk_key(chunk_x, chunk_y),
                                                 std::vector<std::uint32_t>(static_cast<std::size_t>(tile_chunk_size) * tile_chunk_size, 0)).first;
                }
                found->second[static_cast<std::size_t>(row - chunk_y * tile_chunk_size) * tile_chunk_size +
                              (column - chunk_x * tile_chunk_size)] = cell;
                return true;
            }
            if (column < 0 || row < 0 || column >= layer.width || row >= layer.height)
                return false;
            layer.cells[static_cast<std::size_t>(row) * layer.width + column] = cell;
            return true;
        }

        float hex_column_width(const TileMap *map)
        {
            return map->hex_stagger_y ? static_cast<float>(map->tile_width)
                                      : (static_cast<float>(map->tile_width) + map->hex_side_length) * 0.5f;
        }

        float hex_row_height(const TileMap *map)
        {
            return map->hex_stagger_y ? (static_cast<float>(map->tile_height) + map->hex_side_length) * 0.5f
                                      : static_cast<float>(map->tile_height);
        }

        // Top-left pixel of a hexagon's bounding box.
        void hex_origin(const TileMap *map, int column, int row, float &x, float &y)
        {
            const bool shifted = (((map->hex_stagger_y ? row : column) & 1) != 0) == map->hex_stagger_odd;
            if (map->hex_stagger_y)
            {
                x = static_cast<float>(column) * map->tile_width + (shifted ? map->tile_width * 0.5f : 0.0f);
                y = static_cast<float>(row) * hex_row_height(map);
            }
            else
            {
                x = static_cast<float>(column) * hex_column_width(map);
                y = static_cast<float>(row) * map->tile_height + (shifted ? map->tile_height * 0.5f : 0.0f);
            }
        }

        IsometricTransform make_iso(const TileMap *map, float origin_x, float origin_y)
        {
            IsometricTransform iso;
            iso.config.tile_width = static_cast<float>(map->tile_width);
            iso.config.tile_height = static_cast<float>(map->tile_height);
            iso.origin_x = origin_x;
            iso.origin_y = origin_y;
            return iso;
        }
    } // namespace

    TileMap *create_tilemap(int width, int height, int tile_width, int tile_height, TileMapOrientation orientation)
    {
        if (width <= 0 || height <= 0 || tile_width <= 0 || tile_height <= 0)
        {
            detail::set_error("Invalid tile map dimensions");
            return nullptr;
        }
        TileMap *map = new TileMap;
        map->width = width;
        map->height = height;
        map->tile_width = tile_width;
        map->tile_height = tile_height;
        map->orientation = orientation;
        return map;
    }

    TileMap *create_infinite_tilemap(int tile_width, int tile_height, TileMapOrientation orientation)
    {
        if (orientation == TileMapOrientation::isometric)
        {
            detail::set_error("Infinite isometric tile maps are not supported");
            return nullptr;
        }
        TileMap *map = create_tilemap(1, 1, tile_width, tile_height, orientation);
        if (map)
        {
            map->infinite = true;
            map->width = map->height = 0;
            if (orientation == TileMapOrientation::hexagonal)
                map->hex_side_length = tile_height / 2;
        }
        return map;
    }

    TileMap *create_hex_tilemap(int width, int height, int tile_width, int tile_height, int side_length,
                                bool stagger_y, bool stagger_odd)
    {
        if (side_length < 0 || side_length > (stagger_y ? tile_height : tile_width))
        {
            detail::set_error("Invalid hexagon side length");
            return nullptr;
        }
        TileMap *map = create_tilemap(width, height, tile_width, tile_height, TileMapOrientation::hexagonal);
        if (map)
        {
            map->hex_side_length = side_length;
            map->hex_stagger_y = stagger_y;
            map->hex_stagger_odd = stagger_odd;
        }
        return map;
    }

    void destroy_tilemap(TileMap *map)
    {
        if (!map)
            return;
        for (Bitmap *bitmap : map->owned_bitmaps)
            destroy_bitmap(bitmap);
        delete map;
    }

    int tilemap_add_tileset(TileMap *map, const Atlas &atlas, int first_gid)
    {
        if (!map || !atlas.bitmap)
            return -1;
        if (first_gid <= 0)
        {
            first_gid = 1;
            if (!map->tilesets.empty())
                first_gid = map->tilesets.back().first_gid + map->tilesets.back().atlas.tile_count;
        }
        TileSet set;
        set.atlas = atlas;
        set.first_gid = first_gid;
        map->tilesets.push_back(std::move(set));
        std::stable_sort(map->tilesets.begin(), map->tilesets.end(),
                         [](const TileSet &a, const TileSet &b) { return a.first_gid < b.first_gid; });
        for (std::size_t index = 0; index < map->tilesets.size(); ++index)
        {
            if (map->tilesets[index].first_gid == first_gid)
                return static_cast<int>(index);
        }
        return -1;
    }

    int tilemap_add_layer(TileMap *map, const std::string &name)
    {
        if (!map)
            return -1;
        TileLayer layer;
        layer.name = name;
        layer.width = map->width;
        layer.height = map->height;
        layer.infinite = map->infinite;
        if (!layer.infinite)
            layer.cells.assign(static_cast<std::size_t>(map->width) * map->height, 0);
        map->layers.push_back(std::move(layer));
        return static_cast<int>(map->layers.size()) - 1;
    }

    TileLayer *tilemap_layer(TileMap *map, int layer)
    {
        if (!map || layer < 0 || layer >= static_cast<int>(map->layers.size()))
            return nullptr;
        return &map->layers[layer];
    }

    TileLayer *tilemap_layer(TileMap *map, const std::string &name)
    {
        if (!map)
            return nullptr;
        for (TileLayer &layer : map->layers)
        {
            if (layer.name == name)
                return &layer;
        }
        return nullptr;
    }

    std::uint32_t tilemap_get(const TileMap *map, int layer, int column, int row)
    {
        if (!map || layer < 0 || layer >= static_cast<int>(map->layers.size()))
            return 0;
        return layer_cell(map->layers[layer], column, row);
    }

    bool tilemap_layer_bounds(const TileMap *map, int layer, int &min_column, int &min_row, int &max_column, int &max_row)
    {
        min_column = min_row = max_column = max_row = 0;
        if (!map || layer < 0 || layer >= static_cast<int>(map->layers.size()))
            return false;
        const TileLayer &data = map->layers[layer];
        if (!data.infinite)
        {
            max_column = data.width - 1;
            max_row = data.height - 1;
            return data.width > 0 && data.height > 0;
        }
        bool found = false;
        for (const auto &[key, cells] : data.chunks)
        {
            const int chunk_x = static_cast<int>(static_cast<std::uint32_t>(key >> 32));
            const int chunk_y = static_cast<int>(static_cast<std::uint32_t>(key & 0xFFFFFFFFu));
            for (int index = 0; index < tile_chunk_size * tile_chunk_size; ++index)
            {
                if ((cells[index] & tile_id_mask) == 0)
                    continue;
                const int column = chunk_x * tile_chunk_size + index % tile_chunk_size;
                const int row = chunk_y * tile_chunk_size + index / tile_chunk_size;
                if (!found)
                {
                    min_column = max_column = column;
                    min_row = max_row = row;
                    found = true;
                }
                min_column = std::min(min_column, column);
                max_column = std::max(max_column, column);
                min_row = std::min(min_row, row);
                max_row = std::max(max_row, row);
            }
        }
        return found;
    }

    bool tilemap_set(TileMap *map, int layer, int column, int row, std::uint32_t cell)
    {
        if (!map || layer < 0 || layer >= static_cast<int>(map->layers.size()))
            return false;
        return layer_set_cell(map->layers[layer], column, row, cell);
    }

    TileInfo *tilemap_tile_info(TileMap *map, std::uint32_t gid)
    {
        gid &= tile_id_mask;
        if (!map || gid == 0)
            return nullptr;
        const int set_index = find_tileset(map, gid);
        if (set_index < 0)
            return nullptr;
        TileSet &set = map->tilesets[set_index];
        const auto found = set.tiles.find(static_cast<int>(gid) - set.first_gid);
        return found == set.tiles.end() ? nullptr : &found->second;
    }

    void tilemap_set_solid(TileMap *map, std::uint32_t gid, bool solid)
    {
        gid &= tile_id_mask;
        if (!map || gid == 0)
            return;
        const int set_index = find_tileset(map, gid);
        if (set_index < 0)
            return;
        TileSet &set = map->tilesets[set_index];
        set.tiles[static_cast<int>(gid) - set.first_gid].solid = solid;
    }

    std::string tilemap_tile_property(const TileMap *map, std::uint32_t gid, const std::string &key)
    {
        const TileInfo *info = tilemap_tile_info(const_cast<TileMap *>(map), gid);
        if (!info)
            return {};
        const auto found = info->properties.find(key);
        return found == info->properties.end() ? std::string{} : found->second;
    }

    bool tilemap_is_solid(const TileMap *map, int column, int row, int layer)
    {
        if (!map)
            return true;
        if (!map->infinite && (column < 0 || row < 0 || column >= map->width || row >= map->height))
            return true;
        const int first = layer >= 0 ? layer : 0;
        const int last = layer >= 0 ? layer : static_cast<int>(map->layers.size()) - 1;
        for (int index = first; index <= last; ++index)
        {
            const std::uint32_t cell = tilemap_get(map, index, column, row);
            const TileInfo *info = tilemap_tile_info(const_cast<TileMap *>(map), cell);
            if (info && info->solid)
                return true;
        }
        return false;
    }

    bool tilemap_rect_blocked(const TileMap *map, float x, float y, float width, float height, int layer)
    {
        if (!map || map->tile_width <= 0 || map->tile_height <= 0 || width <= 0.0f || height <= 0.0f ||
            !std::isfinite(x) || !std::isfinite(y) || !std::isfinite(width) || !std::isfinite(height) ||
            !std::isfinite(x + width) || !std::isfinite(y + height))
            return false;
        const float right = std::nextafter(x + width, -std::numeric_limits<float>::infinity());
        const float bottom = std::nextafter(y + height, -std::numeric_limits<float>::infinity());
        if (map->orientation == TileMapOrientation::hexagonal)
        {
            const float step = std::max(1.0f, std::min(hex_column_width(map), hex_row_height(map)) * 0.5f);
            for (float sy = y;; sy += step)
            {
                const float py = std::min(sy, bottom);
                for (float sx = x;; sx += step)
                {
                    int column = 0, row = 0;
                    tilemap_world_to_tile(map, std::min(sx, right), py, column, row);
                    if (tilemap_is_solid(map, column, row, layer))
                        return true;
                    if (sx >= right)
                        break;
                }
                if (sy >= bottom)
                    break;
            }
            return false;
        }
        const int first_column = pixel_to_tile(x, map->tile_width);
        const int last_column = pixel_to_tile(right, map->tile_width);
        const int first_row = pixel_to_tile(y, map->tile_height);
        const int last_row = pixel_to_tile(bottom, map->tile_height);
        for (int row = first_row; row <= last_row; ++row)
        {
            for (int column = first_column; column <= last_column; ++column)
            {
                if (tilemap_is_solid(map, column, row, layer))
                    return true;
            }
        }
        return false;
    }

    void tilemap_world_to_tile(const TileMap *map, float x, float y, int &column, int &row)
    {
        column = row = 0;
        if (!map || map->tile_width <= 0 || map->tile_height <= 0)
            return;
        if (map->orientation == TileMapOrientation::isometric)
        {
            const IsoGridPoint cell = make_iso(map, 0.0f, 0.0f).screen_to_grid(x, y);
            column = cell.x;
            row = cell.y;
            return;
        }
        if (map->orientation == TileMapOrientation::hexagonal)
        {
            const int guess_column = pixel_to_tile(x, std::max(1, static_cast<int>(hex_column_width(map))));
            const int guess_row = pixel_to_tile(y, std::max(1, static_cast<int>(hex_row_height(map))));
            float best = std::numeric_limits<float>::max();
            for (int test_row = guess_row - 1; test_row <= guess_row + 1; ++test_row)
            {
                for (int test_column = guess_column - 1; test_column <= guess_column + 1; ++test_column)
                {
                    float origin_x = 0.0f, origin_y = 0.0f;
                    hex_origin(map, test_column, test_row, origin_x, origin_y);
                    const float dx = x - (origin_x + map->tile_width * 0.5f);
                    const float dy = y - (origin_y + map->tile_height * 0.5f);
                    const float distance = dx * dx + dy * dy;
                    if (distance < best)
                    {
                        best = distance;
                        column = test_column;
                        row = test_row;
                    }
                }
            }
            return;
        }
        column = pixel_to_tile(x, map->tile_width);
        row = pixel_to_tile(y, map->tile_height);
    }

    void tilemap_tile_to_world(const TileMap *map, int column, int row, float &x, float &y)
    {
        x = y = 0.0f;
        if (!map)
            return;
        if (map->orientation == TileMapOrientation::isometric)
        {
            make_iso(map, 0.0f, 0.0f).world_to_screen(static_cast<float>(column) + 0.5f,
                                                      static_cast<float>(row) + 0.5f, 0.0f, x, y);
            return;
        }
        if (map->orientation == TileMapOrientation::hexagonal)
        {
            hex_origin(map, column, row, x, y);
            return;
        }
        x = static_cast<float>(column) * map->tile_width;
        y = static_cast<float>(row) * map->tile_height;
    }

    void update_tilemap(TileMap *map, float dt_seconds)
    {
        if (!map || dt_seconds <= 0.0f)
            return;
        map->animation_time_ms += static_cast<double>(dt_seconds) * 1000.0;
        for (TileDecal &decal : map->decals)
            decal.age += dt_seconds;
        map->decals.erase(std::remove_if(map->decals.begin(), map->decals.end(),
                                         [](const TileDecal &decal) { return decal.lifetime > 0.0f && decal.age >= decal.lifetime; }),
                          map->decals.end());
    }

    bool tilemap_add_decal(TileMap *map, int after_layer, std::uint32_t gid, float x, float y, float rotation,
                           float scale, Colour tint, float lifetime, float fade)
    {
        if (!map || (gid & tile_id_mask) == 0)
            return false;
        TileDecal decal;
        decal.gid = gid;
        decal.after_layer = std::clamp(after_layer, -1, static_cast<int>(map->layers.size()) - 1);
        decal.x = x;
        decal.y = y;
        decal.rotation = rotation;
        decal.scale = scale;
        decal.tint = tint;
        decal.lifetime = std::max(lifetime, 0.0f);
        decal.fade = std::max(fade, 0.0f);
        map->decals.push_back(decal);
        const std::size_t limit = static_cast<std::size_t>(std::max(map->max_decals, 1));
        if (map->decals.size() > limit)
            map->decals.erase(map->decals.begin(), map->decals.begin() + (map->decals.size() - limit));
        return true;
    }

    void tilemap_clear_decals(TileMap *map)
    {
        if (map)
            map->decals.clear();
    }

    void draw_tilemap(const TileMap *map, float camera_x, float camera_y, int view_width, int view_height,
                      int first_layer, int last_layer)
    {
        if (!map || map->tile_width <= 0 || map->tile_height <= 0)
            return;
        if (view_width <= 0)
            view_width = screen_width();
        if (view_height <= 0)
            view_height = screen_height();
        if (view_width <= 0 || view_height <= 0)
            return;

        first_layer = std::max(first_layer, 0);
        last_layer = last_layer < 0 ? static_cast<int>(map->layers.size()) - 1
                                    : std::min(last_layer, static_cast<int>(map->layers.size()) - 1);
        const int overhang_up = std::max(max_tile_height(map) - map->tile_height, 0);
        const int overhang_side = std::max(max_tile_width(map) - map->tile_width, 0);

        if (first_layer == 0)
            draw_slot(map, -1, camera_x, camera_y, view_width, view_height);
        for (int index = first_layer; index <= last_layer; ++index)
        {
            struct SlotGuard
            {
                const TileMap *map;
                int slot;
                float camera_x, camera_y;
                int view_width, view_height;
                ~SlotGuard() { draw_slot(map, slot, camera_x, camera_y, view_width, view_height); }
            } slot_guard{map, index, camera_x, camera_y, view_width, view_height};
            const TileLayer &layer = map->layers[index];
            if (!layer.visible || (!layer.infinite && (layer.width <= 0 || layer.height <= 0)))
                continue;
            const Colour layer_tint = layer_colour(layer.tint, layer.opacity);
            // Snap to whole pixels so neighbouring tiles never show seams.
            const float scroll_x = std::floor(camera_x * layer.parallax_x - layer.offset_x);
            const float scroll_y = std::floor(camera_y * layer.parallax_y - layer.offset_y);

            if (map->orientation == TileMapOrientation::isometric)
            {
                const IsometricTransform iso = make_iso(map, -scroll_x, -scroll_y);
                const IsoBounds bounds = iso.visible_bounds(
                    -static_cast<float>(overhang_side), 0.0f,
                    static_cast<float>(view_width + overhang_side), static_cast<float>(view_height + overhang_up));
                const int min_col = std::max(bounds.min_x, 0), max_col = std::min(bounds.max_x, layer.width - 1);
                const int min_row = std::max(bounds.min_y, 0), max_row = std::min(bounds.max_y, layer.height - 1);
                // Painter's order: ascending (column + row) so nearer tiles overdraw farther ones.
                for (int diagonal = min_col + min_row; diagonal <= max_col + max_row; ++diagonal)
                {
                    const int col_start = std::max(min_col, diagonal - max_row);
                    const int col_end = std::min(max_col, diagonal - min_row);
                    for (int column = col_start; column <= col_end; ++column)
                    {
                        const int row = diagonal - column;
                        const std::uint32_t cell = layer.cells[static_cast<std::size_t>(row) * layer.width + column];
                        if ((cell & tile_id_mask) == 0)
                            continue;
                        const IsoPoint top = iso.world_to_screen(static_cast<float>(column), static_cast<float>(row));
                        const int set_index = find_tileset(map, cell & tile_id_mask);
                        const int image_width = set_index >= 0 ? map->tilesets[set_index].atlas.tile_width : map->tile_width;
                        draw_cell(map, cell, std::floor(top.x - image_width * 0.5f),
                                  std::floor(top.y) + static_cast<float>(map->tile_height), layer_tint);
                    }
                }
                continue;
            }

            if (map->orientation == TileMapOrientation::hexagonal)
            {
                const float column_width = hex_column_width(map);
                const float row_height = hex_row_height(map);
                int min_col = static_cast<int>(std::floor(scroll_x / column_width)) - 1;
                int max_col = static_cast<int>(std::floor((scroll_x + view_width + overhang_side + map->tile_width) / column_width)) + 1;
                int min_row = static_cast<int>(std::floor(scroll_y / row_height)) - 1;
                int max_row = static_cast<int>(std::floor((scroll_y + view_height + overhang_up + map->tile_height) / row_height)) + 1;
                if (!layer.infinite)
                {
                    min_col = std::max(min_col, 0);
                    min_row = std::max(min_row, 0);
                    max_col = std::min(max_col, layer.width - 1);
                    max_row = std::min(max_row, layer.height - 1);
                }
                for (int row = min_row; row <= max_row; ++row)
                {
                    for (int column = min_col; column <= max_col; ++column)
                    {
                        const std::uint32_t cell = layer_cell(layer, column, row);
                        if ((cell & tile_id_mask) == 0)
                            continue;
                        float origin_x = 0.0f, origin_y = 0.0f;
                        hex_origin(map, column, row, origin_x, origin_y);
                        draw_cell(map, cell, origin_x - scroll_x, origin_y + static_cast<float>(map->tile_height) - scroll_y, layer_tint);
                    }
                }
                continue;
            }

            int min_col = static_cast<int>(std::floor(scroll_x / map->tile_width)) - 1;
            int max_col = static_cast<int>(std::floor((scroll_x + view_width + overhang_side) / map->tile_width));
            int min_row = static_cast<int>(std::floor(scroll_y / map->tile_height));
            int max_row = static_cast<int>(std::floor((scroll_y + view_height + overhang_up) / map->tile_height));
            if (!layer.infinite)
            {
                min_col = std::max(min_col, 0);
                min_row = std::max(min_row, 0);
                max_col = std::min(max_col, layer.width - 1);
                max_row = std::min(max_row, layer.height - 1);
            }
            for (int row = min_row; row <= max_row; ++row)
            {
                for (int column = min_col; column <= max_col; ++column)
                {
                    const std::uint32_t cell = layer_cell(layer, column, row);
                    if ((cell & tile_id_mask) == 0)
                        continue;
                    draw_cell(map, cell, static_cast<float>(column * map->tile_width) - scroll_x,
                              static_cast<float>((row + 1) * map->tile_height) - scroll_y, layer_tint);
                }
            }
        }
    }

    const TileObject *tilemap_find_object(const TileMap *map, const std::string &name)
    {
        if (!map)
            return nullptr;
        for (const ObjectLayer &layer : map->object_layers)
        {
            for (const TileObject &object : layer.objects)
            {
                if (object.name == name)
                    return &object;
            }
        }
        return nullptr;
    }

    namespace
    {
        using Json = nlohmann::json;

        bool read_json_file(const std::filesystem::path &path, Json &out)
        {
            std::ifstream file(path);
            if (!file)
            {
                detail::set_error("Unable to open Tiled file: " + path.string());
                return false;
            }
            try
            {
                file >> out;
            }
            catch (const Json::exception &exception)
            {
                detail::set_error("Invalid Tiled JSON in " + path.string() + ": " + exception.what());
                return false;
            }
            return true;
        }

        std::string value_text(const Json &value)
        {
            return value.is_string() ? value.get<std::string>() : value.dump();
        }

        std::map<std::string, std::string> read_properties(const Json &node)
        {
            std::map<std::string, std::string> result;
            const auto found = node.find("properties");
            if (found == node.end() || !found->is_array())
                return result;
            for (const Json &property : *found)
            {
                if (property.contains("name") && property.contains("value"))
                    result[property["name"].get<std::string>()] = value_text(property["value"]);
            }
            return result;
        }

        std::string class_of(const Json &node)
        {
            if (node.contains("class") && node["class"].is_string())
                return node["class"].get<std::string>();
            return node.value("type", std::string{});
        }


        // ---- Tile layer data: CSV arrays, or base64 optionally compressed with zlib or gzip ----

        bool base64_decode(const std::string &text, std::string &out)
        {
            int value = 0, bits = -8;
            for (const char c : text)
            {
                int digit;
                if (c >= 'A' && c <= 'Z')
                    digit = c - 'A';
                else if (c >= 'a' && c <= 'z')
                    digit = c - 'a' + 26;
                else if (c >= '0' && c <= '9')
                    digit = c - '0' + 52;
                else if (c == '+')
                    digit = 62;
                else if (c == '/')
                    digit = 63;
                else if (c == '=')
                    break;
                else if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
                    continue;
                else
                    return false;
                value = (value << 6) | digit;
                bits += 6;
                if (bits >= 0)
                {
                    out.push_back(static_cast<char>((value >> bits) & 0xFF));
                    bits -= 8;
                }
            }
            return true;
        }

        /** Inflate zlib or gzip data (auto-detected); `limit` caps the output size. */
        bool inflate_bytes(const std::string &in, std::size_t limit, std::string &out)
        {
            z_stream stream{};
            if (inflateInit2(&stream, 15 + 32) != Z_OK)
                return false;
            stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(in.data()));
            stream.avail_in = static_cast<uInt>(in.size());
            char buffer[16384];
            int status = Z_OK;
            while (status == Z_OK)
            {
                stream.next_out = reinterpret_cast<Bytef *>(buffer);
                stream.avail_out = sizeof(buffer);
                status = inflate(&stream, Z_NO_FLUSH);
                out.append(buffer, sizeof(buffer) - stream.avail_out);
                if (out.size() > limit)
                {
                    inflateEnd(&stream);
                    return false;
                }
            }
            inflateEnd(&stream);
            return status == Z_STREAM_END;
        }

        /** Read the cells of a layer or chunk node; `format` is the layer node holding encoding and compression. */
        bool read_cells(const Json &node, const Json &format, std::size_t expected, std::vector<std::uint32_t> &cells,
                        std::string &error)
        {
            if (!node.contains("data"))
            {
                error = "Tile layer has no data";
                return false;
            }
            const Json &data = node["data"];
            if (data.is_array())
            {
                for (const Json &cell : data)
                    cells.push_back(cell.get<std::uint32_t>());
                return true;
            }
            const std::string encoding = format.value("encoding", std::string("csv"));
            const std::string compression = format.value("compression", std::string{});
            if (!data.is_string() || encoding != "base64")
            {
                error = "Unsupported tile layer encoding '" + encoding + "'; use CSV or base64";
                return false;
            }
            if (!compression.empty() && compression != "zlib" && compression != "gzip")
            {
                error = "Unsupported tile layer compression '" + compression +
                        "'; choose zlib, gzip or none in Tiled (zstd is not supported)";
                return false;
            }
            std::string bytes;
            if (!base64_decode(data.get<std::string>(), bytes))
            {
                error = "Tile layer data is not valid base64";
                return false;
            }
            if (!compression.empty())
            {
                std::string inflated;
                if (!inflate_bytes(bytes, expected * 4 + 4, inflated))
                {
                    error = "Tile layer data could not be decompressed";
                    return false;
                }
                bytes.swap(inflated);
            }
            if (bytes.size() % 4 != 0)
            {
                error = "Tile layer data has a partial cell";
                return false;
            }
            for (std::size_t i = 0; i < bytes.size(); i += 4)
            {
                cells.push_back(static_cast<std::uint32_t>(static_cast<unsigned char>(bytes[i])) |
                                (static_cast<std::uint32_t>(static_cast<unsigned char>(bytes[i + 1])) << 8) |
                                (static_cast<std::uint32_t>(static_cast<unsigned char>(bytes[i + 2])) << 16) |
                                (static_cast<std::uint32_t>(static_cast<unsigned char>(bytes[i + 3])) << 24));
            }
            return true;
        }

        // ---- Minimal XML reader, enough for Tiled .tsx tilesets ----

        struct XmlNode
        {
            std::string name;
            std::map<std::string, std::string> attributes;
            std::vector<XmlNode> children;
            std::string text;

            std::string attr(const std::string &key, const std::string &fallback = {}) const
            {
                const auto found = attributes.find(key);
                return found == attributes.end() ? fallback : found->second;
            }
            const XmlNode *child(const std::string &tag) const
            {
                for (const XmlNode &node : children)
                    if (node.name == tag)
                        return &node;
                return nullptr;
            }
        };

        std::string xml_unescape(const std::string &text)
        {
            std::string out;
            for (std::size_t i = 0; i < text.size(); ++i)
            {
                if (text[i] != '&')
                {
                    out.push_back(text[i]);
                    continue;
                }
                const std::size_t end = text.find(';', i);
                if (end == std::string::npos || end - i > 10)
                {
                    out.push_back('&');
                    continue;
                }
                const std::string entity = text.substr(i + 1, end - i - 1);
                if (entity == "amp")
                    out.push_back('&');
                else if (entity == "lt")
                    out.push_back('<');
                else if (entity == "gt")
                    out.push_back('>');
                else if (entity == "quot")
                    out.push_back('"');
                else if (entity == "apos")
                    out.push_back('\'');
                else if (entity.size() > 1 && entity[0] == '#')
                {
                    const bool hex = entity[1] == 'x' || entity[1] == 'X';
                    const long code = std::strtol(entity.c_str() + (hex ? 2 : 1), nullptr, hex ? 16 : 10);
                    if (code > 0 && code < 0x80)
                        out.push_back(static_cast<char>(code));
                    else if (code >= 0x80 && code < 0x800)
                    {
                        out.push_back(static_cast<char>(0xC0 | (code >> 6)));
                        out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                    }
                    else if (code >= 0x800 && code < 0x10000)
                    {
                        out.push_back(static_cast<char>(0xE0 | (code >> 12)));
                        out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
                        out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                    }
                    else if (code >= 0x10000 && code < 0x110000)
                    {
                        out.push_back(static_cast<char>(0xF0 | (code >> 18)));
                        out.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
                        out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
                        out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                    }
                }
                else
                {
                    out.push_back('&');
                    continue;
                }
                i = end;
            }
            return out;
        }

        class XmlParser
        {
        public:
            explicit XmlParser(const std::string &source) : s_(source) {}

            bool parse(XmlNode &root, std::string &error)
            {
                if (s_.compare(0, 3, "\xEF\xBB\xBF") == 0)
                    pos_ = 3;
                while (skip_misc())
                {
                }
                if (pos_ >= s_.size() || s_[pos_] != '<')
                    return fail(error, "no root element");
                if (!element(root, 0))
                    return fail(error, "malformed element near offset " + std::to_string(pos_));
                return true;
            }

        private:
            const std::string &s_;
            std::size_t pos_ = 0;

            static bool fail(std::string &error, const std::string &why)
            {
                error = why;
                return false;
            }
            void skip_space()
            {
                while (pos_ < s_.size() && std::isspace(static_cast<unsigned char>(s_[pos_])))
                    ++pos_;
            }
            bool starts(const char *text) const { return s_.compare(pos_, std::strlen(text), text) == 0; }
            /** Skip one comment, declaration or doctype; false when none is next. */
            bool skip_misc()
            {
                skip_space();
                const char *close = nullptr;
                if (starts("<?"))
                    close = "?>";
                else if (starts("<!--"))
                    close = "-->";
                else if (starts("<!"))
                    close = ">";
                if (!close)
                    return false;
                const std::size_t end = s_.find(close, pos_);
                pos_ = end == std::string::npos ? s_.size() : end + std::strlen(close);
                return true;
            }
            static bool name_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-' || c == ':' || c == '.'; }

            bool element(XmlNode &node, int depth)
            {
                if (depth > 64 || pos_ >= s_.size() || s_[pos_] != '<')
                    return false;
                ++pos_;
                const std::size_t name_start = pos_;
                while (pos_ < s_.size() && name_char(s_[pos_]))
                    ++pos_;
                node.name = s_.substr(name_start, pos_ - name_start);
                if (node.name.empty())
                    return false;
                for (;;)
                {
                    skip_space();
                    if (pos_ >= s_.size())
                        return false;
                    if (s_[pos_] == '/')
                        return starts("/>") ? (pos_ += 2, true) : false;
                    if (s_[pos_] == '>')
                    {
                        ++pos_;
                        break;
                    }
                    const std::size_t key_start = pos_;
                    while (pos_ < s_.size() && name_char(s_[pos_]))
                        ++pos_;
                    const std::string key = s_.substr(key_start, pos_ - key_start);
                    skip_space();
                    if (key.empty() || pos_ >= s_.size() || s_[pos_] != '=')
                        return false;
                    ++pos_;
                    skip_space();
                    if (pos_ >= s_.size() || (s_[pos_] != '"' && s_[pos_] != '\''))
                        return false;
                    const char quote = s_[pos_++];
                    const std::size_t value_end = s_.find(quote, pos_);
                    if (value_end == std::string::npos)
                        return false;
                    node.attributes[key] = xml_unescape(s_.substr(pos_, value_end - pos_));
                    pos_ = value_end + 1;
                }
                for (;;)
                {
                    const std::size_t text_end = s_.find('<', pos_);
                    if (text_end == std::string::npos)
                        return false;
                    node.text += xml_unescape(s_.substr(pos_, text_end - pos_));
                    pos_ = text_end;
                    if (starts("</"))
                    {
                        const std::size_t end = s_.find('>', pos_);
                        if (end == std::string::npos || s_.compare(pos_ + 2, node.name.size(), node.name) != 0)
                            return false;
                        pos_ = end + 1;
                        return true;
                    }
                    if (starts("<![CDATA["))
                    {
                        const std::size_t end = s_.find("]]>", pos_);
                        if (end == std::string::npos)
                            return false;
                        node.text += s_.substr(pos_ + 9, end - pos_ - 9);
                        pos_ = end + 3;
                    }
                    else if (starts("<?") || starts("<!"))
                        skip_misc();
                    else
                    {
                        node.children.emplace_back();
                        if (!element(node.children.back(), depth + 1))
                            return false;
                    }
                }
            }
        };

        Json xml_properties(const XmlNode &owner)
        {
            Json list = Json::array();
            const XmlNode *properties = owner.child("properties");
            if (!properties)
                return list;
            for (const XmlNode &property : properties->children)
            {
                if (property.name != "property")
                    continue;
                const std::string type = property.attr("type", "string");
                std::string value = property.attributes.count("value") ? property.attr("value") : property.text;
                Json entry;
                entry["name"] = property.attr("name");
                entry["type"] = type;
                try
                {
                    if (type == "bool")
                        entry["value"] = value == "true";
                    else if (type == "int")
                        entry["value"] = std::stoll(value);
                    else if (type == "float")
                        entry["value"] = std::stod(value);
                    else
                        entry["value"] = value;
                }
                catch (const std::exception &)
                {
                    entry["value"] = value;
                }
                list.push_back(std::move(entry));
            }
            return list;
        }

        int xml_int(const XmlNode &node, const char *key, int fallback = 0)
        {
            const std::string value = node.attr(key);
            if (value.empty())
                return fallback;
            try
            {
                return std::stoi(value);
            }
            catch (const std::exception &)
            {
                return fallback;
            }
        }

        bool read_xml_file(const std::filesystem::path &path, const char *expected_root, XmlNode &root)
        {
            std::ifstream file(path, std::ios::binary);
            if (!file)
            {
                detail::set_error("Unable to open Tiled file: " + path.string());
                return false;
            }
            const std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            std::string problem;
            if (!XmlParser(text).parse(root, problem) || root.name != expected_root)
            {
                detail::set_error("Invalid Tiled file " + path.string() + ": " +
                                  (problem.empty() ? std::string("root element is not <") + expected_root + ">" : problem));
                return false;
            }
            return true;
        }

        /** Convert a <tileset> element into the JSON shape used by Tiled JSON tilesets. */
        Json tileset_to_json(const XmlNode &root)
        {
            Json out = Json::object();
            for (const char *key : {"tilewidth", "tileheight", "spacing", "margin", "tilecount", "columns"})
            {
                if (root.attributes.count(key))
                    out[key] = xml_int(root, key);
            }
            if (const XmlNode *image = root.child("image"))
                out["image"] = image->attr("source");
            Json tiles = Json::array();
            for (const XmlNode &tile : root.children)
            {
                if (tile.name != "tile")
                    continue;
                Json entry;
                entry["id"] = xml_int(tile, "id");
                Json properties = xml_properties(tile);
                if (!properties.empty())
                    entry["properties"] = std::move(properties);
                if (const XmlNode *image = tile.child("image"))
                    entry["image"] = image->attr("source");
                if (const XmlNode *animation = tile.child("animation"))
                {
                    Json frames = Json::array();
                    for (const XmlNode &frame : animation->children)
                    {
                        if (frame.name == "frame")
                            frames.push_back({{"tileid", xml_int(frame, "tileid")}, {"duration", xml_int(frame, "duration", 100)}});
                    }
                    entry["animation"] = std::move(frames);
                }
                tiles.push_back(std::move(entry));
            }
            if (!tiles.empty())
                out["tiles"] = std::move(tiles);
            return out;
        }

        bool read_tsx_file(const std::filesystem::path &path, Json &out)
        {
            XmlNode root;
            if (!read_xml_file(path, "tileset", root))
                return false;
            out = tileset_to_json(root);
            return true;
        }

        double xml_double(const XmlNode &node, const char *key, double fallback)
        {
            const std::string value = node.attr(key);
            if (value.empty())
                return fallback;
            try
            {
                return std::stod(value);
            }
            catch (const std::exception &)
            {
                return fallback;
            }
        }

        /** Tile data of a <data> element: CSV and per-tile gids become arrays, base64 stays a string. */
        bool tmx_data(const XmlNode &data, Json &into, Json &format, std::string &error)
        {
            const std::string encoding = data.attr("encoding");
            if (!encoding.empty())
                format["encoding"] = encoding;
            if (!data.attr("compression").empty())
                format["compression"] = data.attr("compression");
            if (encoding == "csv")
            {
                Json cells = Json::array();
                std::size_t i = 0;
                while (i < data.text.size())
                {
                    while (i < data.text.size() && !std::isdigit(static_cast<unsigned char>(data.text[i])))
                        ++i;
                    if (i >= data.text.size())
                        break;
                    std::size_t used = 0;
                    try
                    {
                        cells.push_back(static_cast<std::uint32_t>(std::stoull(data.text.substr(i), &used)));
                    }
                    catch (const std::exception &)
                    {
                        error = "Invalid CSV tile data";
                        return false;
                    }
                    i += used;
                }
                into["data"] = std::move(cells);
            }
            else if (encoding.empty())
            {
                Json cells = Json::array();
                for (const XmlNode &tile : data.children)
                {
                    if (tile.name == "tile")
                        cells.push_back(static_cast<std::uint32_t>(std::strtoull(tile.attr("gid", "0").c_str(), nullptr, 10)));
                }
                into["data"] = std::move(cells);
            }
            else
            {
                std::string text;
                for (const char c : data.text)
                    if (!std::isspace(static_cast<unsigned char>(c)))
                        text.push_back(c);
                into["data"] = text;
            }
            return true;
        }

        bool tmx_layers(const XmlNode &parent, Json &layers, std::string &error)
        {
            for (const XmlNode &node : parent.children)
            {
                Json entry = Json::object();
                if (node.name == "layer")
                {
                    entry["type"] = "tilelayer";
                    entry["width"] = xml_int(node, "width");
                    entry["height"] = xml_int(node, "height");
                    const XmlNode *data = node.child("data");
                    if (!data)
                    {
                        error = "Tile layer '" + node.attr("name") + "' has no data";
                        return false;
                    }
                    Json format = Json::object();
                    bool has_chunks = false;
                    Json chunks = Json::array();
                    for (const XmlNode &chunk : data->children)
                    {
                        if (chunk.name != "chunk")
                            continue;
                        has_chunks = true;
                        Json chunk_entry = Json::object();
                        chunk_entry["x"] = xml_int(chunk, "x");
                        chunk_entry["y"] = xml_int(chunk, "y");
                        chunk_entry["width"] = xml_int(chunk, "width");
                        chunk_entry["height"] = xml_int(chunk, "height");
                        XmlNode as_data = chunk;
                        as_data.attributes = data->attributes;
                        if (!tmx_data(as_data, chunk_entry, format, error))
                            return false;
                        chunks.push_back(std::move(chunk_entry));
                    }
                    if (has_chunks)
                        entry["chunks"] = std::move(chunks);
                    else if (!tmx_data(*data, entry, format, error))
                        return false;
                    for (auto it = format.begin(); it != format.end(); ++it)
                        entry[it.key()] = it.value();
                }
                else if (node.name == "objectgroup")
                {
                    entry["type"] = "objectgroup";
                    Json objects = Json::array();
                    for (const XmlNode &object : node.children)
                    {
                        if (object.name != "object")
                            continue;
                        Json item = Json::object();
                        item["name"] = object.attr("name");
                        item["class"] = object.attributes.count("class") ? object.attr("class") : object.attr("type");
                        item["x"] = xml_double(object, "x", 0.0);
                        item["y"] = xml_double(object, "y", 0.0);
                        item["width"] = xml_double(object, "width", 0.0);
                        item["height"] = xml_double(object, "height", 0.0);
                        item["rotation"] = xml_double(object, "rotation", 0.0);
                        item["visible"] = object.attr("visible", "1") != "0";
                        if (object.attributes.count("gid"))
                            item["gid"] = static_cast<std::uint32_t>(std::strtoul(object.attr("gid").c_str(), nullptr, 10));
                        Json properties = xml_properties(object);
                        if (!properties.empty())
                            item["properties"] = std::move(properties);
                        objects.push_back(std::move(item));
                    }
                    entry["objects"] = std::move(objects);
                }
                else if (node.name == "imagelayer")
                {
                    entry["type"] = "imagelayer";
                    if (const XmlNode *image = node.child("image"))
                        entry["image"] = image->attr("source");
                    entry["repeatx"] = node.attr("repeatx", "0") != "0";
                    entry["repeaty"] = node.attr("repeaty", "0") != "0";
                }
                else if (node.name == "group")
                {
                    entry["type"] = "group";
                    Json children = Json::array();
                    if (!tmx_layers(node, children, error))
                        return false;
                    entry["layers"] = std::move(children);
                }
                else
                    continue;
                entry["name"] = node.attr("name");
                entry["visible"] = node.attr("visible", "1") != "0";
                entry["offsetx"] = xml_double(node, "offsetx", 0.0);
                entry["offsety"] = xml_double(node, "offsety", 0.0);
                entry["parallaxx"] = xml_double(node, "parallaxx", 1.0);
                entry["parallaxy"] = xml_double(node, "parallaxy", 1.0);
                entry["opacity"] = xml_double(node, "opacity", 1.0);
                if (node.attributes.count("tintcolor"))
                    entry["tintcolor"] = node.attr("tintcolor");
                layers.push_back(std::move(entry));
            }
            return true;
        }

        /** Read a .tmx map into the same JSON shape as a Tiled JSON map. */
        bool read_tmx_file(const std::filesystem::path &path, Json &out, std::string &error)
        {
            XmlNode root;
            if (!read_xml_file(path, "map", root))
            {
                error = last_error();
                return false;
            }
            out = Json::object();
            out["orientation"] = root.attr("orientation", "orthogonal");
            out["infinite"] = root.attr("infinite") == "1";
            for (const char *key : {"width", "height", "tilewidth", "tileheight", "hexsidelength"})
            {
                if (root.attributes.count(key))
                    out[key] = xml_int(root, key);
            }
            if (root.attributes.count("staggeraxis"))
                out["staggeraxis"] = root.attr("staggeraxis");
            if (root.attributes.count("staggerindex"))
                out["staggerindex"] = root.attr("staggerindex");
            Json tilesets = Json::array();
            for (const XmlNode &node : root.children)
            {
                if (node.name != "tileset")
                    continue;
                Json set = node.attributes.count("source") ? Json::object() : tileset_to_json(node);
                if (node.attributes.count("source"))
                    set["source"] = node.attr("source");
                set["firstgid"] = xml_int(node, "firstgid", 1);
                tilesets.push_back(std::move(set));
            }
            out["tilesets"] = std::move(tilesets);
            Json layers = Json::array();
            if (!tmx_layers(root, layers, error))
                return false;
            out["layers"] = std::move(layers);
            return true;
        }

        bool load_tileset(TileMap *map, const Json &node, const std::filesystem::path &base_dir, int first_gid,
                          std::string &error)
        {
            Json external;
            const Json *set = &node;
            std::filesystem::path set_dir = base_dir;
            if (node.contains("source"))
            {
                const std::filesystem::path source = base_dir / node["source"].get<std::string>();
                const bool xml = source.extension() == ".tsx";
                if (!(xml ? read_tsx_file(source, external) : read_json_file(source, external)))
                {
                    error = "Unable to read tileset " + source.string();
                    return false;
                }
                set = &external;
                set_dir = source.parent_path();
            }
            Atlas atlas;
            std::unordered_map<int, Bitmap *> images;
            if (set->contains("image"))
            {
                const std::filesystem::path image_path = set_dir / (*set)["image"].get<std::string>();
                Bitmap *bitmap = load_bitmap(image_path.string());
                if (!bitmap)
                {
                    error = "Unable to load tileset image " + image_path.string();
                    return false;
                }
                map->owned_bitmaps.push_back(bitmap);
                atlas = create_atlas(bitmap, set->value("tilewidth", 0), set->value("tileheight", 0),
                                     set->value("spacing", 0), set->value("margin", 0));
                if (atlas.tile_count <= 0)
                {
                    error = "Tileset image does not contain any complete tiles: " + image_path.string();
                    return false;
                }
            }
            else
            {
                // Image-collection tileset: every tile has its own image.
                int max_id = -1, max_width = 0, max_height = 0;
                if (set->contains("tiles") && (*set)["tiles"].is_array())
                {
                    for (const Json &tile : (*set)["tiles"])
                    {
                        if (!tile.contains("image") || !tile["image"].is_string())
                            continue;
                        const std::filesystem::path image_path = set_dir / tile["image"].get<std::string>();
                        Bitmap *bitmap = load_bitmap(image_path.string());
                        if (!bitmap)
                        {
                            error = "Unable to load tile image " + image_path.string();
                            return false;
                        }
                        map->owned_bitmaps.push_back(bitmap);
                        const int id = tile.value("id", 0);
                        images[id] = bitmap;
                        max_id = std::max(max_id, id);
                        max_width = std::max(max_width, bitmap->width);
                        max_height = std::max(max_height, bitmap->height);
                    }
                }
                if (images.empty())
                {
                    error = "Tileset has neither an image nor per-tile images";
                    return false;
                }
                atlas.bitmap = images.begin()->second;
                atlas.tile_width = max_width;
                atlas.tile_height = max_height;
                atlas.tile_count = max_id + 1;
            }
            const int index = tilemap_add_tileset(map, atlas, first_gid);
            TileSet &target = map->tilesets[index];
            target.images = std::move(images);
            if (set->contains("tiles") && (*set)["tiles"].is_array())
            {
                for (const Json &tile : (*set)["tiles"])
                {
                    TileInfo &info = target.tiles[tile.value("id", 0)];
                    info.properties = read_properties(tile);
                    const auto solid = info.properties.find("solid");
                    info.solid = solid != info.properties.end() && solid->second == "true";
                    if (tile.contains("animation") && tile["animation"].is_array())
                    {
                        for (const Json &frame : tile["animation"])
                            info.animation.push_back({frame.value("tileid", 0), frame.value("duration", 100)});
                    }
                }
            }
            return true;
        }

        /** Visibility, opacity, offset, parallax and tint a Tiled group passes down to its children. */
        struct LayerStyle
        {
            bool visible = true;
            float opacity = 1.0f;
            float offset_x = 0.0f, offset_y = 0.0f;
            float parallax_x = 1.0f, parallax_y = 1.0f;
            Colour tint = White;
        };

        /** Parse a Tiled "#AARRGGBB" or "#RRGGBB" colour; false when malformed. */
        bool parse_tint(const std::string &text, Colour &out)
        {
            std::string digits = !text.empty() && text[0] == '#' ? text.substr(1) : text;
            if (digits.size() != 6 && digits.size() != 8)
                return false;
            for (const char c : digits)
            {
                if (!std::isxdigit(static_cast<unsigned char>(c)))
                    return false;
            }
            const auto value = static_cast<std::uint32_t>(std::strtoul(digits.c_str(), nullptr, 16));
            const bool has_alpha = digits.size() == 8;
            out.alpha = has_alpha ? static_cast<Uint8>(value >> 24) : 255;
            out.red = static_cast<Uint8>(value >> 16);
            out.green = static_cast<Uint8>(value >> 8);
            out.blue = static_cast<Uint8>(value);
            return true;
        }

        /** Combine a node's own style with the style inherited from its enclosing groups. */
        LayerStyle node_style(const Json &node, const LayerStyle &parent)
        {
            LayerStyle style = parent;
            style.visible = parent.visible && node.value("visible", true);
            style.opacity = parent.opacity * node.value("opacity", 1.0f);
            style.offset_x += node.value("offsetx", 0.0f);
            style.offset_y += node.value("offsety", 0.0f);
            style.parallax_x *= node.value("parallaxx", 1.0f);
            style.parallax_y *= node.value("parallaxy", 1.0f);
            Colour tint = White;
            if (node.contains("tintcolor") && node["tintcolor"].is_string() && parse_tint(node["tintcolor"].get<std::string>(), tint))
            {
                style.tint = Colour{static_cast<Uint8>(parent.tint.red * tint.red / 255),
                                    static_cast<Uint8>(parent.tint.green * tint.green / 255),
                                    static_cast<Uint8>(parent.tint.blue * tint.blue / 255),
                                    static_cast<Uint8>(parent.tint.alpha * tint.alpha / 255)};
            }
            return style;
        }

        bool load_layers(TileMap *map, const Json &layers, const std::filesystem::path &base_dir, std::string &error,
                         const LayerStyle &parent = LayerStyle{})
        {
            for (const Json &node : layers)
            {
                const std::string kind = node.value("type", std::string{});
                const LayerStyle style = node_style(node, parent);
                if (kind == "group")
                {
                    if (node.contains("layers") && !load_layers(map, node["layers"], base_dir, error, style))
                        return false;
                }
                else if (kind == "tilelayer")
                {
                    if (node.contains("chunks"))
                    {
                        if (map->orientation == TileMapOrientation::isometric)
                        {
                            error = "Infinite isometric Tiled maps are not supported";
                            return false;
                        }
                        map->infinite = true;
                        TileLayer layer;
                        layer.name = node.value("name", std::string{});
                        layer.infinite = true;
                        layer.visible = style.visible;
                        layer.offset_x = style.offset_x;
                        layer.offset_y = style.offset_y;
                        layer.parallax_x = style.parallax_x;
                        layer.parallax_y = style.parallax_y;
                        layer.opacity = style.opacity;
                        layer.tint = style.tint;
                        for (const Json &chunk : node["chunks"])
                        {
                            const int start_x = chunk.value("x", 0);
                            const int start_y = chunk.value("y", 0);
                            const int chunk_width = chunk.value("width", 0);
                            const std::size_t expected =
                                static_cast<std::size_t>(chunk_width) * static_cast<std::size_t>(std::max(chunk.value("height", 0), 0));
                            std::vector<std::uint32_t> chunk_cells;
                            if (!read_cells(chunk, node, expected, chunk_cells, error))
                                return false;
                            if (chunk_width <= 0 || chunk_cells.size() % static_cast<std::size_t>(chunk_width) != 0)
                            {
                                error = "Tile layer '" + layer.name + "' has an invalid chunk";
                                return false;
                            }
                            int index = 0;
                            for (const std::uint32_t value : chunk_cells)
                            {
                                if (value != 0)
                                    layer_set_cell(layer, start_x + index % chunk_width, start_y + index / chunk_width, value);
                                ++index;
                            }
                        }
                        map->layers.push_back(std::move(layer));
                        continue;
                    }
                    TileLayer layer;
                    layer.name = node.value("name", std::string{});
                    layer.width = node.value("width", map->width);
                    layer.height = node.value("height", map->height);
                    layer.visible = style.visible;
                    layer.offset_x = style.offset_x;
                    layer.offset_y = style.offset_y;
                    layer.parallax_x = style.parallax_x;
                    layer.parallax_y = style.parallax_y;
                    layer.opacity = style.opacity;
                    layer.tint = style.tint;
                    const std::size_t expected = static_cast<std::size_t>(std::max(layer.width, 0)) *
                                                 static_cast<std::size_t>(std::max(layer.height, 0));
                    if (!read_cells(node, node, expected, layer.cells, error))
                        return false;
                    if (layer.cells.size() != expected)
                    {
                        error = "Tile layer '" + layer.name + "' has the wrong number of cells";
                        return false;
                    }
                    map->layers.push_back(std::move(layer));
                }
                else if (kind == "objectgroup")
                {
                    ObjectLayer layer;
                    layer.name = node.value("name", std::string{});
                    layer.after_layer = static_cast<int>(map->layers.size()) - 1;
                    layer.visible = style.visible;
                    layer.opacity = style.opacity;
                    layer.tint = style.tint;
                    layer.offset_x = style.offset_x;
                    layer.offset_y = style.offset_y;
                    layer.parallax_x = style.parallax_x;
                    layer.parallax_y = style.parallax_y;
                    if (node.contains("objects"))
                    {
                        for (const Json &object : node["objects"])
                        {
                            TileObject entry;
                            entry.name = object.value("name", std::string{});
                            entry.type = class_of(object);
                            entry.x = object.value("x", 0.0f);
                            entry.y = object.value("y", 0.0f);
                            entry.width = object.value("width", 0.0f);
                            entry.height = object.value("height", 0.0f);
                            entry.rotation = object.value("rotation", 0.0f);
                            entry.visible = object.value("visible", true);
                            entry.gid = object.value("gid", 0u);
                            entry.properties = read_properties(object);
                            layer.objects.push_back(std::move(entry));
                        }
                    }
                    map->object_layers.push_back(std::move(layer));
                }
                else if (kind == "imagelayer")
                {
                    ImageLayer layer;
                    layer.name = node.value("name", std::string{});
                    layer.after_layer = static_cast<int>(map->layers.size()) - 1;
                    layer.visible = style.visible;
                    layer.opacity = style.opacity;
                    layer.tint = style.tint;
                    layer.offset_x = style.offset_x;
                    layer.offset_y = style.offset_y;
                    layer.parallax_x = style.parallax_x;
                    layer.parallax_y = style.parallax_y;
                    layer.repeat_x = node.value("repeatx", false);
                    layer.repeat_y = node.value("repeaty", false);
                    const std::string image = node.value("image", std::string{});
                    if (!image.empty())
                    {
                        const std::filesystem::path image_path = base_dir / image;
                        layer.bitmap = load_bitmap(image_path.string());
                        if (!layer.bitmap)
                        {
                            error = "Unable to load image layer " + image_path.string();
                            return false;
                        }
                        map->owned_bitmaps.push_back(layer.bitmap);
                    }
                    map->image_layers.push_back(std::move(layer));
                }
            }
            return true;
        }
    } // namespace

    TileMap *load_tiled_map(const std::string &path)
    {
        Json root;
        const std::filesystem::path map_path(path);
        if (map_path.extension() == ".tmx")
        {
            std::string tmx_error;
            if (!read_tmx_file(map_path, root, tmx_error))
            {
                if (!tmx_error.empty())
                    detail::set_error(tmx_error);
                return nullptr;
            }
        }
        else if (!read_json_file(map_path, root))
            return nullptr;

        TileMap *map = nullptr;
        std::string error;
        try
        {
            const std::string orientation = root.value("orientation", std::string("orthogonal"));
            if (orientation != "orthogonal" && orientation != "isometric" && orientation != "hexagonal")
            {
                detail::set_error("Unsupported Tiled orientation: " + orientation);
                return nullptr;
            }
            const bool infinite = root.value("infinite", false);
            map = create_tilemap(std::max(root.value("width", 0), infinite ? 1 : 0),
                                 std::max(root.value("height", 0), infinite ? 1 : 0), root.value("tilewidth", 0),
                                 root.value("tileheight", 0),
                                 orientation == "isometric"   ? TileMapOrientation::isometric
                                 : orientation == "hexagonal" ? TileMapOrientation::hexagonal
                                                              : TileMapOrientation::orthogonal);
            if (!map)
                return nullptr;
            map->infinite = infinite;
            if (orientation == "hexagonal")
            {
                map->hex_stagger_y = root.value("staggeraxis", std::string("y")) != "x";
                map->hex_stagger_odd = root.value("staggerindex", std::string("odd")) != "even";
                map->hex_side_length = root.value("hexsidelength", 0);
                if (map->hex_side_length < 0 ||
                    map->hex_side_length > (map->hex_stagger_y ? map->tile_height : map->tile_width))
                {
                    detail::set_error("Invalid Tiled hexsidelength");
                    destroy_tilemap(map);
                    return nullptr;
                }
            }
            if (infinite && orientation == "isometric")
            {
                detail::set_error("Infinite isometric Tiled maps are not supported");
                destroy_tilemap(map);
                return nullptr;
            }
            const std::filesystem::path base_dir = map_path.parent_path();
            if (root.contains("tilesets"))
            {
                for (const Json &set : root["tilesets"])
                {
                    if (!load_tileset(map, set, base_dir, set.value("firstgid", 1), error))
                    {
                        detail::set_error(error);
                        destroy_tilemap(map);
                        return nullptr;
                    }
                }
            }
            if (root.contains("layers") && !load_layers(map, root["layers"], base_dir, error))
            {
                detail::set_error(error);
                destroy_tilemap(map);
                return nullptr;
            }
        }
        catch (const Json::exception &exception)
        {
            detail::set_error(std::string("Invalid Tiled map: ") + exception.what());
            destroy_tilemap(map);
            return nullptr;
        }
        return map;
    }
} // namespace sl
