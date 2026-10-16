#include "voxel.h"

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
                                  const std::function<BlockId(int, int, int)> &world_block_lookup)
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

                        const float r = (def.tint.red / 255.0f) * face.brightness;
                        const float g = (def.tint.green / 255.0f) * face.brightness;
                        const float b = (def.tint.blue / 255.0f) * face.brightness;
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

    std::uint64_t VoxelWorld::chunk_key(int cx, int cy, int cz)
    {
        const std::uint64_t ux = static_cast<std::uint64_t>(static_cast<std::uint32_t>(cx)) & 0x3FFFFFu;
        const std::uint64_t uy = static_cast<std::uint64_t>(static_cast<std::uint32_t>(cy)) & 0xFFFFFu;
        const std::uint64_t uz = static_cast<std::uint64_t>(static_cast<std::uint32_t>(cz)) & 0x3FFFFFu;
        return (ux << 42) | (uy << 22) | uz;
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

    bool VoxelWorld::set_block(int x, int y, int z, BlockId id)
    {
        const glm::ivec3 cp = world_to_chunk(x, y, z);
        const glm::ivec3 lp = world_to_local(x, y, z);
        VoxelChunk &chunk = get_or_create_chunk(cp.x, cp.y, cp.z);
        if (!chunk.set_block(lp.x, lp.y, lp.z, id))
            return false;

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
        const std::uint64_t key = chunk_key(cx, cy, cz);
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
        const std::uint64_t key = chunk_key(cx, cy, cz);
        if (chunks_.find(key) != chunks_.end())
        {
            dirty_chunks_.insert(key);
        }
    }

    void VoxelWorld::update_dirty_chunks(const VoxelAtlas *atlas)
    {
        if (dirty_chunks_.empty())
            return;

        auto lookup = [this](int wx, int wy, int wz) -> BlockId
        {
            return this->get_block(wx, wy, wz);
        };

        for (std::uint64_t key : dirty_chunks_)
        {
            auto it = chunks_.find(key);
            if (it != chunks_.end() && it->second)
            {
                it->second->rebuild_mesh(atlas, lookup);
            }
        }
        dirty_chunks_.clear();
    }

    VoxelRaycastResult VoxelWorld::raycast(const glm::vec3 &origin, const glm::vec3 &direction, float max_distance) const
    {
        VoxelRaycastResult result;
        const float len_sq = glm::dot(direction, direction);
        if (len_sq < 1e-6f || max_distance <= 0.0f)
            return result;

        const glm::vec3 dir = glm::normalize(direction);

        int x = static_cast<int>(std::floor(origin.x));
        int y = static_cast<int>(std::floor(origin.y));
        int z = static_cast<int>(std::floor(origin.z));

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
                    x += step_x;
                    dist = t_max_x;
                    t_max_x += t_delta_x;
                    hit_face = (step_x > 0) ? VoxelFace::west : VoxelFace::east;
                }
                else
                {
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
                    y += step_y;
                    dist = t_max_y;
                    t_max_y += t_delta_y;
                    hit_face = (step_y > 0) ? VoxelFace::bottom : VoxelFace::top;
                }
                else
                {
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
