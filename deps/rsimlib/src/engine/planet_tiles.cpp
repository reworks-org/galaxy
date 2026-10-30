#include "planet_tiles.h"

#include "error.h"

#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace sl
{
    namespace
    {
        namespace fs = std::filesystem;

        constexpr int colour_layer = 0;
        constexpr int specular_layer = 1;
        constexpr int normal_layer = 2;
        constexpr int layer_count = 3;

        std::uint64_t tile_key(int level, int column, int row)
        {
            return (static_cast<std::uint64_t>(level) << 56) | (static_cast<std::uint64_t>(column) << 28) | static_cast<std::uint64_t>(row);
        }

        std::string lower(std::string text)
        {
            for (char &c : text)
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            return text;
        }

        /** The tiles one map has on disk: which `levelN/tx_<column>_<row>` files exist. */
        struct TileSet
        {
            std::string root;
            std::string extension;
            int base_split = 0;
            int min_level = 0;
            int max_level = -1;
            std::vector<std::unordered_set<std::uint64_t>> present;

            bool valid() const { return max_level >= min_level; }
            bool has(int level, int column, int row) const
            {
                return level >= min_level && level <= max_level && present[static_cast<std::size_t>(level)].count(tile_key(level, column, row)) > 0;
            }
            std::string path(int level, int column, int row) const
            {
                char name[64];
                std::snprintf(name, sizeof(name), "tx_%d_%d.", column, row);
                return (fs::path(root) / ("level" + std::to_string(level)) / (std::string(name) + extension)).string();
            }
        };

        bool parse_ctx(const std::string &path, std::string &directory, int &split, std::string &type)
        {
            std::ifstream in(path);
            if (!in)
                return false;
            std::stringstream buffer;
            buffer << in.rdbuf();
            const std::string text = buffer.str();
            auto word_after = [&](const std::string &key) -> std::string
            {
                const std::size_t at = text.find(key);
                if (at == std::string::npos)
                    return {};
                std::size_t i = at + key.size();
                while (i < text.size() && std::isspace(static_cast<unsigned char>(text[i])))
                    ++i;
                if (i < text.size() && text[i] == '"')
                {
                    const std::size_t end = text.find('"', i + 1);
                    return end == std::string::npos ? std::string() : text.substr(i + 1, end - i - 1);
                }
                std::size_t end = i;
                while (end < text.size() && !std::isspace(static_cast<unsigned char>(text[end])) && text[end] != '}')
                    ++end;
                return text.substr(i, end - i);
            };
            directory = word_after("ImageDirectory");
            const std::string base = word_after("BaseSplit");
            split = base.empty() ? 0 : std::max(0, std::atoi(base.c_str()));
            type = lower(word_after("TileType"));
            return true;
        }

        bool open_tile_set(const std::string &source, TileSet &set, std::string &why)
        {
            std::string folder = source, type;
            if (lower(fs::path(source).extension().string()) == ".ctx")
            {
                std::string directory;
                if (!parse_ctx(source, directory, set.base_split, type))
                {
                    why = "cannot read " + source;
                    return false;
                }
                const fs::path beside = fs::path(source).parent_path();
                folder = (beside / directory).string();
                // Some packs name a folder that was renamed; the level folders then sit beside the descriptor.
                if (directory.empty() || !fs::is_directory(folder))
                    folder = beside.string();
            }
            std::error_code ec;
            if (!fs::is_directory(folder, ec))
            {
                why = "no tile folder at " + folder;
                return false;
            }
            set.root = folder;
            for (const fs::directory_entry &level_dir : fs::directory_iterator(folder, ec))
            {
                const std::string name = level_dir.path().filename().string();
                if (!level_dir.is_directory() || name.rfind("level", 0) != 0 || name.size() < 6)
                    continue;
                const int level = std::atoi(name.c_str() + 5);
                if (level < 0 || level > 40)
                    continue;
                if (static_cast<int>(set.present.size()) <= level)
                    set.present.resize(static_cast<std::size_t>(level) + 1);
                for (const fs::directory_entry &file : fs::directory_iterator(level_dir.path(), ec))
                {
                    const std::string file_name = file.path().filename().string();
                    int column = 0, row = 0;
                    if (std::sscanf(file_name.c_str(), "tx_%d_%d.", &column, &row) != 2 || column < 0 || row < 0)
                        continue;
                    const std::string extension = lower(file.path().extension().string());
                    if (extension.size() < 2)
                        continue;
                    if (set.extension.empty())
                        set.extension = file.path().extension().string().substr(1);
                    else if (lower(set.extension) != extension.substr(1))
                        continue;
                    set.present[static_cast<std::size_t>(level)].insert(tile_key(level, column, row));
                    set.max_level = std::max(set.max_level, level);
                }
            }
            // The coarsest level that has tiles.
            set.min_level = 0;
            while (set.min_level <= set.max_level && set.present[static_cast<std::size_t>(set.min_level)].empty())
                ++set.min_level;
            if (!type.empty() && set.extension.empty())
                set.extension = type;
            if (!set.valid())
            {
                why = "no tiles found under " + folder;
                return false;
            }
            return true;
        }

        struct Frustum
        {
            glm::vec4 planes[4];
            bool enabled = false;
        };

        Frustum make_frustum(const glm::mat4 &m)
        {
            auto row = [&](int i) { return glm::vec4(m[0][i], m[1][i], m[2][i], m[3][i]); };
            Frustum f;
            f.planes[0] = row(3) + row(0);
            f.planes[1] = row(3) - row(0);
            f.planes[2] = row(3) + row(1);
            f.planes[3] = row(3) - row(1);
            f.enabled = true;
            return f;
        }

        // The position on the unit sphere for tile-space coordinates, using create_sphere_mesh()'s axes.
        glm::dvec3 sphere_point(double u, double v)
        {
            const double theta = 2.0 * glm::pi<double>() * u, phi = glm::pi<double>() * v;
            return {std::sin(phi) * std::cos(theta), std::cos(phi), -std::sin(phi) * std::sin(theta)};
        }
    } // namespace

    void PlanetTiles::ancestor_uv(int column, int row, int levels_up, float rect[4])
    {
        const int span = 1 << std::max(levels_up, 0);
        const float size = 1.0f / static_cast<float>(span);
        const float u0 = static_cast<float>(column & (span - 1)) * size, v0 = static_cast<float>(row & (span - 1)) * size;
        rect[0] = u0;
        rect[1] = v0;
        rect[2] = u0 + size;
        rect[3] = v0 + size;
    }

    namespace
    {
        double now_seconds()
        {
            return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
        }
    } // namespace

    struct PlanetTiles::Impl
    {
        struct Tile
        {
            Bitmap *bitmap = nullptr;
            AsyncBitmap load;
            bool loading = false;
            bool failed = false;
            std::uint64_t used = 0;
            double arrived = -1.0e9; // seconds on now_seconds()'s clock when the bitmap became resident
        };

        struct MeshEntry
        {
            Mesh *mesh = nullptr;
            std::uint64_t used = 0;
        };

        struct Leaf
        {
            int level, column, row;
            int up;      // levels up to the colour tile that is drawn
            int spec_up; // levels up to the specular tile, or -1
            Bitmap *colour = nullptr, *normal = nullptr, *specular = nullptr;
            Bitmap *under = nullptr; // the parent tile drawn beneath a leaf that is fading in or feathered at its edges
            float fade = 1.0f;       // opacity while the tile fades in over its parent
            int edges = 0;           // bits 0-3: sides of the patch (i = 0, i = n, j = 0, j = n) with no neighbour at this level
        };

        PlanetTilesOptions options;
        TileSet sets[layer_count];
        std::unordered_map<std::uint64_t, Tile> tiles[layer_count];
        std::unordered_map<std::uint64_t, MeshEntry> meshes;
        std::vector<Leaf> leaves;
        std::uint64_t frame = 1;
        int tile_px = 512;
        int max_level = 0;
        int in_flight = 0;

        // Per-update camera state.
        glm::dvec3 eye{0.0};
        double eye_distance = 0.0;
        double pixel_per_unit = 1.0; // pixels per world unit at distance 1
        Frustum frustum;

        ~Impl()
        {
            for (auto &entry : meshes)
                destroy_mesh(entry.second.mesh);
            for (auto &map : tiles)
                for (auto &entry : map)
                    if (entry.second.bitmap)
                        destroy_bitmap(entry.second.bitmap);
        }

        AsyncBitmapOptions load_options(int layer) const
        {
            AsyncBitmapOptions o;
            o.smooth = true;
            o.mipmaps = true;
            o.anisotropy = options.anisotropy;
            if (layer == specular_layer)
            {
                o.invert = true;
                o.invert_floor = static_cast<int>(std::clamp(options.specular_min_roughness, 0.04f, 1.0f) * 255.0f);
            }
            return o;
        }

        bool usable(int layer, int level, int column, int row) const
        {
            if (!sets[layer].has(level, column, row))
                return false;
            const auto found = tiles[layer].find(tile_key(level, column, row));
            return found == tiles[layer].end() || !found->second.failed;
        }

        Tile *resident(int layer, int level, int column, int row)
        {
            const auto found = tiles[layer].find(tile_key(level, column, row));
            return found != tiles[layer].end() && found->second.bitmap ? &found->second : nullptr;
        }

        void request(int layer, int level, int column, int row, bool wait)
        {
            if (!usable(layer, level, column, row))
                return;
            Tile &tile = tiles[layer][tile_key(level, column, row)];
            if (tile.bitmap || tile.loading || tile.failed)
                return;
            if (!wait && in_flight >= options.max_loads_in_flight)
                return;
            tile.load = load_bitmap_async(sets[layer].path(level, column, row), load_options(layer));
            tile.loading = true;
            ++in_flight;
            if (wait)
                poll(tile, true);
        }

        void poll(Tile &tile, bool wait = false)
        {
            if (!tile.loading)
                return;
            if (wait)
                tile.load.wait();
            if (!tile.load.done())
                return;
            tile.loading = false;
            --in_flight;
            tile.bitmap = tile.load.ready() ? tile.load.get() : nullptr;
            tile.failed = tile.bitmap == nullptr;
            tile.arrived = wait ? -1.0e9 : now_seconds();
            tile.load = AsyncBitmap();
        }

        void poll_all()
        {
            for (auto &map : tiles)
                for (auto &entry : map)
                    poll(entry.second);
        }

        // Bounding sphere (centre direction and radius) of a tile's patch of unit sphere.
        void bounds(int level, int column, int row, glm::dvec3 &centre, double &radius) const
        {
            const double cols = columns(level, sets[colour_layer].base_split), rws = rows(level, sets[colour_layer].base_split);
            glm::dvec3 points[9];
            for (int j = 0; j < 3; ++j)
                for (int i = 0; i < 3; ++i)
                {
                    points[j * 3 + i] = sphere_point((column + 0.5 * i) / cols, (row + 0.5 * j) / rws);
                }
            centre = points[4];
            radius = 0.0;
            for (const glm::dvec3 &p : points)
                radius = std::max(radius, glm::length(p - centre));
        }

        bool visible(const glm::dvec3 &centre, double radius) const
        {
            if (eye_distance > 1.0)
            {
                // Beyond the horizon: the angle between the eye and the tile exceeds the horizon angle plus the tile's size.
                const double angle = std::acos(std::clamp(glm::dot(centre, eye / eye_distance), -1.0, 1.0));
                if (angle > std::acos(1.0 / eye_distance) + 2.0 * std::asin(std::min(radius * 0.5, 1.0)))
                    return false;
            }
            if (frustum.enabled)
                for (const glm::vec4 &plane : frustum.planes)
                {
                    const double distance = plane.x * centre.x + plane.y * centre.y + plane.z * centre.z + plane.w;
                    if (distance < -radius * glm::length(glm::dvec3(plane.x, plane.y, plane.z)))
                        return false;
                }
            return true;
        }

        bool wants_detail(int level, const glm::dvec3 &centre, double radius) const
        {
            const double distance = std::max(glm::length(eye - centre) - radius, 1e-9);
            const double texel = (glm::pi<double>() / rows(level, sets[colour_layer].base_split)) / tile_px;
            return texel * pixel_per_unit / distance > options.texel_pixels;
        }

        void emit(int level, int column, int row, int up)
        {
            Leaf leaf{level, column, row, up, -1};
            const int cl = level - up, cc = column >> up, cr = row >> up;
            Tile *colour = resident(colour_layer, cl, cc, cr);
            if (!colour)
                return;
            colour->used = frame;
            leaf.colour = colour->bitmap;
            if (Tile *normal = resident(normal_layer, cl, cc, cr))
            {
                normal->used = frame;
                leaf.normal = normal->bitmap;
            }
            if (sets[specular_layer].valid())
                for (int l = level; l >= sets[specular_layer].min_level; --l)
                {
                    const int shift = level - l;
                    if (Tile *spec = resident(specular_layer, l, column >> shift, row >> shift))
                    {
                        spec->used = frame;
                        leaf.specular = spec->bitmap;
                        leaf.spec_up = shift;
                        break;
                    }
                }
            if (up == 0 && level > sets[colour_layer].min_level)
            {
                Tile *parent = resident(colour_layer, level - 1, column >> 1, row >> 1);
                if (parent)
                {
                    const double age = now_seconds() - colour->arrived;
                    if (options.fade_seconds > 0.0f && age < options.fade_seconds)
                        leaf.fade = static_cast<float>(std::max(age, 0.0) / options.fade_seconds);
                    if (options.feather > 0.0f)
                    {
                        const int cols = columns(level, sets[colour_layer].base_split), rws = rows(level, sets[colour_layer].base_split);
                        auto missing = [&](int c, int r)
                        {
                            if (r < 0 || r >= rws)
                                return false;
                            return !sets[colour_layer].has(level, (c + cols) % cols, r);
                        };
                        leaf.edges = (missing(column - 1, row) ? 1 : 0) | (missing(column + 1, row) ? 2 : 0) | (missing(column, row - 1) ? 4 : 0) |
                                     (missing(column, row + 1) ? 8 : 0);
                    }
                    if (leaf.fade < 1.0f || leaf.edges)
                    {
                        parent->used = frame;
                        leaf.under = parent->bitmap;
                    }
                }
            }
            leaves.push_back(leaf);
        }

        void visit(int level, int column, int row, bool exists)
        {
            glm::dvec3 centre;
            double radius;
            bounds(level, column, row, centre, radius);
            if (!visible(centre, radius))
                return;
            Tile *tile = exists ? resident(colour_layer, level, column, row) : nullptr;
            if (!tile)
            {
                if (exists)
                {
                    request(colour_layer, level, column, row, false);
                    for (int layer = 1; layer < layer_count; ++layer)
                        request(layer, level, column, row, false);
                }
                emit(level, column, row, 1);
                return;
            }
            tile->used = frame;
            for (int layer = 1; layer < layer_count; ++layer)
                request(layer, level, column, row, false);
            bool refine = level < max_level && wants_detail(level, centre, radius);
            if (refine)
            {
                refine = false;
                for (int k = 0; k < 4 && !refine; ++k)
                    refine = usable(colour_layer, level + 1, 2 * column + (k & 1), 2 * row + (k >> 1));
            }
            if (!refine)
            {
                emit(level, column, row, 0);
                return;
            }
            for (int k = 0; k < 4; ++k)
            {
                const int cc = 2 * column + (k & 1), cr = 2 * row + (k >> 1);
                visit(level + 1, cc, cr, usable(colour_layer, level + 1, cc, cr));
            }
        }

        Mesh *patch_mesh(int level, int column, int row, int up, int layer, int edges = 0)
        {
            // Layers: 0 colour, 1 specular glint, 2 the lowered underlay, 3 the edge-feathered colour patch.
            const std::uint64_t key = tile_key(level, column, row) ^ (static_cast<std::uint64_t>(up) << 52) ^ (static_cast<std::uint64_t>(layer) << 50) ^
                                      (static_cast<std::uint64_t>(edges) << 46);
            MeshEntry &entry = meshes[key];
            entry.used = frame;
            if (entry.mesh)
                return entry.mesh;

            const int n = std::max(options.segments, 2);
            const int base_split = sets[colour_layer].base_split;
            const double cols = columns(level, base_split), rws = rows(level, base_split);
            float rect[4];
            ancestor_uv(column, row, up, rect);
            const float inset = 0.5f / static_cast<float>(std::max(tile_px, 1));
            if (rect[0] == 0.0f)
                rect[0] = inset;
            if (rect[1] == 0.0f)
                rect[1] = inset;
            if (rect[2] == 1.0f)
                rect[2] = 1.0f - inset;
            if (rect[3] == 1.0f)
                rect[3] = 1.0f - inset;

            const double skirt_depth = options.skirt * glm::pi<double>() / rws;
            const int stride = n + 1;
            std::vector<Vertex3D> vertices;
            std::vector<std::uint32_t> indices;
            auto add_vertex = [&](int i, int j, double scale)
            {
                const double fu = static_cast<double>(i) / n, fv = static_cast<double>(j) / n;
                const glm::dvec3 p = sphere_point((column + fu) / cols, (row + fv) / rws);
                Vertex3D v;
                for (int a = 0; a < 3; ++a)
                {
                    v.position[a] = static_cast<float>(p[a] * scale);
                    v.normal[a] = static_cast<float>(p[a]);
                }
                v.uv[0] = rect[0] + (rect[2] - rect[0]) * static_cast<float>(fu);
                v.uv[1] = rect[1] + (rect[3] - rect[1]) * static_cast<float>(fv);
                if (edges && options.feather > 0.0f)
                {
                    // Opaque inside, fading to clear along each side that has no neighbour at this level.
                    double opacity = 1.0;
                    const double w = options.feather;
                    if (edges & 1)
                        opacity = std::min(opacity, fu / w);
                    if (edges & 2)
                        opacity = std::min(opacity, (1.0 - fu) / w);
                    if (edges & 4)
                        opacity = std::min(opacity, fv / w);
                    if (edges & 8)
                        opacity = std::min(opacity, (1.0 - fv) / w);
                    opacity = std::clamp(opacity, 0.0, 1.0);
                    v.colour[3] = static_cast<float>(opacity * opacity * (3.0 - 2.0 * opacity));
                }
                vertices.push_back(v);
            };
            // The underlay sits just beneath the patch drawn over it, well within the skirt, so the two never fight for depth.
            const double surface = layer == 2 ? 1.0 - 0.2 * skirt_depth : 1.0;
            for (int j = 0; j <= n; ++j)
                for (int i = 0; i <= n; ++i)
                    add_vertex(i, j, surface);
            for (int j = 0; j < n; ++j)
                for (int i = 0; i < n; ++i)
                {
                    const std::uint32_t a = static_cast<std::uint32_t>(j * stride + i), b = a + stride;
                    for (const std::uint32_t index : {a, b, a + 1, a + 1, b, b + 1})
                        indices.push_back(index);
                }
            // Skirts: a ring of vertices pulled towards the centre, joined to the border both ways round so they show from either side.
            const double depth = 1.0 - skirt_depth;
            const std::uint32_t skirt_base = static_cast<std::uint32_t>(vertices.size());
            std::vector<std::uint32_t> border;
            std::vector<std::pair<int, int>> ring;
            for (int i = 0; i < n; ++i)
                ring.emplace_back(i, 0);
            for (int j = 0; j < n; ++j)
                ring.emplace_back(n, j);
            for (int i = n; i > 0; --i)
                ring.emplace_back(i, n);
            for (int j = n; j > 0; --j)
                ring.emplace_back(0, j);
            for (const auto &[i, j] : ring)
            {
                border.push_back(static_cast<std::uint32_t>(j * stride + i));
                add_vertex(i, j, depth);
            }
            const std::uint32_t count = static_cast<std::uint32_t>(ring.size());
            for (std::uint32_t k = 0; k < count; ++k)
            {
                const std::uint32_t k2 = (k + 1) % count;
                const std::uint32_t a = border[k], b = border[k2], c = skirt_base + k, d = skirt_base + k2;
                for (const std::uint32_t index : {a, b, c, b, d, c, a, c, b, b, c, d})
                    indices.push_back(index);
            }
            entry.mesh = create_mesh(vertices, indices);
            return entry.mesh;
        }

        void evict()
        {
            // Meshes and textures unused for a few frames may be released, oldest first, once over their budgets.
            const std::uint64_t safe = frame > 3 ? frame - 3 : 0;
            if (meshes.size() > 1200)
            {
                std::vector<std::pair<std::uint64_t, std::uint64_t>> old;
                for (const auto &entry : meshes)
                    if (entry.second.used < safe)
                        old.emplace_back(entry.second.used, entry.first);
                std::sort(old.begin(), old.end());
                for (const auto &item : old)
                {
                    if (meshes.size() <= 900)
                        break;
                    destroy_mesh(meshes[item.second].mesh);
                    meshes.erase(item.second);
                }
            }
            std::size_t loaded = 0;
            for (const auto &map : tiles)
                for (const auto &entry : map)
                    loaded += entry.second.bitmap ? 1 : 0;
            if (loaded <= static_cast<std::size_t>(std::max(options.max_cached_tiles, 8)))
                return;
            struct Candidate
            {
                std::uint64_t used;
                int layer;
                std::uint64_t key;
            };
            std::vector<Candidate> old;
            for (int layer = 0; layer < layer_count; ++layer)
                for (const auto &entry : tiles[layer])
                {
                    const int level = static_cast<int>(entry.first >> 56);
                    if (entry.second.bitmap && entry.second.used < safe && !(layer == colour_layer && level == sets[colour_layer].min_level))
                        old.push_back({entry.second.used, layer, entry.first});
                }
            std::sort(old.begin(), old.end(), [](const Candidate &a, const Candidate &b) { return a.used < b.used; });
            for (const Candidate &c : old)
            {
                if (loaded <= static_cast<std::size_t>(options.max_cached_tiles))
                    break;
                Tile &tile = tiles[c.layer][c.key];
                destroy_bitmap(tile.bitmap);
                tiles[c.layer].erase(c.key);
                --loaded;
            }
        }
    };

    PlanetTiles::PlanetTiles() : impl_(new Impl) {}
    PlanetTiles::~PlanetTiles() = default;

    std::unique_ptr<PlanetTiles> PlanetTiles::create(const PlanetTilesOptions &options)
    {
        std::unique_ptr<PlanetTiles> planet(new PlanetTiles);
        Impl &m = *planet->impl_;
        m.options = options;
        std::string why;
        if (options.colour.empty() || !open_tile_set(options.colour, m.sets[colour_layer], why))
        {
            detail::set_error("PlanetTiles: " + (why.empty() ? std::string("no colour map given") : why));
            return nullptr;
        }
        const std::string optional[] = {"", options.specular, options.normal};
        for (int layer = 1; layer < layer_count; ++layer)
            if (!optional[layer].empty() && !open_tile_set(optional[layer], m.sets[layer], why))
            {
                detail::set_error("PlanetTiles: " + why);
                return nullptr;
            }
        TileSet &colour = m.sets[colour_layer];
        m.max_level = options.max_level < 0 ? colour.max_level : std::min(options.max_level, colour.max_level);
        m.max_level = std::max(m.max_level, colour.min_level);

        // The coarsest level is loaded now and kept, so every patch always has something to draw.
        const int cols = columns(colour.min_level, colour.base_split), rws = rows(colour.min_level, colour.base_split);
        for (int r = 0; r < rws; ++r)
            for (int c = 0; c < cols; ++c)
            {
                if (!colour.has(colour.min_level, c, r))
                {
                    detail::set_error("PlanetTiles: the coarsest level is missing tile " + std::to_string(c) + "," + std::to_string(r));
                    return nullptr;
                }
                m.request(colour_layer, colour.min_level, c, r, true);
                Impl::Tile *tile = m.resident(colour_layer, colour.min_level, c, r);
                if (!tile)
                {
                    detail::set_error("PlanetTiles: cannot load " + colour.path(colour.min_level, c, r));
                    return nullptr;
                }
                if (c == 0 && r == 0)
                    m.tile_px = options.tile_size > 0 ? options.tile_size : std::max(tile->bitmap->width, 1);
            }
        return planet;
    }

    void PlanetTiles::update(const glm::dvec3 &eye, double fov_y, int viewport_height, const glm::mat4 *view_projection)
    {
        Impl &m = *impl_;
        ++m.frame;
        m.poll_all();
        m.leaves.clear();
        m.eye = eye;
        m.eye_distance = glm::length(eye);
        m.pixel_per_unit = static_cast<double>(std::max(viewport_height, 1)) / (2.0 * std::tan(std::clamp(fov_y, 0.01, 3.0) * 0.5));
        m.frustum = view_projection ? make_frustum(*view_projection) : Frustum{};
        const TileSet &colour = m.sets[colour_layer];
        const int cols = columns(colour.min_level, colour.base_split), rws = rows(colour.min_level, colour.base_split);
        for (int r = 0; r < rws; ++r)
            for (int c = 0; c < cols; ++c)
                m.visit(colour.min_level, c, r, true);
        m.evict();
    }

    void PlanetTiles::draw(const glm::mat4 &model, const Material &material)
    {
        Impl &m = *impl_;
        // Opaque patches first; those fading in or feathered at their edges put their parent tile down here and are blended over it after.
        for (const Impl::Leaf &leaf : m.leaves)
        {
            Material patch = material;
            if (leaf.under)
            {
                patch.texture = leaf.under;
                if (Mesh *mesh = m.patch_mesh(leaf.level, leaf.column, leaf.row, 1, 2))
                    draw_mesh(mesh, model, patch);
                continue;
            }
            patch.texture = leaf.colour;
            patch.normal_map = leaf.normal;
            if (Mesh *mesh = m.patch_mesh(leaf.level, leaf.column, leaf.row, leaf.up, 0))
                draw_mesh(mesh, model, patch);
        }
        for (const Impl::Leaf &leaf : m.leaves)
        {
            if (!leaf.under)
                continue;
            Material patch = material;
            patch.texture = leaf.colour;
            patch.normal_map = leaf.normal;
            patch.blend = MaterialBlend::alpha;
            patch.tint.alpha = static_cast<std::uint8_t>(static_cast<float>(material.tint.alpha) * leaf.fade);
            if (Mesh *mesh = leaf.edges ? m.patch_mesh(leaf.level, leaf.column, leaf.row, 0, 3, leaf.edges) : m.patch_mesh(leaf.level, leaf.column, leaf.row, 0, 0))
                draw_mesh(mesh, model, patch);
        }
        if (!m.sets[specular_layer].valid() || m.options.specular_strength <= 0.0f)
            return;
        // The glint is lifted a hair off the surface because backends draw with a strict depth test.
        const glm::mat4 lifted = model * glm::scale(glm::mat4(1.0f), glm::vec3(1.0003f));
        for (const Impl::Leaf &leaf : m.leaves)
        {
            if (!leaf.specular)
                continue;
            Material glint;
            glint.lit = true;
            glint.pbr = true;
            glint.fog = false;
            glint.cull = material.cull;
            glint.blend = MaterialBlend::additive;
            glint.tint = {0, 0, 0, static_cast<std::uint8_t>(std::clamp(m.options.specular_strength, 0.0f, 1.0f) * 255.0f)};
            glint.metallic = 0.0f;
            glint.roughness = 1.0f;
            glint.occlusion_strength = 0.0f;
            glint.metallic_roughness = leaf.specular;
            if (Mesh *mesh = m.patch_mesh(leaf.level, leaf.column, leaf.row, leaf.spec_up, 1))
                draw_mesh(mesh, lifted, glint);
        }
    }

    int PlanetTiles::deepest_level() const { return impl_->sets[colour_layer].max_level; }
    int PlanetTiles::tile_size() const { return impl_->tile_px; }
    bool PlanetTiles::has_specular() const { return impl_->sets[specular_layer].valid(); }
    void PlanetTiles::set_specular_strength(float strength) { impl_->options.specular_strength = strength; }

    PlanetTilesStats PlanetTiles::stats() const
    {
        const Impl &m = *impl_;
        PlanetTilesStats s;
        s.patches = static_cast<int>(m.leaves.size());
        for (const Impl::Leaf &leaf : m.leaves)
            s.deepest_level = std::max(s.deepest_level, leaf.level - leaf.up);
        for (const auto &map : m.tiles)
            for (const auto &entry : map)
            {
                s.tiles_loaded += entry.second.bitmap ? 1 : 0;
                s.tiles_loading += entry.second.loading ? 1 : 0;
            }
        s.meshes = static_cast<int>(m.meshes.size());
        return s;
    }

} // namespace sl
