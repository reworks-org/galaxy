#include "voxel.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace sl
{
    namespace
    {
        inline int floor_div(int a, int b)
        {
            int res = a / b;
            int rem = a % b;
            if (rem != 0 && ((a < 0) ^ (b < 0)))
            {
                res -= 1;
            }
            return res;
        }

        inline int positive_mod(int a, int b)
        {
            int rem = a % b;
            return rem < 0 ? rem + b : rem;
        }

        struct GlobalBlockRegistry
        {
            std::unordered_map<BlockId, BlockDefinition> definitions;
            BlockDefinition air_definition{false, true, {}, {0, 0, 0, 0}};
            BlockDefinition default_solid_definition{true, false, {}, {255, 255, 255, 255}};
        };

        GlobalBlockRegistry &registry()
        {
            static GlobalBlockRegistry reg;
            return reg;
        }

        // Face geometric data
        struct FaceInfo
        {
            int dx, dy, dz;
            float normal[3];
            float brightness;
            // 4 vertices per face, local offset in [0..1]
            float corners[4][3];
        };

        const FaceInfo FACES[6] = {
            // top (+Y)
            {
                0, 1, 0,
                {0.0f, 1.0f, 0.0f},
                1.0f,
                {
                    {0.0f, 1.0f, 1.0f},
                    {1.0f, 1.0f, 1.0f},
                    {1.0f, 1.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f}
                }
            },
            // bottom (-Y)
            {
                0, -1, 0,
                {0.0f, -1.0f, 0.0f},
                0.55f,
                {
                    {0.0f, 0.0f, 0.0f},
                    {1.0f, 0.0f, 0.0f},
                    {1.0f, 0.0f, 1.0f},
                    {0.0f, 0.0f, 1.0f}
                }
            },
            // north (+Z)
            {
                0, 0, 1,
                {0.0f, 0.0f, 1.0f},
                0.85f,
                {
                    {0.0f, 0.0f, 1.0f},
                    {1.0f, 0.0f, 1.0f},
                    {1.0f, 1.0f, 1.0f},
                    {0.0f, 1.0f, 1.0f}
                }
            },
            // south (-Z)
            {
                0, 0, -1,
                {0.0f, 0.0f, -1.0f},
                0.85f,
                {
                    {1.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f},
                    {1.0f, 1.0f, 0.0f}
                }
            },
            // east (+X)
            {
                1, 0, 0,
                {1.0f, 0.0f, 0.0f},
                0.72f,
                {
                    {1.0f, 0.0f, 1.0f},
                    {1.0f, 0.0f, 0.0f},
                    {1.0f, 1.0f, 0.0f},
                    {1.0f, 1.0f, 1.0f}
                }
            },
            // west (-X)
            {
                -1, 0, 0,
                {-1.0f, 0.0f, 0.0f},
                0.72f,
                {
                    {0.0f, 0.0f, 0.0f},
                    {0.0f, 0.0f, 1.0f},
                    {0.0f, 1.0f, 1.0f},
                    {0.0f, 1.0f, 0.0f}
                }
            }
        };

        // UV coordinates mapping to corners:
        // corner 0: bottom-left (u0, v1)
        // corner 1: bottom-right (u1, v1)
        // corner 2: top-right (u1, v0)
        // corner 3: top-left (u0, v0)
        const float UV_CORNERS[4][2] = {
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        };

        inline bool is_face_visible(BlockId current, BlockId neighbor)
        {
            if (neighbor == BLOCK_AIR)
                return true;
            const BlockDefinition &neigh_def = get_block_definition(neighbor);
            if (!neigh_def.is_solid)
                return true;
            if (neigh_def.is_transparent && neighbor != current)
                return true;
            return false;
        }

        int block_light_opacity(BlockId id)
        {
            if (id == BLOCK_AIR)
                return 1;
            const BlockDefinition &definition = get_block_definition(id);
            if (definition.light_opacity <= 15)
                return definition.light_opacity;
            return definition.is_solid && !definition.is_transparent ? 15 : 1;
        }

        // Block light is boosted well past 1.0 (the framebuffer clamps) so emitters read as bright sources
        constexpr float BLOCK_LIGHT_GAIN = 2.0f;

        // Perceptual curve so mid-range light levels stay readable instead of fading linearly to black
        float light_curve(std::uint8_t level)
        {
            return std::pow(static_cast<float>(level) / 15.0f, 0.6f);
        }

        std::uint8_t attenuate_light(std::uint8_t level, int opacity)
        {
            return level > opacity ? static_cast<std::uint8_t>(level - opacity) : 0;
        }

        std::uint64_t column_key(int x, int z)
        {
            return (static_cast<std::uint64_t>(static_cast<std::uint32_t>(x)) << 32) |
                   static_cast<std::uint32_t>(z);
        }
    } // namespace

    void register_block_definition(BlockId id, const BlockDefinition &def)
    {
        registry().definitions[id] = def;
    }

    const BlockDefinition &get_block_definition(BlockId id)
    {
        if (id == BLOCK_AIR)
            return registry().air_definition;
        const auto it = registry().definitions.find(id);
        if (it != registry().definitions.end())
            return it->second;
        return registry().default_solid_definition;
    }

    // -------------------------------------------------------------------------
    // VoxelChunk
    // -------------------------------------------------------------------------

    VoxelChunk::VoxelChunk(int chunk_x, int chunk_y, int chunk_z)
        : chunk_x_(chunk_x), chunk_y_(chunk_y), chunk_z_(chunk_z)
    {
        blocks_.fill(BLOCK_AIR);
    }

    VoxelChunk::~VoxelChunk()
    {
        if (mesh_)
        {
            destroy_mesh(mesh_);
            mesh_ = nullptr;
        }
    }

    VoxelChunk::VoxelChunk(VoxelChunk &&other) noexcept
        : chunk_x_(other.chunk_x_),
          chunk_y_(other.chunk_y_),
          chunk_z_(other.chunk_z_),
          blocks_(other.blocks_),
          lights_(other.lights_),
          mesh_(other.mesh_),
          non_air_count_(other.non_air_count_)
    {
        other.mesh_ = nullptr;
        other.non_air_count_ = 0;
    }

    VoxelChunk &VoxelChunk::operator=(VoxelChunk &&other) noexcept
    {
        if (this != &other)
        {
            if (mesh_)
                destroy_mesh(mesh_);
            chunk_x_ = other.chunk_x_;
            chunk_y_ = other.chunk_y_;
            chunk_z_ = other.chunk_z_;
            blocks_ = other.blocks_;
            lights_ = other.lights_;
            mesh_ = other.mesh_;
            non_air_count_ = other.non_air_count_;
            other.mesh_ = nullptr;
            other.non_air_count_ = 0;
        }
        return *this;
    }

    BlockId VoxelChunk::get_block(int x, int y, int z) const
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
            return BLOCK_AIR;
        return blocks_[index(x, y, z)];
    }

    bool VoxelChunk::set_block(int x, int y, int z, BlockId id)
    {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
            return false;
        const int idx = index(x, y, z);
        const BlockId old_id = blocks_[idx];
        if (old_id == id)
            return false;
        blocks_[idx] = id;
        if (old_id == BLOCK_AIR && id != BLOCK_AIR)
            ++non_air_count_;
        else if (old_id != BLOCK_AIR && id == BLOCK_AIR)
            --non_air_count_;
        return true;
    }

    void VoxelChunk::fill(BlockId id)
    {
        blocks_.fill(id);
        non_air_count_ = (id == BLOCK_AIR) ? 0 : CHUNK_VOLUME;
    }

    int VoxelChunk::count_visible_faces(const std::function<BlockId(int, int, int)> &world_block_lookup) const
    {
        if (non_air_count_ == 0)
            return 0;

        int count = 0;
        const int world_base_x = chunk_x_ * CHUNK_SIZE;
        const int world_base_y = chunk_y_ * CHUNK_SIZE;
        const int world_base_z = chunk_z_ * CHUNK_SIZE;

        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                for (int x = 0; x < CHUNK_SIZE; ++x)
                {
                    const BlockId current = blocks_[index(x, y, z)];
                    if (current == BLOCK_AIR)
                        continue;

                    for (int face_idx = 0; face_idx < 6; ++face_idx)
                    {
                        const FaceInfo &face = FACES[face_idx];
                        const int nx = x + face.dx;
                        const int ny = y + face.dy;
                        const int nz = z + face.dz;

                        BlockId neighbor = BLOCK_AIR;
                        if (nx >= 0 && nx < CHUNK_SIZE &&
                            ny >= 0 && ny < CHUNK_SIZE &&
                            nz >= 0 && nz < CHUNK_SIZE)
                        {
                            neighbor = blocks_[index(nx, ny, nz)];
                        }
                        else if (world_block_lookup)
                        {
                            neighbor = world_block_lookup(world_base_x + nx,
                                                          world_base_y + ny,
                                                          world_base_z + nz);
                        }

                        if (is_face_visible(current, neighbor))
                        {
                            ++count;
                        }
                    }
                }
            }
        }
        return count;
    }

    void VoxelChunk::rebuild_mesh(const VoxelAtlas *atlas,
                                  const std::function<BlockId(int, int, int)> &world_block_lookup,
                                  const std::function<VoxelLight(int, int, int)> &world_light_lookup,
                                 float sky_brightness)
    {
        if (mesh_)
        {
            destroy_mesh(mesh_);
            mesh_ = nullptr;
        }

        if (non_air_count_ == 0)
            return;

        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
        // Conservative reservation based on typical surface faces
        vertices.reserve(1024);
        indices.reserve(1536);

        const int world_base_x = chunk_x_ * CHUNK_SIZE;
        const int world_base_y = chunk_y_ * CHUNK_SIZE;
        const int world_base_z = chunk_z_ * CHUNK_SIZE;

        for (int z = 0; z < CHUNK_SIZE; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE; ++y)
            {
                for (int x = 0; x < CHUNK_SIZE; ++x)
                {
                    const BlockId current = blocks_[index(x, y, z)];
                    if (current == BLOCK_AIR)
                        continue;

                    const BlockDefinition &def = get_block_definition(current);

                    for (int face_idx = 0; face_idx < 6; ++face_idx)
                    {
                        const FaceInfo &face = FACES[face_idx];
                        const int nx = x + face.dx;
                        const int ny = y + face.dy;
                        const int nz = z + face.dz;

                        BlockId neighbor = BLOCK_AIR;
                        if (nx >= 0 && nx < CHUNK_SIZE &&
                            ny >= 0 && ny < CHUNK_SIZE &&
                            nz >= 0 && nz < CHUNK_SIZE)
                        {
                            neighbor = blocks_[index(nx, ny, nz)];
                        }
                        else if (world_block_lookup)
                        {
                            neighbor = world_block_lookup(world_base_x + nx,
                                                          world_base_y + ny,
                                                          world_base_z + nz);
                        }

                        if (!is_face_visible(current, neighbor))
                            continue;

                        // Calculate UVs for this face
                        float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
                        if (atlas)
                        {
                            const VoxelTile tile = def.tiles[face_idx];
                            atlas->get_tile_uv(tile.x, tile.y, u0, v0, u1, v1);
                        }

                        VoxelLight light{15, 0, 0, 0};
                        if (world_light_lookup)
                        {
                            light = world_light_lookup(world_base_x + nx,
                                                       world_base_y + ny,
                                                       world_base_z + nz);
                        }
                        const float sky_scale = std::clamp(sky_brightness, 0.0f, 1.0f);
                        const float light_red = std::max(sky_scale * light_curve(light.sky), BLOCK_LIGHT_GAIN * light_curve(light.red));
                        const float light_green = std::max(sky_scale * light_curve(light.sky), BLOCK_LIGHT_GAIN * light_curve(light.green));
                        const float light_blue = std::max(sky_scale * light_curve(light.sky), BLOCK_LIGHT_GAIN * light_curve(light.blue));
                        const float r = (def.tint.red / 255.0f) * face.brightness * light_red;
                        const float g = (def.tint.green / 255.0f) * face.brightness * light_green;
                        const float b = (def.tint.blue / 255.0f) * face.brightness * light_blue;
                        const float a = def.tint.alpha / 255.0f;

                        const std::uint32_t base_index = static_cast<std::uint32_t>(vertices.size());

                        for (int c = 0; c < 4; ++c)
                        {
                            Vertex3D v;
                            v.position[0] = static_cast<float>(x) + face.corners[c][0];
                            v.position[1] = static_cast<float>(y) + face.corners[c][1];
                            v.position[2] = static_cast<float>(z) + face.corners[c][2];
                            v.normal[0] = face.normal[0];
                            v.normal[1] = face.normal[1];
                            v.normal[2] = face.normal[2];
                            v.colour[0] = r;
                            v.colour[1] = g;
                            v.colour[2] = b;
                            v.colour[3] = a;
                            v.uv[0] = (UV_CORNERS[c][0] == 0.0f) ? u0 : u1;
                            v.uv[1] = (UV_CORNERS[c][1] == 0.0f) ? v0 : v1;
                            vertices.push_back(v);
                        }

                        // Quad indices: (0, 1, 2) and (0, 2, 3)
                        indices.push_back(base_index + 0);
                        indices.push_back(base_index + 1);
                        indices.push_back(base_index + 2);
                        indices.push_back(base_index + 0);
                        indices.push_back(base_index + 2);
                        indices.push_back(base_index + 3);
                    }
                }
            }
        }

        if (!vertices.empty() && !indices.empty())
        {
            mesh_ = create_mesh(vertices, indices);
        }
    }

    bool VoxelChunk::draw(const Material &material)
    {
        if (!mesh_)
            return true;
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), world_origin());
        return draw_mesh(mesh_, transform, material);
    }

    // -------------------------------------------------------------------------
    // VoxelWorld
    // -------------------------------------------------------------------------

    VoxelWorld::VoxelWorld() = default;
    VoxelWorld::~VoxelWorld() = default;

    glm::ivec3 VoxelWorld::world_to_chunk(int x, int y, int z)
    {
        return {floor_div(x, CHUNK_SIZE), floor_div(y, CHUNK_SIZE), floor_div(z, CHUNK_SIZE)};
    }

    glm::ivec3 VoxelWorld::world_to_local(int x, int y, int z)
    {
        return {positive_mod(x, CHUNK_SIZE), positive_mod(y, CHUNK_SIZE), positive_mod(z, CHUNK_SIZE)};
    }

    VoxelWorld::ChunkKey VoxelWorld::chunk_key(int cx, int cy, int cz)
    {
        return {cx, cy, cz};
    }

    BlockId VoxelWorld::get_block(int x, int y, int z) const
    {
        const glm::ivec3 cp = world_to_chunk(x, y, z);
        const VoxelChunk *chunk = get_chunk(cp.x, cp.y, cp.z);
        if (!chunk)
            return BLOCK_AIR;
        const glm::ivec3 lp = world_to_local(x, y, z);
        return chunk->get_block(lp.x, lp.y, lp.z);
    }

    VoxelLight VoxelWorld::get_light(int x, int y, int z) const
    {
        const glm::ivec3 cp = world_to_chunk(x, y, z);
        const VoxelChunk *chunk = get_chunk(cp.x, cp.y, cp.z);
        if (!chunk)
        {
            const auto highest = highest_opaque_by_column_.find(column_key(x, z));
            if (highest == highest_opaque_by_column_.end() || y > highest->second)
                return {15, 0, 0, 0};
            return {};
        }
        const glm::ivec3 lp = world_to_local(x, y, z);
        return chunk->lights_[VoxelChunk::index(lp.x, lp.y, lp.z)];
    }

    bool VoxelWorld::set_block(int x, int y, int z, BlockId id)
    {
        const glm::ivec3 cp = world_to_chunk(x, y, z);
        const glm::ivec3 lp = world_to_local(x, y, z);
        VoxelChunk &chunk = get_or_create_chunk(cp.x, cp.y, cp.z);
        if (!chunk.set_block(lp.x, lp.y, lp.z, id))
            return false;

        lighting_dirty_ = true;
        mark_chunk_dirty(cp.x, cp.y, cp.z);

        // Mark neighbor chunks dirty if modified voxel lies on a chunk boundary
        if (lp.x == 0)
            mark_chunk_dirty(cp.x - 1, cp.y, cp.z);
        else if (lp.x == CHUNK_SIZE - 1)
            mark_chunk_dirty(cp.x + 1, cp.y, cp.z);

        if (lp.y == 0)
            mark_chunk_dirty(cp.x, cp.y - 1, cp.z);
        else if (lp.y == CHUNK_SIZE - 1)
            mark_chunk_dirty(cp.x, cp.y + 1, cp.z);

        if (lp.z == 0)
            mark_chunk_dirty(cp.x, cp.y, cp.z - 1);
        else if (lp.z == CHUNK_SIZE - 1)
            mark_chunk_dirty(cp.x, cp.y, cp.z + 1);

        return true;
    }

    VoxelChunk *VoxelWorld::get_chunk(int cx, int cy, int cz)
    {
        const auto it = chunks_.find(chunk_key(cx, cy, cz));
        return (it != chunks_.end()) ? it->second.get() : nullptr;
    }

    const VoxelChunk *VoxelWorld::get_chunk(int cx, int cy, int cz) const
    {
        const auto it = chunks_.find(chunk_key(cx, cy, cz));
        return (it != chunks_.end()) ? it->second.get() : nullptr;
    }

    VoxelChunk &VoxelWorld::get_or_create_chunk(int cx, int cy, int cz)
    {
        const ChunkKey key = chunk_key(cx, cy, cz);
        auto it = chunks_.find(key);
        if (it == chunks_.end())
        {
            auto chunk = std::make_unique<VoxelChunk>(cx, cy, cz);
            VoxelChunk *ptr = chunk.get();
            chunks_.emplace(key, std::move(chunk));
            return *ptr;
        }
        return *it->second;
    }

    void VoxelWorld::mark_chunk_dirty(int cx, int cy, int cz)
    {
        const ChunkKey key = chunk_key(cx, cy, cz);
        if (chunks_.find(key) != chunks_.end())
        {
            dirty_chunks_.insert(key);
        }
    }

    void VoxelWorld::mark_all_dirty()
    {
        for (const auto &entry : chunks_)
            dirty_chunks_.insert(entry.first);
    }

    void VoxelWorld::update_lighting()
    {
        struct LightNode
        {
            int x;
            int y;
            int z;
        };

        using LightArray = std::array<VoxelLight, CHUNK_VOLUME>;
        std::vector<std::pair<ChunkKey, LightArray>> previous_lights;
        previous_lights.reserve(chunks_.size());
        highest_opaque_by_column_.clear();

        for (auto &entry : chunks_)
        {
            const ChunkKey key = entry.first;
            VoxelChunk &chunk = *entry.second;
            previous_lights.emplace_back(key, chunk.lights_);
            chunk.lights_.fill({});

            const int base_x = chunk.chunk_x_ * CHUNK_SIZE;
            const int base_y = chunk.chunk_y_ * CHUNK_SIZE;
            const int base_z = chunk.chunk_z_ * CHUNK_SIZE;
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                for (int y = 0; y < CHUNK_SIZE; ++y)
                {
                    for (int x = 0; x < CHUNK_SIZE; ++x)
                    {
                        const BlockId id = chunk.blocks_[VoxelChunk::index(x, y, z)];
                        if (block_light_opacity(id) >= 15)
                        {
                            const std::uint64_t column = column_key(base_x + x, base_z + z);
                            const int world_y = base_y + y;
                            auto highest = highest_opaque_by_column_.find(column);
                            if (highest == highest_opaque_by_column_.end() || world_y > highest->second)
                                highest_opaque_by_column_[column] = world_y;
                        }
                    }
                }
            }
        }

        auto light_at = [this](int x, int y, int z) -> VoxelLight *
        {
            const glm::ivec3 cp = world_to_chunk(x, y, z);
            VoxelChunk *chunk = get_chunk(cp.x, cp.y, cp.z);
            if (!chunk)
                return nullptr;
            const glm::ivec3 lp = world_to_local(x, y, z);
            return &chunk->lights_[VoxelChunk::index(lp.x, lp.y, lp.z)];
        };

        std::vector<LightNode> sky_queue;
        std::vector<LightNode> block_queue;

        for (auto &entry : chunks_)
        {
            VoxelChunk &chunk = *entry.second;
            const int base_x = chunk.chunk_x_ * CHUNK_SIZE;
            const int base_y = chunk.chunk_y_ * CHUNK_SIZE;
            const int base_z = chunk.chunk_z_ * CHUNK_SIZE;
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                for (int y = 0; y < CHUNK_SIZE; ++y)
                {
                    for (int x = 0; x < CHUNK_SIZE; ++x)
                    {
                        const int world_x = base_x + x;
                        const int world_y = base_y + y;
                        const int world_z = base_z + z;
                        const int index = VoxelChunk::index(x, y, z);
                        const BlockId id = chunk.blocks_[index];
                        const int opacity = block_light_opacity(id);
                        VoxelLight &light = chunk.lights_[index];

                        auto highest = highest_opaque_by_column_.find(column_key(world_x, world_z));
                        if (opacity < 15 &&
                            (highest == highest_opaque_by_column_.end() || world_y > highest->second))
                        {
                            light.sky = static_cast<std::uint8_t>(
                                id == BLOCK_AIR ? 15 : 15 - opacity);
                        }

                        const BlockDefinition &definition = get_block_definition(id);
                        if (definition.light_emission > 0)
                        {
                            const unsigned int emission = std::min<unsigned int>(definition.light_emission, 15);
                            light.red = static_cast<std::uint8_t>(emission * definition.light_colour.red / 255);
                            light.green = static_cast<std::uint8_t>(emission * definition.light_colour.green / 255);
                            light.blue = static_cast<std::uint8_t>(emission * definition.light_colour.blue / 255);
                        }
                    }
                }
            }
        }

        for (auto &entry : chunks_)
        {
            VoxelChunk &chunk = *entry.second;
            const int base_x = chunk.chunk_x_ * CHUNK_SIZE;
            const int base_y = chunk.chunk_y_ * CHUNK_SIZE;
            const int base_z = chunk.chunk_z_ * CHUNK_SIZE;
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                for (int y = 0; y < CHUNK_SIZE; ++y)
                {
                    for (int x = 0; x < CHUNK_SIZE; ++x)
                    {
                        const VoxelLight &light = chunk.lights_[VoxelChunk::index(x, y, z)];
                        if (light.sky == 0)
                            continue;
                        const int world_x = base_x + x;
                        const int world_y = base_y + y;
                        const int world_z = base_z + z;
                        for (const FaceInfo &face : FACES)
                        {
                            VoxelLight *neighbor = light_at(world_x + face.dx,
                                                            world_y + face.dy,
                                                            world_z + face.dz);
                            if (!neighbor)
                                continue;
                            const BlockId neighbor_id = get_block(world_x + face.dx,
                                                                  world_y + face.dy,
                                                                  world_z + face.dz);
                            if (block_light_opacity(neighbor_id) < 15 &&
                                neighbor->sky < attenuate_light(light.sky, block_light_opacity(neighbor_id)))
                            {
                                sky_queue.push_back({world_x, world_y, world_z});
                                break;
                            }
                        }
                    }
                }
            }
        }

        auto propagate_sky = [&]()
        {
            for (std::size_t cursor = 0; cursor < sky_queue.size(); ++cursor)
            {
                const LightNode node = sky_queue[cursor];
                const VoxelLight *source = light_at(node.x, node.y, node.z);
                if (!source || source->sky == 0)
                    continue;
                const std::uint8_t source_level = source->sky;
                for (const FaceInfo &face : FACES)
                {
                    const int nx = node.x + face.dx;
                    const int ny = node.y + face.dy;
                    const int nz = node.z + face.dz;
                    VoxelLight *target = light_at(nx, ny, nz);
                    if (!target)
                        continue;
                    const int opacity = block_light_opacity(get_block(nx, ny, nz));
                    if (opacity >= 15)
                        continue;
                    const std::uint8_t candidate = attenuate_light(source_level, opacity);
                    if (candidate > target->sky)
                    {
                        target->sky = candidate;
                        sky_queue.push_back({nx, ny, nz});
                    }
                }
            }
        };

        for (auto &entry : chunks_)
        {
            VoxelChunk &chunk = *entry.second;
            const int base_x = chunk.chunk_x_ * CHUNK_SIZE;
            const int base_y = chunk.chunk_y_ * CHUNK_SIZE;
            const int base_z = chunk.chunk_z_ * CHUNK_SIZE;
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                for (int y = 0; y < CHUNK_SIZE; ++y)
                {
                    for (int x = 0; x < CHUNK_SIZE; ++x)
                    {
                        const VoxelLight &light = chunk.lights_[VoxelChunk::index(x, y, z)];
                        if (light.red == 0 && light.green == 0 && light.blue == 0)
                            continue;
                        block_queue.push_back({base_x + x, base_y + y, base_z + z});
                    }
                }
            }
        }

        auto propagate_block = [&]()
        {
            for (std::size_t cursor = 0; cursor < block_queue.size(); ++cursor)
            {
                const LightNode node = block_queue[cursor];
                const VoxelLight *source = light_at(node.x, node.y, node.z);
                if (!source)
                    continue;
                const VoxelLight source_light = *source;
                if (source_light.red == 0 && source_light.green == 0 && source_light.blue == 0)
                    continue;
                for (const FaceInfo &face : FACES)
                {
                    const int nx = node.x + face.dx;
                    const int ny = node.y + face.dy;
                    const int nz = node.z + face.dz;
                    VoxelLight *target = light_at(nx, ny, nz);
                    if (!target)
                        continue;
                    const int opacity = block_light_opacity(get_block(nx, ny, nz));
                    if (opacity >= 15)
                        continue;

                    const VoxelLight candidate{
                        0,
                        attenuate_light(source_light.red, opacity),
                        attenuate_light(source_light.green, opacity),
                        attenuate_light(source_light.blue, opacity)};
                    bool changed = false;
                    if (candidate.red > target->red)
                    {
                        target->red = candidate.red;
                        changed = true;
                    }
                    if (candidate.green > target->green)
                    {
                        target->green = candidate.green;
                        changed = true;
                    }
                    if (candidate.blue > target->blue)
                    {
                        target->blue = candidate.blue;
                        changed = true;
                    }
                    if (changed)
                        block_queue.push_back({nx, ny, nz});
                }
            }
        };

        propagate_sky();
        propagate_block();

        for (const auto &previous : previous_lights)
        {
            const auto chunk_it = chunks_.find(previous.first);
            if (chunk_it == chunks_.end() || !chunk_it->second)
                continue;
            if (previous.second != chunk_it->second->lights_)
                dirty_chunks_.insert(previous.first);
        }
        lighting_dirty_ = false;
    }

    void VoxelWorld::set_sky_brightness(float brightness)
    {
        brightness = std::clamp(brightness, 0.0f, 1.0f);
        if (brightness == sky_brightness_)
            return;
        sky_brightness_ = brightness;
        mark_all_dirty();
    }

    void VoxelWorld::update_dirty_chunks(const VoxelAtlas *atlas)
    {
        if (lighting_dirty_)
            update_lighting();
        if (dirty_chunks_.empty())
            return;

        auto lookup = [this](int wx, int wy, int wz) -> BlockId
        {
            return this->get_block(wx, wy, wz);
        };
        auto light_lookup = [this](int wx, int wy, int wz) -> VoxelLight
        {
            return this->get_light(wx, wy, wz);
        };

        for (const ChunkKey &key : dirty_chunks_)
        {
            auto it = chunks_.find(key);
            if (it != chunks_.end() && it->second)
            {
                it->second->rebuild_mesh(atlas, lookup, light_lookup, sky_brightness_);
            }
        }
        dirty_chunks_.clear();
    }

    VoxelRaycastResult VoxelWorld::raycast(const glm::vec3 &origin, const glm::vec3 &direction, float max_distance) const
    {
        VoxelRaycastResult result;
        const float len_sq = glm::dot(direction, direction);
        if (!std::isfinite(origin.x) || !std::isfinite(origin.y) || !std::isfinite(origin.z) ||
            !std::isfinite(direction.x) || !std::isfinite(direction.y) || !std::isfinite(direction.z) ||
            !std::isfinite(len_sq) || !std::isfinite(max_distance) || len_sq < 1e-6f || max_distance <= 0.0f)
            return result;

        const glm::vec3 dir = glm::normalize(direction);
        const double floor_x = std::floor(static_cast<double>(origin.x));
        const double floor_y = std::floor(static_cast<double>(origin.y));
        const double floor_z = std::floor(static_cast<double>(origin.z));
        if (floor_x < std::numeric_limits<int>::lowest() || floor_x > std::numeric_limits<int>::max() ||
            floor_y < std::numeric_limits<int>::lowest() || floor_y > std::numeric_limits<int>::max() ||
            floor_z < std::numeric_limits<int>::lowest() || floor_z > std::numeric_limits<int>::max())
            return result;
        int x = static_cast<int>(floor_x);
        int y = static_cast<int>(floor_y);
        int z = static_cast<int>(floor_z);

        // If the origin itself is inside a solid block
        const BlockId start_id = get_block(x, y, z);
        if (start_id != BLOCK_AIR && get_block_definition(start_id).is_solid)
        {
            result.hit = true;
            result.hit_block = {x, y, z};
            result.place_block = {x, y, z};
            result.hit_face = VoxelFace::top;
            result.hit_pos = origin;
            result.distance = 0.0f;
            return result;
        }

        const int step_x = (dir.x > 0.0f) ? 1 : ((dir.x < 0.0f) ? -1 : 0);
        const int step_y = (dir.y > 0.0f) ? 1 : ((dir.y < 0.0f) ? -1 : 0);
        const int step_z = (dir.z > 0.0f) ? 1 : ((dir.z < 0.0f) ? -1 : 0);

        const float inf = std::numeric_limits<float>::infinity();
        const float t_delta_x = (step_x != 0) ? std::abs(1.0f / dir.x) : inf;
        const float t_delta_y = (step_y != 0) ? std::abs(1.0f / dir.y) : inf;
        const float t_delta_z = (step_z != 0) ? std::abs(1.0f / dir.z) : inf;

        // Distance to next voxel boundary:
        // When step > 0: distance to ceil boundary (floor(origin) + 1.0f - origin) * t_delta
        // When step < 0: distance to floor boundary (origin - floor(origin)) * t_delta.
        // If origin is exactly on an integer boundary (diff <= 1e-7f):
        //   For step < 0, the ray enters the adjacent negative voxel immediately, so distance is 0.0f.
        auto init_t_max = [](float origin_val, float dir_val, int step, float t_delta) -> float
        {
            if (step > 0)
            {
                return (std::floor(origin_val) + 1.0f - origin_val) * t_delta;
            }
            if (step < 0)
            {
                const float diff = origin_val - std::floor(origin_val);
                return (diff > 1e-7f ? diff : 0.0f) * t_delta;
            }
            return std::numeric_limits<float>::infinity();
        };

        float t_max_x = init_t_max(origin.x, dir.x, step_x, t_delta_x);
        float t_max_y = init_t_max(origin.y, dir.y, step_y, t_delta_y);
        float t_max_z = init_t_max(origin.z, dir.z, step_z, t_delta_z);

        float dist = 0.0f;
        VoxelFace hit_face = VoxelFace::top;

        while (dist <= max_distance)
        {
            if (t_max_x < t_max_y)
            {
                if (t_max_x < t_max_z)
                {
                    if ((step_x > 0 && x == std::numeric_limits<int>::max()) ||
                        (step_x < 0 && x == std::numeric_limits<int>::lowest()))
                        return result;
                    x += step_x;
                    dist = t_max_x;
                    t_max_x += t_delta_x;
                    hit_face = (step_x > 0) ? VoxelFace::west : VoxelFace::east;
                }
                else
                {
                    if ((step_z > 0 && z == std::numeric_limits<int>::max()) ||
                        (step_z < 0 && z == std::numeric_limits<int>::lowest()))
                        return result;
                    z += step_z;
                    dist = t_max_z;
                    t_max_z += t_delta_z;
                    hit_face = (step_z > 0) ? VoxelFace::south : VoxelFace::north;
                }
            }
            else
            {
                if (t_max_y < t_max_z)
                {
                    if ((step_y > 0 && y == std::numeric_limits<int>::max()) ||
                        (step_y < 0 && y == std::numeric_limits<int>::lowest()))
                        return result;
                    y += step_y;
                    dist = t_max_y;
                    t_max_y += t_delta_y;
                    hit_face = (step_y > 0) ? VoxelFace::bottom : VoxelFace::top;
                }
                else
                {
                    if ((step_z > 0 && z == std::numeric_limits<int>::max()) ||
                        (step_z < 0 && z == std::numeric_limits<int>::lowest()))
                        return result;
                    z += step_z;
                    dist = t_max_z;
                    t_max_z += t_delta_z;
                    hit_face = (step_z > 0) ? VoxelFace::south : VoxelFace::north;
                }
            }

            if (dist > max_distance)
                break;

            const BlockId block_id = get_block(x, y, z);
            if (block_id != BLOCK_AIR && get_block_definition(block_id).is_solid)
            {
                result.hit = true;
                result.hit_block = {x, y, z};
                const FaceInfo &face_info = FACES[static_cast<int>(hit_face)];
                result.place_block = {x + face_info.dx, y + face_info.dy, z + face_info.dz};
                result.hit_face = hit_face;
                result.hit_pos = origin + dir * dist;
                result.distance = dist;
                return result;
            }
        }

        return result;
    }

    void VoxelWorld::draw(const Material &material)
    {
        for (const auto &pair : chunks_)
        {
            if (pair.second)
            {
                pair.second->draw(material);
            }
        }
    }

    int VoxelWorld::total_triangles() const
    {
        int total = 0;
        for (const auto &pair : chunks_)
        {
            if (pair.second)
            {
                total += pair.second->triangle_count();
            }
        }
        return total;
    }

} // namespace sl
