#pragma once

#include "atlas.h"
#include "draw.h"

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace sl
{
    /** Flip flags stored in the top bits of a layer cell, matching the Tiled GID flag layout. */
    constexpr std::uint32_t tile_flip_horizontal = 0x80000000u;
    /** Vertical flip flag. */
    constexpr std::uint32_t tile_flip_vertical = 0x40000000u;
    /** Diagonal (transpose) flip flag, applied before the horizontal and vertical flips. */
    constexpr std::uint32_t tile_flip_diagonal = 0x20000000u;
    /** Mask selecting the tile id (GID) from a layer cell. */
    constexpr std::uint32_t tile_id_mask = 0x0FFFFFFFu;

    /** One frame of an animated tile; tile is a tile index local to the owning tileset. */
    struct TileAnimationFrame
    {
        /** Tileset-local tile index shown during this frame. */
        int tile = 0;
        /** Frame length in milliseconds. */
        int duration_ms = 100;
    };

    /** Optional per-tile metadata, keyed by tileset-local tile index. */
    struct TileInfo
    {
        /** True when the tile blocks movement in tilemap_rect_blocked(). */
        bool solid = false;
        /** Arbitrary named properties; non-string Tiled values are stored as their JSON text. */
        std::map<std::string, std::string> properties;
        /** Animation frames; empty for static tiles. */
        std::vector<TileAnimationFrame> animation;
    };

    /** Width and height, in cells, of one storage chunk of an infinite layer. */
    constexpr int tile_chunk_size = 16;

    /** A tile atlas plus the first global tile id it covers. */
    struct TileSet
    {
        /** Tile grid; the bitmap is owned by the map only if it was loaded by load_tiled_map(). */
        Atlas atlas;
        /** Global id of this tileset's tile 0. Ids below this belong to earlier tilesets. */
        int first_gid = 1;
        /** Metadata for individual tiles. */
        std::unordered_map<int, TileInfo> tiles;
        /**
         * Per-tile images for an image-collection tileset, keyed by tileset-local id. When not empty these are drawn
         * instead of atlas cells (the bitmaps are owned by the map when it came from load_tiled_map()), and
         * atlas.tile_count is the highest id plus one.
         */
        std::unordered_map<int, Bitmap *> images;
    };

    /** A grid of tile cells; each cell is a GID plus optional flip flags, 0 meaning empty. */
    struct TileLayer
    {
        /** Layer name. */
        std::string name;
        /** Width in tiles. */
        int width = 0;
        /** Height in tiles. */
        int height = 0;
        /** Row-major cells. */
        std::vector<std::uint32_t> cells;
        /** Hidden layers are skipped by draw_tilemap(). */
        bool visible = true;
        /** Pixel offset applied to the layer. */
        float offset_x = 0.0f;
        /** Vertical pixel offset applied to the layer. */
        float offset_y = 0.0f;
        /** Camera scroll multiplier; values below 1 give a distant-background effect. */
        float parallax_x = 1.0f;
        /** Vertical camera scroll multiplier. */
        float parallax_y = 1.0f;
        /** Layer opacity from 0 (invisible) to 1; multiplied by the tint alpha. */
        float opacity = 1.0f;
        /** Colour multiplied into every tile of the layer (Tiled's tint colour); White leaves tiles unchanged. */
        Colour tint = White;
        /** True for an unbounded layer: cells live in sparse chunks (see tile_chunk_size) and may have negative coordinates. */
        bool infinite = false;
        /** Chunk storage for infinite layers, keyed by chunk coordinate; each chunk holds tile_chunk_size squared cells. */
        std::unordered_map<std::uint64_t, std::vector<std::uint32_t>> chunks;
    };

    /** A point, rectangle or tile placed in an object layer (spawn points, triggers). */
    struct TileObject
    {
        /** Object name. */
        std::string name;
        /** Object class/type string. */
        std::string type;
        /** Position in map pixels. */
        float x = 0.0f;
        /** Vertical position in map pixels. */
        float y = 0.0f;
        /** Width in pixels; 0 for points. */
        float width = 0.0f;
        /** Height in pixels; 0 for points. */
        float height = 0.0f;
        /** Tile object: GID plus flip flags, 0 for a plain object. Tile objects are drawn by draw_tilemap(). */
        std::uint32_t gid = 0;
        /** Clockwise rotation in degrees about (x, y); for a tile object that point is the bottom-left corner, as in Tiled. */
        float rotation = 0.0f;
        /** Hidden objects are not drawn. */
        bool visible = true;
        /** Custom properties. */
        std::map<std::string, std::string> properties;
    };

    /** A named list of objects. */
    struct ObjectLayer
    {
        /** Layer name. */
        std::string name;
        /** Objects in the layer. */
        std::vector<TileObject> objects;
        /** Drawn directly above tile layer after_layer; -1 draws below every tile layer. */
        int after_layer = -1;
        /** Hidden layers are not drawn. */
        bool visible = true;
        /** Layer opacity from 0 to 1. */
        float opacity = 1.0f;
        /** Colour multiplied into drawn tile objects. */
        Colour tint = White;
        /** Pixel offset applied to the layer. */
        float offset_x = 0.0f, offset_y = 0.0f;
        /** Camera scroll multipliers. */
        float parallax_x = 1.0f, parallax_y = 1.0f;
    };

    /** A picture drawn as a layer (Tiled image layer). The bitmap is owned by the map when it came from load_tiled_map(). */
    struct ImageLayer
    {
        /** Layer name. */
        std::string name;
        /** Image; nullptr draws nothing. */
        Bitmap *bitmap = nullptr;
        /** Drawn directly above tile layer after_layer; -1 draws below every tile layer. */
        int after_layer = -1;
        /** Hidden layers are not drawn. */
        bool visible = true;
        /** Layer opacity from 0 to 1. */
        float opacity = 1.0f;
        /** Colour multiplied into the image. */
        Colour tint = White;
        /** Pixel offset of the image's top-left corner. */
        float offset_x = 0.0f, offset_y = 0.0f;
        /** Camera scroll multipliers. */
        float parallax_x = 1.0f, parallax_y = 1.0f;
        /** Tile the image endlessly horizontally or vertically. */
        bool repeat_x = false, repeat_y = false;
    };

    /** A free-positioned tile stamped on the map at runtime (blood splats, scorch marks, footprints); see tilemap_add_decal(). */
    struct TileDecal
    {
        /** GID plus flip flags of the tile drawn. */
        std::uint32_t gid = 0;
        /** Drawn directly above tile layer after_layer; -1 draws below every tile layer. */
        int after_layer = -1;
        /** Centre in map pixels. */
        float x = 0.0f, y = 0.0f;
        /** Clockwise rotation in degrees. */
        float rotation = 0.0f;
        /** Uniform scale. */
        float scale = 1.0f;
        /** Colour multiplied into the tile. */
        Colour tint = White;
        /** Seconds lived so far. */
        float age = 0.0f;
        /** Total seconds to live; 0 means permanent. */
        float lifetime = 0.0f;
        /** Seconds at the end of the lifetime over which the decal fades out. */
        float fade = 0.0f;
    };

    /** How cells are projected to the screen. */
    enum class TileMapOrientation
    {
        orthogonal,
        isometric,
        /** Hexagonal cells in staggered rows or columns, as in Tiled. */
        hexagonal
    };

    /** A multi-layer tile map. Create with create_tilemap() or load_tiled_map(); free with destroy_tilemap(). */
    struct TileMap
    {
        /** Cell projection. */
        TileMapOrientation orientation = TileMapOrientation::orthogonal;
        /** Map width in tiles. */
        int width = 0;
        /** Map height in tiles. */
        int height = 0;
        /** Logical cell width in pixels (the diamond width for isometric maps). */
        int tile_width = 0;
        /** Logical cell height in pixels (the diamond height for isometric maps). */
        int tile_height = 0;
        /** Tilesets ordered by first_gid. */
        std::vector<TileSet> tilesets;
        /** Tile layers, drawn in order. */
        std::vector<TileLayer> layers;
        /** Object layers. */
        std::vector<ObjectLayer> object_layers;
        /** Bitmaps destroyed together with the map. */
        std::vector<Bitmap *> owned_bitmaps;
        /** Animation clock advanced by update_tilemap(). */
        double animation_time_ms = 0.0;
        /** True for an unbounded map; width and height are then unused and out-of-range cells are empty, not solid. */
        bool infinite = false;
        /** Hexagonal maps: length in pixels of the hexagon's flat side. */
        int hex_side_length = 0;
        /** Hexagonal maps: true when rows are staggered (pointy-top hexes), false when columns are (flat-top). */
        bool hex_stagger_y = true;
        /** Hexagonal maps: true when odd rows/columns (counting from 0) are shifted, false for even. */
        bool hex_stagger_odd = true;
        /** Image layers, interleaved with the tile layers through ImageLayer::after_layer. */
        std::vector<ImageLayer> image_layers;
        /** Live runtime decals, oldest first. */
        std::vector<TileDecal> decals;
        /** Maximum number of live decals; adding another discards the oldest. */
        int max_decals = 512;
    };

    /** Create an empty map; returns nullptr for non-positive dimensions. */
    TileMap *create_tilemap(int width, int height, int tile_width, int tile_height,
                            TileMapOrientation orientation = TileMapOrientation::orthogonal);
    /** Destroy a map and any bitmaps it owns. */
    /** Create an unbounded map (orthogonal or hexagonal); cells may be set at any coordinate, including negative ones. */
    TileMap *create_infinite_tilemap(int tile_width, int tile_height,
                                     TileMapOrientation orientation = TileMapOrientation::orthogonal);
    /**
     * Create a hexagonal map. tile_width x tile_height is the hexagon's bounding box and side_length the flat side,
     * as in Tiled. stagger_y selects pointy-top hexes in staggered rows (false: flat-top, staggered columns).
     */
    TileMap *create_hex_tilemap(int width, int height, int tile_width, int tile_height, int side_length,
                                bool stagger_y = true, bool stagger_odd = true);
    void destroy_tilemap(TileMap *map);
    /** Inclusive extent of non-empty cells in a layer; returns false when an infinite layer is empty or the index is invalid. */
    bool tilemap_layer_bounds(const TileMap *map, int layer, int &min_column, int &min_row, int &max_column, int &max_row);

    /** Append a tileset. A first_gid of 0 continues after the previous tileset. Returns its index. */
    int tilemap_add_tileset(TileMap *map, const Atlas &atlas, int first_gid = 0);
    /** Append an empty layer sized to the map. Returns its index. */
    int tilemap_add_layer(TileMap *map, const std::string &name);
    /** Return a layer by index, or nullptr. */
    TileLayer *tilemap_layer(TileMap *map, int layer);
    /** Return a layer by name, or nullptr. */
    TileLayer *tilemap_layer(TileMap *map, const std::string &name);
    /** Return the raw cell (GID plus flip flags) or 0 when out of range. */
    std::uint32_t tilemap_get(const TileMap *map, int layer, int column, int row);
    /** Set a raw cell; returns false when out of range. */
    bool tilemap_set(TileMap *map, int layer, int column, int row, std::uint32_t cell);

    /** Return the tileset metadata for a GID, or nullptr when the tile has none. */
    TileInfo *tilemap_tile_info(TileMap *map, std::uint32_t gid);
    /** Mark a tile (by GID) solid or not, creating its metadata if needed. */
    void tilemap_set_solid(TileMap *map, std::uint32_t gid, bool solid);
    /** Return a tile property, or an empty string when absent. */
    std::string tilemap_tile_property(const TileMap *map, std::uint32_t gid, const std::string &key);
    /** Return true if the cell holds a solid tile on any layer (or the given layer when layer >= 0). Out of bounds is solid. */
    bool tilemap_is_solid(const TileMap *map, int column, int row, int layer = -1);
    /** Return true if a rectangle in map pixels overlaps a solid tile (orthogonal and hexagonal maps; hex maps sample the rectangle). */
    bool tilemap_rect_blocked(const TileMap *map, float x, float y, float width, float height, int layer = -1);

    /** Convert map pixels to a cell coordinate (orthogonal; hexagonal maps pick the nearest hexagon centre). Isometric maps use the diamond projection with origin (0, 0). */
    void tilemap_world_to_tile(const TileMap *map, float x, float y, int &column, int &row);
    /** Return the pixel position of a cell's top-left corner (orthogonal and hexagonal) or the diamond centre (isometric). */
    void tilemap_tile_to_world(const TileMap *map, int column, int row, float &x, float &y);

    /**
     * Stamp a tile on the map as a decal drawn directly above tile layer after_layer (-1: below all layers; a value
     * past the last layer is clamped to it). x, y is the tile's centre in map pixels (camera-relative like layer
     * pixels, so it suits orthogonal maps). A lifetime of 0 keeps it forever; otherwise it fades out over the last
     * fade seconds. When TileMap::max_decals is reached the oldest decal is discarded. Returns false for a null map
     * or an empty gid.
     */
    bool tilemap_add_decal(TileMap *map, int after_layer, std::uint32_t gid, float x, float y, float rotation = 0.0f,
                           float scale = 1.0f, Colour tint = White, float lifetime = 0.0f, float fade = 0.5f);
    /** Remove all decals. */
    void tilemap_clear_decals(TileMap *map);

    /** Advance animated tiles and age (and expire) decals by dt seconds. */
    void update_tilemap(TileMap *map, float dt_seconds);

    /**
     * Draw visible tile layers [first_layer, last_layer] (last_layer < 0 means all), each followed by the image layers,
     * tile objects and decals placed after it; content placed below every layer (after_layer -1) draws only when first_layer is 0. The camera is the map pixel shown at the
     * top-left of the viewport; for isometric maps it is the screen-space offset of cell (0, 0)'s diamond origin.
     * A view size of 0 uses the current screen size.
     */
    void draw_tilemap(const TileMap *map, float camera_x, float camera_y, int view_width = 0, int view_height = 0,
                      int first_layer = 0, int last_layer = -1);

    /**
     * Load a Tiled map (JSON .tmj/.json, or XML .tmx) with embedded or external (.tsj/.tsx) tilesets including image collections, CSV or base64 (zlib/gzip) layer data, flips,
     * tile properties, animations, object layers (including tile objects), layer opacity and tint, group inheritance and image layers. Returns nullptr and sets the error on failure.
     */
    TileMap *load_tiled_map(const std::string &path);
    /** Return the first object with the given name across all object layers, or nullptr. */
    const TileObject *tilemap_find_object(const TileMap *map, const std::string &name);
} // namespace sl
