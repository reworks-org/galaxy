#pragma once

#include "surface3d.h"
#include "scene3d.h"

#include <cstdint>
#include <array>
#include <memory>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace sl
{

    /** Voxel chunk dimension along each axis (X, Y, Z). */
    constexpr int CHUNK_SIZE = 16;
    /** Total number of voxels per chunk (16x16x16 = 4096). */
    constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    /** 8-bit voxel identifier. 0 is reserved for air. */
    using BlockId = std::uint8_t;
    constexpr BlockId BLOCK_AIR = 0;

    /** The 6 cube faces for culling, texturing, and raycast hit detection. */
    enum class VoxelFace : std::uint8_t
    {
        top = 0,    /**< +Y face */
        bottom = 1, /**< -Y face */
        north = 2,  /**< +Z face */
        south = 3,  /**< -Z face */
        east = 4,   /**< +X face */
        west = 5    /**< -X face */
    };

    /** Coordinate of a tile in a grid texture atlas. */
    struct VoxelTile
    {
        int x = 0;
        int y = 0;
    };

    /** Visual and physical properties of a block type. */
    struct BlockDefinition
    {
        bool is_solid = true;
        bool is_transparent = false;
        /** Per-face tile index into the texture atlas (indexed by VoxelFace). */
        VoxelTile tiles[6] = {};
        /** Base color tint multiplied into the face vertices. */
        Colour tint{255, 255, 255, 255};
        /** Emitted block-light intensity in [0..15]. */
        std::uint8_t light_emission = 0;
        /** RGB color of emitted block light. */
        Colour light_colour{255, 255, 255, 255};
        /** Light attenuation in [0..15], or 255 to infer from solidity/transparency. */
        std::uint8_t light_opacity = 255;
    };

    /** Propagated voxel light levels. Each channel is in [0..15]. */
    struct VoxelLight
    {
        std::uint8_t sky = 0;
        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        bool operator==(const VoxelLight &other) const
        {
            return sky == other.sky && red == other.red &&
                   green == other.green && blue == other.blue;
        }
    };

    /** Register a block definition in the global registry. */
    void register_block_definition(BlockId id, const BlockDefinition &def);
    /** Query a block definition (returns default air properties if unregistered). */
    const BlockDefinition &get_block_definition(BlockId id);

    /** Texture atlas metadata for mapping block face tiles to UV texture coordinates. */
    struct VoxelAtlas
    {
        Bitmap *texture = nullptr;
        int tiles_per_row = 16;
        int tiles_per_col = 16;

        /** Compute UV coordinates [u0, v0, u1, v1] for a tile (v=0 is the top). */
        void get_tile_uv(int tile_x, int tile_y, float &u0, float &v0, float &u1, float &v1) const
        {
            const float tw = 1.0f / static_cast<float>(tiles_per_row > 0 ? tiles_per_row : 1);
            const float th = 1.0f / static_cast<float>(tiles_per_col > 0 ? tiles_per_col : 1);
            u0 = static_cast<float>(tile_x) * tw;
            v0 = static_cast<float>(tile_y) * th;
            u1 = u0 + tw;
            v1 = v0 + th;
        }
    };

    /** Result returned by voxel raycasting for block picking and placement. */
    struct VoxelRaycastResult
    {
        bool hit = false;
        /** World coordinate of the targeted solid block (for breaking). */
        glm::ivec3 hit_block{0, 0, 0};
        /** World coordinate of the adjacent empty voxel (for placing). */
        glm::ivec3 place_block{0, 0, 0};
        /** Face of the block that was intersected by the ray. */
        VoxelFace hit_face = VoxelFace::top;
        /** Intersection point in world space. */
        glm::vec3 hit_pos{0.0f, 0.0f, 0.0f};
        /** Ray distance from origin to intersection. */
        float distance = 0.0f;
    };

    /**
     * A 16x16x16 chunk of voxels stored in local chunk space.
     * Manages block data and generates an optimized GPU mesh using hidden-face culling.
     */
    class VoxelChunk
    {
    public:
        explicit VoxelChunk(int chunk_x = 0, int chunk_y = 0, int chunk_z = 0);
        ~VoxelChunk();

        VoxelChunk(const VoxelChunk &) = delete;
        VoxelChunk &operator=(const VoxelChunk &) = delete;
        VoxelChunk(VoxelChunk &&other) noexcept;
        VoxelChunk &operator=(VoxelChunk &&other) noexcept;

        int chunk_x() const { return chunk_x_; }
        int chunk_y() const { return chunk_y_; }
        int chunk_z() const { return chunk_z_; }
        glm::ivec3 chunk_pos() const { return {chunk_x_, chunk_y_, chunk_z_}; }
        /** Minimum world-space corner coordinate of this chunk. */
        glm::vec3 world_origin() const
        {
            return glm::vec3(chunk_x_ * CHUNK_SIZE, chunk_y_ * CHUNK_SIZE, chunk_z_ * CHUNK_SIZE);
        }

        /** Query local block at (x, y, z) in [0..15]. Returns BLOCK_AIR if out of bounds. */
        BlockId get_block(int x, int y, int z) const;
        /** Set local block at (x, y, z) in [0..15]. Returns true if modified. */
        bool set_block(int x, int y, int z, BlockId id);
        /** Fill all voxels in this chunk with the specified block. */
        void fill(BlockId id);
        /** Returns true if this chunk contains only BLOCK_AIR. */
        bool is_empty() const { return non_air_count_ == 0; }

        /** Count visible faces that would be generated by meshing. */
        int count_visible_faces(const std::function<BlockId(int, int, int)> &world_block_lookup = nullptr) const;

        /**
         * Rebuild the GPU mesh for this chunk with hidden-face culling.
         * @param atlas Optional texture atlas for UV calculation.
         * @param world_block_lookup Optional callback(wx, wy, wz) -> BlockId for checking
         *        neighbor voxels across chunk borders. If null, borders are treated as air.
         * @param world_light_lookup Optional callback(wx, wy, wz) -> propagated light for
         *        face shading. If null, the chunk is treated as fully sky-lit.
         * @param sky_brightness Multiplier for the sky light contribution, 0 (night) to 1 (full daylight).
         */
        void rebuild_mesh(const VoxelAtlas *atlas = nullptr,
                          const std::function<BlockId(int, int, int)> &world_block_lookup = nullptr,
                          const std::function<VoxelLight(int, int, int)> &world_light_lookup = nullptr,
                          float sky_brightness = 1.0f);

        /** The current GPU mesh handle, or nullptr if empty. */
        Mesh *mesh() const { return mesh_; }
        int vertex_count() const { return mesh_ ? mesh_->vertex_count : 0; }
        int triangle_count() const { return mesh_ ? mesh_->index_count / 3 : 0; }

        /** Draw the chunk's mesh into the active 3D surface at its world origin. */
        bool draw(const Material &material = {});

    private:
        friend class VoxelWorld;

        static constexpr int index(int x, int y, int z)
        {
            return x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE;
        }

        int chunk_x_ = 0;
        int chunk_y_ = 0;
        int chunk_z_ = 0;
        std::array<BlockId, CHUNK_VOLUME> blocks_{};
        std::array<VoxelLight, CHUNK_VOLUME> lights_{};
        Mesh *mesh_ = nullptr;
        int non_air_count_ = 0;
    };

    /**
     * Multi-chunk manager coordinating chunk storage, cross-boundary neighbor queries,
     * block modification with automatic dirty-chunk updating, and 3D DDA raycasting.
     */
    class VoxelWorld
    {
    public:
        VoxelWorld();
        ~VoxelWorld();

        /** Convert world block coordinates to chunk grid coordinates. */
        static glm::ivec3 world_to_chunk(int x, int y, int z);
        /** Convert world block coordinates to chunk-local coordinates [0..15]. */
        static glm::ivec3 world_to_local(int x, int y, int z);

        /** Query block at arbitrary world coordinates. Returns BLOCK_AIR if chunk is unallocated. */
        BlockId get_block(int x, int y, int z) const;
        /** Query propagated sky and RGB block light; unallocated cells can still receive direct sky. */
        VoxelLight get_light(int x, int y, int z) const;
        /** Set block at world coordinates. Allocates chunk if needed and marks affected chunks dirty. */
        bool set_block(int x, int y, int z, BlockId id);

        VoxelChunk *get_chunk(int cx, int cy, int cz);
        const VoxelChunk *get_chunk(int cx, int cy, int cz) const;
        VoxelChunk &get_or_create_chunk(int cx, int cy, int cz);

        /** Mark a chunk as needing mesh regeneration. */
        void mark_chunk_dirty(int cx, int cy, int cz);
        /** Queue every chunk for remeshing, e.g. after the texture atlas changes. */
        void mark_all_dirty();
        /** Recalculate sky and RGB block light after world or light-emitter changes. */
        void update_lighting();
        /**
         * Scale how much sky light brightens faces, 0 (night) to 1 (daylight, the default). Block light is unaffected,
         * so torches and crystals stand out as the sky fades. Changing it queues every chunk for remeshing.
         */
        void set_sky_brightness(float brightness);
        float sky_brightness() const { return sky_brightness_; }
        /** Rebuild meshes for all dirty chunks. */
        void update_dirty_chunks(const VoxelAtlas *atlas = nullptr);

        /**
         * Fast Voxel Traversal (3D DDA) raycast through the voxel world.
         * @param origin Starting point (e.g. camera position).
         * @param direction Normalized ray direction.
         * @param max_distance Maximum reach distance in world units.
         */
        VoxelRaycastResult raycast(const glm::vec3 &origin, const glm::vec3 &direction, float max_distance = 8.0f) const;

        /** Draw all chunk meshes into the active 3D surface. */
        void draw(const Material &material = {});

        std::size_t chunk_count() const { return chunks_.size(); }
        int total_triangles() const;

    private:
        struct ChunkKey
        {
            int x;
            int y;
            int z;

            bool operator==(const ChunkKey &other) const noexcept
            {
                return x == other.x && y == other.y && z == other.z;
            }
        };

        struct ChunkKeyHash
        {
            std::size_t operator()(const ChunkKey &key) const noexcept
            {
                std::size_t hash = std::hash<int>{}(key.x);
                hash ^= std::hash<int>{}(key.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                hash ^= std::hash<int>{}(key.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                return hash;
            }
        };

        static ChunkKey chunk_key(int cx, int cy, int cz);

        std::unordered_map<ChunkKey, std::unique_ptr<VoxelChunk>, ChunkKeyHash> chunks_;
        std::unordered_set<ChunkKey, ChunkKeyHash> dirty_chunks_;
        std::unordered_map<std::uint64_t, int> highest_opaque_by_column_;
        bool lighting_dirty_ = true;
        float sky_brightness_ = 1.0f;
    };

} // namespace sl
