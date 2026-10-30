#include "terrain.h"

#include "renderer.h"

#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cmath>
#include <limits>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <utility>

namespace sl
{
    namespace
    {
        constexpr double pi = 3.14159265358979323846;

        struct FaceBasis
        {
            dvec3 n, u, v;
        };

        /** u x v == -n, which is also true of the flat terrain's (x, z), so every face shares one winding. */
        FaceBasis face_basis(int face)
        {
            static const dvec3 normals[6] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
            FaceBasis basis;
            basis.n = normals[std::clamp(face, 0, 5)];
            basis.u = std::fabs(basis.n.y) > 0.5 ? dvec3(1, 0, 0) : dvec3(0, 1, 0);
            basis.v = glm::cross(basis.u, basis.n);
            return basis;
        }

        double smoothstep(double a, double b, double x)
        {
            if (b <= a)
                return x >= b ? 1.0 : 0.0;
            const double t = std::clamp((x - a) / (b - a), 0.0, 1.0);
            return t * t * (3.0 - 2.0 * t);
        }

        /** 1 inside [low, high], fading to 0 over `blend` outside. */
        double band(double x, double low, double high, double blend)
        {
            const double rise = smoothstep(low - blend, low, x);
            const double fall = 1.0 - smoothstep(high, high + blend, x);
            return rise * fall;
        }

        int deepest_level(const TerrainConfig &config)
        {
            if (config.max_level >= 0)
                return std::min(config.max_level, 24);
            const double root = config.shape == TerrainShape::flat ? config.size : config.radius * pi * 0.5;
            const double cells = static_cast<double>(std::max(config.chunk_cells, 2));
            const double finest = std::max(config.finest_cell_size, 1.0e-3);
            const double ratio = root / (cells * finest);
            return ratio <= 1.0 ? 0 : std::clamp(static_cast<int>(std::ceil(std::log2(ratio))), 0, 24);
        }

        /** Geometry shared by chunk building, selection and queries. */
        struct Geometry
        {
            const TerrainConfig &config;
            const TerrainSource &source;
            bool sphere;
            int cells;
            double root_size;
            int max_level;
            double lod_cell = 0.0;

            Geometry(const TerrainConfig &c, const TerrainSource &s)
                : config(c), source(s), sphere(c.shape == TerrainShape::sphere), cells(std::max(c.chunk_cells, 2)),
                  root_size(c.shape == TerrainShape::flat ? c.size : c.radius * pi * 0.5), max_level(deepest_level(c))
            {
            }

            double chunk_size(int level) const { return std::ldexp(root_size, -level); }
            double cell_size(int level) const { return chunk_size(level) / static_cast<double>(cells); }

            /** Position along a face in 0..1 of grid line `i` of the node whose index along that axis is `index`. */
            double unit(int level, int index, double i) const
            {
                return std::ldexp(static_cast<double>(index) * cells + i, -level) / cells;
            }

            /** Flat: the world (x, 0, z); planet: the unit direction from the centre. */
            dvec3 ground(int face, double a, double b) const
            {
                if (!sphere)
                    return {config.origin.x + a * config.size, 0.0, config.origin.z + b * config.size};
                const FaceBasis basis = face_basis(face);
                const double s = -1.0 + 2.0 * a;
                const double t = -1.0 + 2.0 * b;
                const dvec3 cube = basis.n + s * basis.u + t * basis.v;
                // The tangent warp evens out the cell size across a cube face.
                const dvec3 warped(std::tan(cube.x * pi * 0.25), std::tan(cube.y * pi * 0.25), std::tan(cube.z * pi * 0.25));
                return glm::normalize(warped);
            }

            double sample(const dvec3 &g) const
            {
                if (!sphere)
                    return source.height(g.x, g.z);
                return lod_cell > 0.0 ? source.height_on_sphere_lod(g, config.radius, lod_cell) : source.height_on_sphere(g, config.radius);
            }

            dvec3 position(const dvec3 &g, double h) const
            {
                if (!sphere)
                    return {g.x, config.origin.y + h, g.z};
                return config.origin + g * (config.radius + h);
            }

            dvec3 up(const dvec3 &g) const { return sphere ? g : dvec3(0.0, 1.0, 0.0); }

            /** Surface normal from heights sampled eps metres to each side, so it does not depend on the mesh. */
            dvec3 normal(const dvec3 &g, double eps) const
            {
                if (!sphere)
                {
                    const double hx = (source.height(g.x + eps, g.z) - source.height(g.x - eps, g.z)) / (2.0 * eps);
                    const double hz = (source.height(g.x, g.z + eps) - source.height(g.x, g.z - eps)) / (2.0 * eps);
                    return glm::normalize(dvec3(-hx, 1.0, -hz));
                }
                const dvec3 axis = std::fabs(g.x) < 0.57 ? dvec3(1, 0, 0) : (std::fabs(g.y) < 0.57 ? dvec3(0, 1, 0) : dvec3(0, 0, 1));
                const dvec3 e1 = glm::normalize(glm::cross(g, axis));
                const dvec3 e2 = glm::cross(g, e1);
                const double angle = eps / config.radius;
                auto point = [&](const dvec3 &d)
                {
                    const dvec3 direction = glm::normalize(d);
                    return direction * (config.radius + sample(direction));
                };
                const dvec3 d1 = point(g + angle * e1) - point(g - angle * e1);
                const dvec3 d2 = point(g + angle * e2) - point(g - angle * e2);
                return glm::normalize(glm::cross(d1, d2));
            }
        };

        int layer_count(const std::vector<TerrainLayer> &layers)
        {
            return std::min(static_cast<int>(layers.size()), terrain_max_layers);
        }

        void layer_weights(const TerrainConfig &config, const std::vector<TerrainLayer> &layers, const TerrainSample &sample,
                           float weights[terrain_max_layers])
        {
            const int count = layer_count(layers);
            for (int i = 0; i < terrain_max_layers; ++i)
            {
                if (i < count)
                {
                    const TerrainLayer &layer = layers[i];
                    const double w = band(sample.height, layer.min_height, layer.max_height, layer.height_blend) *
                                     band(sample.slope_degrees, layer.min_slope, layer.max_slope, layer.slope_blend) *
                                     layer.strength;
                    weights[i] = static_cast<float>(std::max(w, 0.0));
                }
                else
                    weights[i] = 0.0f;
            }
            if (config.weight_function)
                config.weight_function(sample, weights);
            float sum = 0.0f;
            for (int i = 0; i < terrain_max_layers; ++i)
            {
                weights[i] = i < std::max(count, 1) ? std::max(weights[i], 0.0f) : 0.0f;
                sum += weights[i];
            }
            if (sum <= 1.0e-6f)
            {
                std::fill(weights, weights + terrain_max_layers, 0.0f);
                weights[0] = 1.0f;
                return;
            }
            for (int i = 0; i < terrain_max_layers; ++i)
                weights[i] /= sum;
        }

        /** Metres after which UVs wrap: the largest tile, so every (snapped) tile fits a whole number of times. */
        double uv_period(const TerrainConfig &config, const std::vector<TerrainLayer> &layers)
        {
            if (config.uv_period_metres > 0.0)
                return config.uv_period_metres;
            double period = 1.0e-3;
            for (int i = 0; i < layer_count(layers); ++i)
                period = std::max(period, layers[i].tile_metres);
            return period;
        }

        Vertex3D make_terrain_vertex(const dvec3 &relative, const dvec3 &normal, const float colour[4], const float extra[4], float u, float v)
        {
            Vertex3D vertex;
            vertex.position[0] = static_cast<float>(relative.x);
            vertex.position[1] = static_cast<float>(relative.y);
            vertex.position[2] = static_cast<float>(relative.z);
            vertex.normal[0] = static_cast<float>(normal.x);
            vertex.normal[1] = static_cast<float>(normal.y);
            vertex.normal[2] = static_cast<float>(normal.z);
            for (int i = 0; i < 4; ++i)
            {
                vertex.colour[i] = colour[i];
                vertex.extra[i] = extra[i];
            }
            vertex.uv[0] = u;
            vertex.uv[1] = v;
            // Geomorph offset, filled in after the grid is complete. A non-zero w stops create_mesh generating tangents.
            vertex.tangent[0] = vertex.tangent[1] = vertex.tangent[2] = 0.0f;
            vertex.tangent[3] = 1.0f;
            return vertex;
        }
    } // namespace

    TerrainView make_terrain_view(const Camera &camera, const dvec3 &world_position, int viewport_width, int viewport_height)
    {
        TerrainView view;
        view.position = world_position;
        const glm::vec3 forward = camera.target - camera.position;
        view.forward = glm::length(forward) > 0.0f ? dvec3(glm::normalize(forward)) : dvec3(0.0, 0.0, -1.0);
        view.up = dvec3(camera.up);
        view.fov_y_degrees = camera.field_of_view;
        view.viewport_height = std::max(viewport_height, 1);
        view.aspect = static_cast<double>(std::max(viewport_width, 1)) / std::max(viewport_height, 1);
        view.far_distance = camera.far_plane;
        return view;
    }

    TerrainChunkData build_terrain_chunk(const TerrainConfig &config, const TerrainSource &source, const TerrainNodeKey &key)
    {
        Geometry geo(config, source);
        geo.lod_cell = geo.cell_size(key.level);
        const int cells = geo.cells;
        const int stride = cells + 1;
        std::vector<TerrainLayer> layers = config.layers;
        if (layers.empty())
            layers = default_terrain_layers(source.min_height(), source.max_height());

        TerrainChunkData data;
        data.key = key;
        data.grid_vertices = stride;

        const std::size_t grid_count = static_cast<std::size_t>(stride) * stride;
        std::vector<dvec3> ground(grid_count);
        std::vector<double> height(grid_count);
        std::vector<dvec3> position(grid_count);
        double min_h = std::numeric_limits<double>::max();
        double max_h = std::numeric_limits<double>::lowest();
        for (int j = 0; j < stride; ++j)
            for (int i = 0; i < stride; ++i)
            {
                const std::size_t index = static_cast<std::size_t>(j) * stride + i;
                ground[index] = geo.ground(key.face, geo.unit(key.level, key.x, i), geo.unit(key.level, key.y, j));
                height[index] = geo.sample(ground[index]);
                position[index] = geo.position(ground[index], height[index]);
                min_h = std::min(min_h, height[index]);
                max_h = std::max(max_h, height[index]);
            }
        data.min_height = min_h;
        data.max_height = max_h;
        const dvec3 centre_ground =
            geo.ground(key.face, std::ldexp(key.x + 0.5, -key.level), std::ldexp(key.y + 0.5, -key.level));
        data.center = geo.position(centre_ground, 0.5 * (min_h + max_h));

        const double cell = geo.cell_size(key.level);
        const double depth = std::max(config.skirt_scale, 0.0) * cell;
        const double period = uv_period(config, layers);
        const FaceBasis basis = face_basis(key.face);

        // Texture coordinates are in metres; subtracting a whole number of periods keeps them small and precise.
        auto uv_of = [&](const dvec3 &g, double a, double b, double &u, double &v)
        {
            if (!geo.sphere)
            {
                u = g.x;
                v = g.z;
            }
            else
            {
                const double metres = config.radius * pi * 0.25;
                u = (2.0 * a - 1.0) * metres;
                v = (2.0 * b - 1.0) * metres;
            }
        };
        double u0, v0;
        uv_of(ground[0], geo.unit(key.level, key.x, 0), geo.unit(key.level, key.y, 0), u0, v0);
        u0 = std::floor(u0 / period) * period;
        v0 = std::floor(v0 / period) * period;

        const double normal_eps = std::max(cell, 1.0e-3);
        data.vertices.reserve(grid_count + static_cast<std::size_t>(stride) * 4);
        std::vector<dvec3> normals(grid_count);
        for (int j = 0; j < stride; ++j)
            for (int i = 0; i < stride; ++i)
            {
                const std::size_t index = static_cast<std::size_t>(j) * stride + i;
                const dvec3 normal = geo.normal(ground[index], normal_eps);
                normals[index] = normal;
                TerrainSample sample;
                sample.position = ground[index];
                sample.height = height[index];
                sample.normal = normal;
                sample.slope_degrees = std::acos(std::clamp(glm::dot(normal, geo.up(ground[index])), -1.0, 1.0)) * 180.0 / pi;
                sample.cell_size = cell;
                float weights[terrain_max_layers];
                layer_weights(config, layers, sample, weights);
                float colour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                float extra[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                if (config.weights_in_vertex_colour)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        colour[k] = weights[k];
                        extra[k] = weights[k + 4];
                    }
                }
                else
                {
                    // Alpha carries the blended specular strength for the untextured shading path.
                    colour[0] = colour[1] = colour[2] = colour[3] = 0.0f;
                    for (int k = 0; k < layer_count(layers); ++k)
                    {
                        colour[3] += weights[k] * static_cast<float>(layers[k].specular);
                        colour[0] += weights[k] * layers[k].colour.red / 255.0f;
                        colour[1] += weights[k] * layers[k].colour.green / 255.0f;
                        colour[2] += weights[k] * layers[k].colour.blue / 255.0f;
                    }
                    float rgb[3];
                    if (config.colour_function && config.colour_function(sample, rgb))
                        colour[0] = rgb[0], colour[1] = rgb[1], colour[2] = rgb[2];
                }
                double u, v;
                uv_of(ground[index], geo.unit(key.level, key.x, i), geo.unit(key.level, key.y, j), u, v);
                data.vertices.push_back(make_terrain_vertex(position[index] - data.center, normal, colour, extra,
                                                            static_cast<float>(u - u0), static_cast<float>(v - v0)));
            }

        if (config.geomorph && key.level > 0)
        {
            const int parent_x = key.x >> 1, parent_y = key.y >> 1;
            std::vector<dvec3> parent(grid_count);
            for (int j = 0; j < stride; ++j)
                for (int i = 0; i < stride; ++i)
                {
                    const dvec3 g = geo.ground(key.face, geo.unit(key.level - 1, parent_x, i),
                                               geo.unit(key.level - 1, parent_y, j));
                    parent[static_cast<std::size_t>(j) * stride + i] = geo.position(g, geo.sample(g));
                }
            auto at = [&](int i, int j) { return parent[static_cast<std::size_t>(j) * stride + i]; };
            for (int j = 0; j < stride; ++j)
                for (int i = 0; i < stride; ++i)
                {
                    // A child vertex sits on a parent vertex, on a parent edge midpoint, or at a parent quad centre.
                    const int si = i + cells * (key.x & 1), sj = j + cells * (key.y & 1);
                    const int pi = si / 2, pj = sj / 2;
                    const bool odd_i = (si & 1) != 0, odd_j = (sj & 1) != 0;
                    dvec3 target;
                    if (!odd_i && !odd_j)
                        target = at(pi, pj);
                    else if (odd_i && !odd_j)
                        target = 0.5 * (at(pi, pj) + at(pi + 1, pj));
                    else if (!odd_i && odd_j)
                        target = 0.5 * (at(pi, pj) + at(pi, pj + 1));
                    else
                        target = 0.5 * (at(pi + 1, pj) + at(pi, pj + 1));
                    const std::size_t index = static_cast<std::size_t>(j) * stride + i;
                    const dvec3 delta = target - position[index];
                    data.vertices[index].tangent[0] = static_cast<float>(delta.x);
                    data.vertices[index].tangent[1] = static_cast<float>(delta.y);
                    data.vertices[index].tangent[2] = static_cast<float>(delta.z);
                }
        }

        data.indices.reserve(static_cast<std::size_t>(cells) * cells * 6 + static_cast<std::size_t>(cells) * 4 * 6);
        auto grid = [&](int i, int j) { return static_cast<std::uint32_t>(j * stride + i); };
        for (int j = 0; j < cells; ++j)
            for (int i = 0; i < cells; ++i)
            {
                const std::uint32_t a = grid(i, j), b = grid(i + 1, j), c = grid(i, j + 1), d = grid(i + 1, j + 1);
                data.indices.insert(data.indices.end(), {a, c, b, b, c, d});
            }

        // Skirts: a lowered copy of each border vertex. Each edge runs in the direction that faces the quad outward.
        std::vector<std::uint32_t> border;
        for (int i = 0; i < stride; ++i)
            border.push_back(grid(i, 0));
        for (int j = 1; j < stride; ++j)
            border.push_back(grid(cells, j));
        for (int i = cells - 1; i >= 0; --i)
            border.push_back(grid(i, cells));
        for (int j = cells - 1; j >= 1; --j)
            border.push_back(grid(0, j));
        std::vector<std::uint32_t> skirt(border.size());
        for (std::size_t k = 0; k < border.size(); ++k)
        {
            Vertex3D vertex = data.vertices[border[k]];
            const std::size_t index = border[k];
            const dvec3 drop = geo.up(ground[index]) * depth;
            vertex.position[0] -= static_cast<float>(drop.x);
            vertex.position[1] -= static_cast<float>(drop.y);
            vertex.position[2] -= static_cast<float>(drop.z);
            skirt[k] = static_cast<std::uint32_t>(data.vertices.size());
            data.vertices.push_back(vertex);
        }
        for (std::size_t k = 0; k < border.size(); ++k)
        {
            const std::size_t next = (k + 1) % border.size();
            const std::uint32_t p = border[k], q = border[next], sp = skirt[k], sq = skirt[next];
            data.indices.insert(data.indices.end(), {p, q, sq, p, sq, sp});
        }

        double radius_squared = 0.0;
        for (const Vertex3D &vertex : data.vertices)
        {
            const double x = vertex.position[0], y = vertex.position[1], z = vertex.position[2];
            radius_squared = std::max(radius_squared, x * x + y * y + z * z);
        }
        data.bounding_radius = std::sqrt(radius_squared);
        return data;
    }

    // ---- Terrain ----

    namespace
    {
        struct Frustum
        {
            dvec3 camera{0.0};
            dvec3 forward{0.0, 0.0, -1.0};
            dvec3 planes[4];
            double far_distance = 1.0e30;
            /** Pixels per metre at one metre distance for a one-metre error. */
            double pixel_scale = 1.0;
            bool test(const dvec3 &centre, double radius) const
            {
                const dvec3 offset = centre - camera;
                for (const dvec3 &plane : planes)
                    if (glm::dot(plane, offset) < -radius)
                        return false;
                return glm::dot(forward, offset) - radius <= far_distance;
            }
        };

        struct Pending
        {
            double priority = 0.0;
            unsigned version = 0;
            std::uint64_t last_wanted = 0;
        };

        struct Result
        {
            TerrainNodeKey key;
            unsigned version = 0;
            TerrainChunkData data;
        };
    } // namespace

    namespace
    {
        // Builds the GPU texture arrays behind Terrain::set_layer_textures(): every input is resampled to one square
        // power-of-two size, then box-filtered mip chains are made on the CPU.
        using detail::TextureArrayDesc;
        using detail::texture_array_byte_count;
        using detail::texture_array_level_offset;

        struct TextureEntry
        {
            Bitmap *albedo = nullptr;
            Bitmap *normal = nullptr;
        };

        bool bitmap_pixels(Bitmap *bitmap)
        {
            if (!bitmap || bitmap->width <= 0 || bitmap->height <= 0)
                return false;
            if (bitmap->pixels.empty())
                download_bitmap(bitmap);
            return bitmap->pixels.size() >= static_cast<std::size_t>(bitmap->width) * bitmap->height * 4;
        }

        void resample_to(const Bitmap &source, Uint8 *out, int size)
        {
            const int sw = source.width, sh = source.height;
            const Uint8 *in = source.pixels.data();
            const bool shrink = sw >= size && sh >= size;
            for (int y = 0; y < size; ++y)
                for (int x = 0; x < size; ++x)
                {
                    Uint8 *dst = out + (static_cast<std::size_t>(y) * size + x) * 4;
                    if (shrink)
                    {
                        const int x0 = static_cast<int>(static_cast<long long>(x) * sw / size);
                        const int x1 = std::max(x0 + 1, static_cast<int>((static_cast<long long>(x + 1) * sw + size - 1) / size));
                        const int y0 = static_cast<int>(static_cast<long long>(y) * sh / size);
                        const int y1 = std::max(y0 + 1, static_cast<int>((static_cast<long long>(y + 1) * sh + size - 1) / size));
                        for (int c = 0; c < 4; ++c)
                        {
                            long long sum = 0;
                            for (int sy = y0; sy < y1; ++sy)
                                for (int sx = x0; sx < x1; ++sx)
                                    sum += in[(static_cast<std::size_t>(sy) * sw + sx) * 4 + c];
                            dst[c] = static_cast<Uint8>(sum / ((x1 - x0) * (y1 - y0)));
                        }
                    }
                    else
                    {
                        const float fx = (x + 0.5f) * sw / size - 0.5f;
                        const float fy = (y + 0.5f) * sh / size - 0.5f;
                        const int ix = static_cast<int>(std::floor(fx));
                        const int iy = static_cast<int>(std::floor(fy));
                        const float tx = fx - ix, ty = fy - iy;
                        auto at = [&](int px, int py, int c)
                        {
                            px = ((px % sw) + sw) % sw;
                            py = ((py % sh) + sh) % sh;
                            return static_cast<float>(in[(static_cast<std::size_t>(py) * sw + px) * 4 + c]);
                        };
                        for (int c = 0; c < 4; ++c)
                        {
                            const float top = at(ix, iy, c) * (1.0f - tx) + at(ix + 1, iy, c) * tx;
                            const float bottom = at(ix, iy + 1, c) * (1.0f - tx) + at(ix + 1, iy + 1, c) * tx;
                            dst[c] = static_cast<Uint8>(std::clamp(top * (1.0f - ty) + bottom * ty + 0.5f, 0.0f, 255.0f));
                        }
                    }
                }
        }

        /** One array's pixel data, level-major then layer. */
        struct ArrayPixels
        {
            TextureArrayDesc desc;
            std::vector<Uint8> data;

            Uint8 *layer_at(int level, int layer)
            {
                const std::size_t size = static_cast<std::size_t>(std::max(desc.width >> level, 1)) *
                                         std::max(desc.height >> level, 1) * 4;
                return data.data() + texture_array_level_offset(desc, level) + size * static_cast<std::size_t>(layer);
            }

            void fill(int layer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
            {
                Uint8 *p = layer_at(0, layer);
                for (std::size_t i = 0; i < static_cast<std::size_t>(desc.width) * desc.height; ++i, p += 4)
                {
                    p[0] = r;
                    p[1] = g;
                    p[2] = b;
                    p[3] = a;
                }
            }

            void build_mips()
            {
                for (int level = 1; level < desc.mip_levels; ++level)
                {
                    const int size = std::max(desc.width >> level, 1);
                    const int from = std::max(desc.width >> (level - 1), 1);
                    for (int layer = 0; layer < desc.layers; ++layer)
                    {
                        const Uint8 *src = layer_at(level - 1, layer);
                        Uint8 *dst = layer_at(level, layer);
                        for (int y = 0; y < size; ++y)
                            for (int x = 0; x < size; ++x)
                                for (int c = 0; c < 4; ++c)
                                {
                                    const int x1 = std::min(2 * x + 1, from - 1);
                                    const int y1 = std::min(2 * y + 1, from - 1);
                                    const int sum = src[(static_cast<std::size_t>(2 * y) * from + 2 * x) * 4 + c] +
                                                    src[(static_cast<std::size_t>(2 * y) * from + x1) * 4 + c] +
                                                    src[(static_cast<std::size_t>(y1) * from + 2 * x) * 4 + c] +
                                                    src[(static_cast<std::size_t>(y1) * from + x1) * 4 + c];
                                    dst[(static_cast<std::size_t>(y) * size + x) * 4 + c] = static_cast<Uint8>((sum + 2) / 4);
                                }
                    }
                }
            }
        };

        int snapped_tiles(double period, double tile)
        {
            return std::max(1, static_cast<int>(std::lround(period / std::max(tile, 1.0e-3))));
        }

        struct BuiltTextures
        {
            std::uint32_t albedo = 0;
            std::uint32_t normal = 0;
            TerrainTextures textures;
            double period = 0.0;
        };

        bool build_layer_textures(detail::Renderer &renderer, const std::vector<TerrainLayer> &layers,
                                  const TerrainTextureOptions &options, BuiltTextures &built)
        {
            const int count = layer_count(layers);
            const int capacity = renderer.max_texture_array_layers();
            std::vector<std::vector<TextureEntry>> entries(static_cast<std::size_t>(count));
            bool any_albedo = false, any_normal = false;
            int max_dimension = 0;
            int total = 0;
            for (int i = 0; i < count; ++i)
            {
                const TerrainLayer &layer = layers[static_cast<std::size_t>(i)];
                auto add = [&](Bitmap *albedo, Bitmap *normal)
                {
                    if (!bitmap_pixels(albedo))
                        return;
                    any_albedo = true;
                    if (!bitmap_pixels(normal))
                        normal = nullptr;
                    any_normal = any_normal || normal;
                    max_dimension = std::max({max_dimension, albedo->width, albedo->height});
                    if (normal)
                        max_dimension = std::max({max_dimension, normal->width, normal->height});
                    entries[static_cast<std::size_t>(i)].push_back({albedo, normal});
                };
                add(layer.albedo, layer.normal_map);
                for (const TerrainLayerVariant &variant : layer.variants)
                    add(variant.albedo, variant.normal_map);
                if (entries[static_cast<std::size_t>(i)].empty())
                    entries[static_cast<std::size_t>(i)].push_back({});
                total += static_cast<int>(entries[static_cast<std::size_t>(i)].size());
            }
            if (!any_albedo && !(options.colour_detail && options.macro_texture))
                return false;
            const bool use_macro = options.macro_texture && options.macro_strength > 0.0 && bitmap_pixels(options.macro_texture);
            if (use_macro)
            {
                max_dimension = std::max({max_dimension, options.macro_texture->width, options.macro_texture->height});
                ++total;
            }
            if (total > capacity)
                return false;

            int size = 4;
            const int cap = std::max(options.max_size, 4);
            while (size < max_dimension && size * 2 <= cap)
                size *= 2;
            int mips = 1;
            while ((size >> mips) >= 1)
                ++mips;

            ArrayPixels albedo, normal;
            albedo.desc = normal.desc = {size, size, total, mips};
            albedo.data.assign(texture_array_byte_count(albedo.desc), 0);
            if (any_normal)
                normal.data.assign(texture_array_byte_count(normal.desc), 0);

            double period = 1.0e-3;
            for (int i = 0; i < count; ++i)
                period = std::max(period, layers[static_cast<std::size_t>(i)].tile_metres);
            const double macro_tile = std::max(options.macro_tile_metres, 1.0e-3);
            if (use_macro)
                period = std::max(period, macro_tile);

            TerrainTextures &out = built.textures;
            out = {};
            int next = 0;
            for (int i = 0; i < terrain_max_layers; ++i)
            {
                out.first[i] = 0;
                out.variants[i] = 1;
                out.tile_metres[i] = static_cast<float>(period);
                out.tiles_per_period[i] = 1;
            }
            for (int i = 0; i < count; ++i)
            {
                const TerrainLayer &layer = layers[static_cast<std::size_t>(i)];
                const auto &list = entries[static_cast<std::size_t>(i)];
                out.first[i] = next;
                out.specular[i] = static_cast<float>(std::clamp(layer.specular, 0.0, 1.0));
                out.variants[i] = static_cast<int>(list.size());
                out.tiles_per_period[i] = snapped_tiles(period, layer.tile_metres);
                out.tile_metres[i] = static_cast<float>(period / out.tiles_per_period[i]);
                for (const TextureEntry &entry : list)
                {
                    if (entry.albedo)
                        resample_to(*entry.albedo, albedo.layer_at(0, next), size);
                    else
                        albedo.fill(next, layer.colour.red, layer.colour.green, layer.colour.blue, 255);
                    if (any_normal)
                    {
                        if (entry.normal)
                            resample_to(*entry.normal, normal.layer_at(0, next), size);
                        else
                            normal.fill(next, 128, 128, 255, 255);
                    }
                    ++next;
                }
            }
            if (use_macro)
            {
                resample_to(*options.macro_texture, albedo.layer_at(0, next), size);
                if (any_normal)
                    normal.fill(next, 128, 128, 255, 255);
                out.macro_layer = next;
                out.macro_tile_metres = static_cast<float>(period / snapped_tiles(period, macro_tile));
                // A negative strength tells the shader the vertex colour is the albedo and the macro texture only modulates it.
                out.macro_strength = static_cast<float>(options.colour_detail ? -options.macro_strength : options.macro_strength);
                ++next;
            }
            albedo.build_mips();
            if (any_normal)
                normal.build_mips();

            // Each layer's average colour is its top mip, which the shader fades to with distance.
            for (int i = 0; i < count; ++i)
            {
                float sum[3] = {0.0f, 0.0f, 0.0f};
                for (int k = 0; k < out.variants[i]; ++k)
                {
                    const Uint8 *p = albedo.layer_at(mips - 1, out.first[i] + k);
                    for (int c = 0; c < 3; ++c)
                        sum[c] += p[c] / 255.0f;
                }
                for (int c = 0; c < 3; ++c)
                    out.average[i][c] = sum[c] / static_cast<float>(out.variants[i]);
            }

            if (!renderer.create_texture_array(albedo.desc, albedo.data.data(), built.albedo))
                return false;
            if (any_normal && !renderer.create_texture_array(normal.desc, normal.data.data(), built.normal))
            {
                renderer.destroy_texture_array(built.albedo);
                built.albedo = 0;
                return false;
            }
            out.albedo_array = built.albedo;
            out.normal_array = built.normal;
            built.period = period;
            return true;
        }
    }

    struct Terrain::Impl
    {
        struct Node
        {
            Mesh *mesh = nullptr;
            bool queued = false;
            bool stale = false;
            unsigned version = 0;
            double min_height = 0.0;
            double max_height = 0.0;
            dvec3 center{0.0};
            double radius = 0.0;
            std::uint64_t last_used = 0;
            std::uint64_t retry_after = 0;
            bool split = false;
            int triangles = 0;
        };

        Impl(const TerrainConfig &c, std::shared_ptr<TerrainSource> s) : config(c), source(std::move(s)), geo(config, *source)
        {
            if (config.worker_threads < 0)
            {
                const unsigned hardware = std::thread::hardware_concurrency();
                thread_count = static_cast<int>(std::clamp<unsigned>(hardware > 1 ? hardware - 1 : 1, 1, 4));
            }
            else
                thread_count = std::min(config.worker_threads, 16);
            for (int i = 0; i < thread_count; ++i)
                workers.emplace_back([this] { worker_loop(); });
        }

        ~Impl()
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread &worker : workers)
                worker.join();
            for (auto &entry : nodes)
                if (entry.second.mesh)
                    destroy_mesh(entry.second.mesh);
            release_textures();
        }

        TerrainConfig config;
        std::shared_ptr<TerrainSource> source;
        Geometry geo;

        std::unordered_map<TerrainNodeKey, Node> nodes;
        std::vector<TerrainNodeKey> visible;
        std::uint64_t frame = 0;
        TerrainStats stats;

        detail::Renderer *texture_owner = nullptr;
        TerrainTextures textures;
        std::atomic<bool> textured{false};
        std::atomic<bool> colour_detail{false};
        std::atomic<double> uv_period{0.0};

        void release_textures()
        {
            if (texture_owner && texture_owner == detail::active_renderer())
            {
                if (textures.albedo_array)
                    texture_owner->destroy_texture_array(textures.albedo_array);
                if (textures.normal_array)
                    texture_owner->destroy_texture_array(textures.normal_array);
            }
            texture_owner = nullptr;
            textures = {};
        }
        double fade_start = 0.0;
        double fade_end = 0.0;
        dvec3 view_position{0.0};
        double pixel_scale = 1.0;
        /** Turn of the body-fixed frame about +Y, in radians. */
        std::atomic<double> rotation{0.0};
        std::atomic<double> axis_x{0.0}, axis_y{1.0}, axis_z{0.0};

        dvec3 axis() const { return {axis_x.load(), axis_y.load(), axis_z.load()}; }
        glm::dquat turn_quat() const { return glm::angleAxis(rotation.load(), axis()); }
        dvec3 to_body(const dvec3 &world_vector) const { return glm::inverse(turn_quat()) * world_vector; }
        dvec3 to_world(const dvec3 &body_vector) const { return turn_quat() * body_vector; }
        bool turning() const { return geo.sphere && rotation.load() != 0.0; }
        /** The view expressed in the body-fixed frame the chunks are built in. */
        TerrainView body_view(const TerrainView &view) const
        {
            if (!turning())
                return view;
            TerrainView v = view;
            v.position = config.origin + to_body(view.position - config.origin);
            v.forward = to_body(view.forward);
            v.up = to_body(view.up);
            return v;
        }

        int thread_count = 0;
        std::vector<std::thread> workers;
        std::mutex mutex;
        std::condition_variable condition;
        std::unordered_map<TerrainNodeKey, Pending> pending;
        std::vector<Result> done;
        bool stop = false;
        int in_flight = 0;

        TerrainChunkData build_chunk(const TerrainNodeKey &key) const
        {
            TerrainConfig local = config;
            local.weights_in_vertex_colour = config.weights_in_vertex_colour || (textured.load() && !colour_detail.load());
            if (textured.load())
                local.uv_period_metres = uv_period.load();
            return build_terrain_chunk(local, *source, key);
        }

        void worker_loop()
        {
            for (;;)
            {
                TerrainNodeKey key;
                unsigned version = 0;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this] { return stop || (!pending.empty() && done.size() < 16); });
                    if (stop)
                        return;
                    auto best = pending.begin();
                    for (auto it = pending.begin(); it != pending.end(); ++it)
                        if (it->second.priority < best->second.priority)
                            best = it;
                    key = best->first;
                    version = best->second.version;
                    pending.erase(best);
                    ++in_flight;
                }
                Result result;
                result.key = key;
                result.version = version;
                result.data = build_chunk(key);
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    done.push_back(std::move(result));
                    --in_flight;
                }
            }
        }

        void request(const TerrainNodeKey &key, double priority, Node &node)
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (node.queued)
            {
                auto it = pending.find(key);
                if (it != pending.end())
                {
                    it->second.priority = priority;
                    it->second.last_wanted = frame;
                }
                return;
            }
            if (frame < node.retry_after)
                return;
            node.queued = true;
            pending[key] = {priority, node.version, frame};
            condition.notify_one();
        }

        // Bounds of a node that has no mesh yet, from the height range of its nearest built ancestor.
        void estimate_bounds(const TerrainNodeKey &key, double hmin, double hmax, dvec3 &centre, double &radius) const
        {
            if (!geo.sphere)
            {
                const double size = geo.chunk_size(key.level);
                const double x0 = config.origin.x + key.x * size;
                const double z0 = config.origin.z + key.y * size;
                centre = {x0 + size * 0.5, config.origin.y + 0.5 * (hmin + hmax), z0 + size * 0.5};
                radius = 0.5 * std::sqrt(2.0 * size * size + (hmax - hmin) * (hmax - hmin));
                return;
            }
            dvec3 sum(0.0);
            dvec3 points[50];
            int count = 0;
            for (int j = 0; j <= 4; ++j)
                for (int i = 0; i <= 4; ++i)
                {
                    const dvec3 g = geo.ground(key.face, std::ldexp(key.x + i * 0.25, -key.level),
                                               std::ldexp(key.y + j * 0.25, -key.level));
                    points[count++] = geo.position(g, hmin);
                    points[count++] = geo.position(g, hmax);
                }
            for (int k = 0; k < count; ++k)
                sum += points[k];
            centre = sum / static_cast<double>(count);
            double r2 = 0.0;
            for (int k = 0; k < count; ++k)
                r2 = std::max(r2, glm::dot(points[k] - centre, points[k] - centre));
            radius = std::sqrt(r2);
        }

        bool occluded(const Frustum &frustum, const dvec3 &centre, double radius) const
        {
            if (!geo.sphere)
                return false;
            const dvec3 e = frustum.camera - config.origin;
            const double d = glm::length(e);
            const double occluder = config.radius + source->min_height();
            if (d <= occluder)
                return false;
            const dvec3 c = centre - config.origin;
            const double cl = glm::length(c);
            if (cl <= radius)
                return false;
            const double angle = std::acos(std::clamp(glm::dot(e, c) / (d * cl), -1.0, 1.0));
            const double angular_radius = std::asin(std::min(1.0, radius / cl));
            const double camera_horizon = std::acos(std::min(1.0, occluder / d));
            const double node_horizon = std::acos(std::min(1.0, occluder / (cl + radius)));
            return angle - angular_radius > camera_horizon + node_horizon;
        }

        void visit(const TerrainNodeKey &key, double hmin, double hmax, const Frustum &frustum, bool interactive,
                   std::vector<TerrainNodeKey> *ideal)
        {
            Node *node = nullptr;
            if (interactive)
                node = &nodes[key];
            else
            {
                const auto it = nodes.find(key);
                if (it != nodes.end())
                    node = &it->second;
            }
            const bool built = node && node->mesh;
            dvec3 centre;
            double radius;
            if (built)
            {
                centre = node->center;
                radius = node->radius;
            }
            else
                estimate_bounds(key, hmin, hmax, centre, radius);
            if (!frustum.test(centre, radius) || occluded(frustum, centre, radius))
                return;
            if (interactive)
                node->last_used = frame;

            const double distance = std::max(glm::length(centre - frustum.camera) - radius, 0.1);
            const double cell = geo.cell_size(key.level);
            const double error = cell * config.error_scale * frustum.pixel_scale / distance;
            const bool was_split = interactive && node->split;
            const bool wants_split = key.level < geo.max_level && error > config.max_screen_error * (was_split ? 0.8 : 1.0);
            const double child_min = built ? node->min_height - cell : hmin;
            const double child_max = built ? node->max_height + cell : hmax;

            auto emit = [&]()
            {
                if (!interactive)
                {
                    ideal->push_back(key);
                    return;
                }
                node->split = false;
                if (!built || node->stale)
                    request(key, distance, *node);
                visible.push_back(key);
            };
            if (!wants_split)
            {
                emit();
                return;
            }
            TerrainNodeKey children[4];
            for (int k = 0; k < 4; ++k)
                children[k] = {key.face, key.level + 1, key.x * 2 + (k & 1), key.y * 2 + (k >> 1)};
            if (!interactive)
            {
                for (const TerrainNodeKey &child : children)
                    visit(child, child_min, child_max, frustum, false, ideal);
                return;
            }
            bool all_ready = true;
            for (const TerrainNodeKey &child : children)
            {
                Node &child_node = nodes[child];
                if (!child_node.mesh)
                {
                    all_ready = false;
                    request(child, distance, child_node);
                }
            }
            if (!all_ready)
            {
                emit();
                return;
            }
            node->split = true;
            for (const TerrainNodeKey &child : children)
                visit(child, child_min, child_max, frustum, true, nullptr);
        }

        Frustum make_frustum(const TerrainView &view) const
        {
            Frustum frustum;
            frustum.camera = view.position;
            dvec3 forward = glm::length(view.forward) > 0.0 ? glm::normalize(view.forward) : dvec3(0.0, 0.0, -1.0);
            dvec3 up = view.up - forward * glm::dot(view.up, forward);
            up = glm::length(up) > 1.0e-9 ? glm::normalize(up) : dvec3(0.0, 1.0, 0.0);
            const dvec3 right = glm::cross(forward, up);
            const double tan_y = std::tan(glm::radians(std::clamp(view.fov_y_degrees, 1.0, 170.0)) * 0.5);
            const double tan_x = tan_y * std::max(view.aspect, 0.01);
            frustum.forward = forward;
            frustum.planes[0] = glm::normalize(right + forward * tan_x);
            frustum.planes[1] = glm::normalize(-right + forward * tan_x);
            frustum.planes[2] = glm::normalize(up + forward * tan_y);
            frustum.planes[3] = glm::normalize(-up + forward * tan_y);
            frustum.far_distance = view.far_distance;
            frustum.pixel_scale = std::max(view.viewport_height, 1.0) / (2.0 * tan_y);
            return frustum;
        }

        std::vector<TerrainNodeKey> roots() const
        {
            std::vector<TerrainNodeKey> keys;
            const int count = geo.sphere ? 6 : 1;
            for (int face = 0; face < count; ++face)
                keys.push_back({face, 0, 0, 0});
            return keys;
        }

        void invalidate_node(Node &node)
        {
            node.stale = true;
            ++node.version;
        }

        void invalidate_all()
        {
            for (auto &entry : nodes)
                invalidate_node(entry.second);
        }

        void invalidate_region(double x0, double z0, double x1, double z1)
        {
            if (geo.sphere)
            {
                invalidate_all();
                return;
            }
            for (auto &entry : nodes)
            {
                const TerrainNodeKey &key = entry.first;
                const double size = geo.chunk_size(key.level);
                const double margin = geo.cell_size(key.level) * 2.0;
                const double nx0 = config.origin.x + key.x * size - margin;
                const double nz0 = config.origin.z + key.y * size - margin;
                if (nx0 <= x1 && nx0 + size + 2.0 * margin >= x0 && nz0 <= z1 && nz0 + size + 2.0 * margin >= z0)
                    invalidate_node(entry.second);
            }
        }

        bool upload(Result &result)
        {
            auto it = nodes.find(result.key);
            if (it == nodes.end())
                return false;
            Node &node = it->second;
            if (node.version != result.version)
            {
                node.queued = false;
                return false;
            }
            Mesh *mesh = create_mesh(result.data.vertices, result.data.indices);
            node.queued = false;
            if (!mesh)
            {
                node.retry_after = frame + 120;
                return false;
            }
            if (node.mesh)
                destroy_mesh(node.mesh);
            node.mesh = mesh;
            node.stale = false;
            node.center = result.data.center;
            node.radius = result.data.bounding_radius;
            node.min_height = result.data.min_height;
            node.max_height = result.data.max_height;
            node.triangles = static_cast<int>(result.data.indices.size() / 3);
            return true;
        }

        void process_uploads()
        {
            int budget = std::max(config.max_uploads_per_frame, 1);
            if (thread_count == 0)
            {
                while (budget > 0)
                {
                    TerrainNodeKey key;
                    Pending job;
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        if (pending.empty())
                            break;
                        auto best = pending.begin();
                        for (auto it = pending.begin(); it != pending.end(); ++it)
                            if (it->second.priority < best->second.priority)
                                best = it;
                        key = best->first;
                        job = best->second;
                        pending.erase(best);
                    }
                    Result result;
                    result.key = key;
                    result.version = job.version;
                    result.data = build_chunk(key);
                    if (upload(result))
                        --budget;
                }
                return;
            }
            std::vector<Result> ready;
            {
                std::lock_guard<std::mutex> lock(mutex);
                const std::size_t take = std::min<std::size_t>(done.size(), 64);
                ready.assign(std::make_move_iterator(done.begin()), std::make_move_iterator(done.begin() + take));
                done.erase(done.begin(), done.begin() + take);
            }
            // Anything beyond this frame's budget goes back to the front of the queue.
            std::vector<Result> deferred;
            for (Result &result : ready)
            {
                if (budget <= 0)
                {
                    deferred.push_back(std::move(result));
                    continue;
                }
                if (upload(result))
                    --budget;
            }
            if (!deferred.empty())
            {
                std::lock_guard<std::mutex> lock(mutex);
                done.insert(done.begin(), std::make_move_iterator(deferred.begin()), std::make_move_iterator(deferred.end()));
            }
            condition.notify_all();
        }

        void cancel_unwanted()
        {
            std::lock_guard<std::mutex> lock(mutex);
            for (auto it = pending.begin(); it != pending.end();)
            {
                if (it->second.last_wanted + 3 < frame)
                {
                    auto node = nodes.find(it->first);
                    if (node != nodes.end())
                        node->second.queued = false;
                    it = pending.erase(it);
                }
                else
                    ++it;
            }
        }

        void evict()
        {
            if (static_cast<int>(nodes.size()) <= config.max_cached_chunks)
                return;
            std::vector<std::pair<std::uint64_t, TerrainNodeKey>> candidates;
            // A parent only splits when all four children are built, including any outside the view. Keep the
            // children of a recently used node, or evicting them would make the parent fall back to its coarse mesh.
            const auto idle = [&](const Node &node) { return node.last_used + config.evict_after_frames < frame; };
            for (const auto &entry : nodes)
            {
                const TerrainNodeKey &key = entry.first;
                if (key.level == 0 || !idle(entry.second))
                    continue;
                const auto parent = nodes.find({key.face, key.level - 1, key.x >> 1, key.y >> 1});
                if (parent != nodes.end() && !idle(parent->second))
                    continue;
                candidates.emplace_back(entry.second.last_used, key);
            }
            std::sort(candidates.begin(), candidates.end(),
                      [](const auto &a, const auto &b) { return a.first < b.first; });
            const std::size_t target = static_cast<std::size_t>(config.max_cached_chunks * 0.9);
            for (const auto &candidate : candidates)
            {
                if (nodes.size() <= target)
                    break;
                auto it = nodes.find(candidate.second);
                if (it->second.mesh)
                    destroy_mesh(it->second.mesh);
                nodes.erase(it);
            }
        }

        void update(const TerrainView &view)
        {
            ++frame;
            double x0, z0, x1, z1;
            if (source->take_dirty_region(x0, z0, x1, z1))
                invalidate_region(x0, z0, x1, z1);
            const Frustum frustum = make_frustum(view);
            view_position = view.position;
            pixel_scale = frustum.pixel_scale;
            visible.clear();
            const double hmin = source->min_height();
            const double hmax = source->max_height();
            for (const TerrainNodeKey &root : roots())
                visit(root, hmin, hmax, frustum, true, nullptr);
            cancel_unwanted();
            process_uploads();
            evict();

            stats = {};
            for (const TerrainNodeKey &key : visible)
            {
                const auto it = nodes.find(key);
                if (it == nodes.end() || !it->second.mesh)
                    continue;
                ++stats.visible_chunks;
                stats.visible_triangles += it->second.triangles;
                stats.deepest_level = std::max(stats.deepest_level, key.level);
            }
            for (const auto &entry : nodes)
                if (entry.second.mesh)
                    ++stats.resident_chunks;
            std::lock_guard<std::mutex> lock(mutex);
            stats.pending_chunks = static_cast<int>(pending.size()) + in_flight + static_cast<int>(done.size());
        }
    };

    Terrain::Terrain(const TerrainConfig &config, std::shared_ptr<TerrainSource> source)
        : impl_(std::make_unique<Impl>(config, std::move(source)))
    {
    }

    Terrain::~Terrain() = default;

    const TerrainConfig &Terrain::config() const { return impl_->config; }
    const std::shared_ptr<TerrainSource> &Terrain::source() const { return impl_->source; }
    int Terrain::max_level() const { return impl_->geo.max_level; }

    void Terrain::set_rotation(double angle, const dvec3 &axis) const
    {
        const double length = glm::length(axis);
        const dvec3 unit = length > 0.0 ? axis / length : dvec3(0.0, 1.0, 0.0);
        impl_->axis_x.store(unit.x);
        impl_->axis_y.store(unit.y);
        impl_->axis_z.store(unit.z);
        impl_->rotation.store(angle);
    }
    double Terrain::rotation() const { return impl_->rotation.load(); }
    dvec3 Terrain::rotation_axis() const { return impl_->axis(); }

    void Terrain::update(const TerrainView &view) { impl_->update(impl_->body_view(view)); }

    void Terrain::prefetch(const TerrainView &view)
    {
        Impl &impl = *impl_;
        const Frustum frustum = impl.make_frustum(impl.body_view(view));
        std::vector<TerrainNodeKey> drawn;
        drawn.swap(impl.visible);
        for (const TerrainNodeKey &root : impl.roots())
            impl.visit(root, impl.source->min_height(), impl.source->max_height(), frustum, true, nullptr);
        impl.visible.swap(drawn);
    }

    bool Terrain::set_layer_textures(const TerrainTextureOptions &options)
    {
        return set_layer_textures(impl_->config.layers, options);
    }

    bool Terrain::set_layer_textures(const std::vector<TerrainLayer> &layers, const TerrainTextureOptions &options)
    {
        Impl &impl = *impl_;
        impl.release_textures();
        bool ok = false;
        detail::Renderer *renderer = detail::active_renderer();
        if (renderer && renderer->max_texture_array_layers() > 0)
        {
            BuiltTextures built;
            if (build_layer_textures(*renderer, layers, options, built))
            {
                impl.texture_owner = renderer;
                impl.textures = built.textures;
                impl.uv_period = built.period;
                impl.colour_detail = options.colour_detail;
                impl.fade_start = options.fade_start;
                impl.fade_end = options.fade_end;
                ok = true;
            }
        }
        if (impl.textured.exchange(ok) != ok || ok)
            impl.invalidate_all();
        return ok;
    }

    void Terrain::clear_layer_textures()
    {
        Impl &impl = *impl_;
        impl.release_textures();
        if (impl.textured.exchange(false))
            impl.invalidate_all();
    }

    bool Terrain::draw(const dvec3 &origin, const Material &material) const
    {
        const Impl &impl = *impl_;
        Material shaded = material;
        if (impl.textured.load())
        {
            if (impl.texture_owner != detail::active_renderer())
                return false;
            shaded.terrain.textures = &impl.textures;
        }
        shaded.terrain.enabled = true;
        shaded.terrain.fade_start = static_cast<float>(impl.fade_start);
        shaded.terrain.fade_end = static_cast<float>(impl.fade_end);
        bool ok = true;
        for (const TerrainNodeKey &key : impl.visible)
        {
            const auto it = impl.nodes.find(key);
            if (it == impl.nodes.end() || !it->second.mesh)
                continue;
            const dvec3 centre = it->second.center;
            const dvec3 eye_body = impl.view_position - centre;
            const bool turning = impl.turning();
            const double angle = turning ? impl.rotation.load() : 0.0;
            const dvec3 placed = turning ? impl.config.origin + impl.to_world(centre - impl.config.origin) : centre;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(placed - origin));
            // Textured terrain is turned by its shaders (the eye is then given in world axes); a plain mesh just takes the turn in its model.
            const bool shader_turn = turning && impl.textured.load();
            if (turning && !shader_turn)
            {
                model = glm::rotate(model, static_cast<float>(angle), glm::vec3(impl.axis()));
                shaded.terrain.enabled = false;
            }
            const dvec3 eye = shader_turn ? impl.to_world(eye_body) : eye_body;
            const glm::dquat q = impl.turn_quat();
            shaded.terrain.rotation[0] = shader_turn ? static_cast<float>(q.x) : 0.0f;
            shaded.terrain.rotation[1] = shader_turn ? static_cast<float>(q.y) : 0.0f;
            shaded.terrain.rotation[2] = shader_turn ? static_cast<float>(q.z) : 0.0f;
            shaded.terrain.rotation[3] = shader_turn ? static_cast<float>(q.w) : 1.0f;
            shaded.terrain.eye[0] = static_cast<float>(eye.x);
            shaded.terrain.eye[1] = static_cast<float>(eye.y);
            shaded.terrain.eye[2] = static_cast<float>(eye.z);
            if (impl.config.geomorph && key.level > 0)
            {
                // A chunk becomes its parent's shape by the distance at which the parent would have been split.
                const double split = 2.0 * impl.geo.cell_size(key.level) * impl.config.error_scale * impl.pixel_scale /
                                     std::max(impl.config.max_screen_error, 1.0e-6);
                shaded.terrain.morph_start = static_cast<float>(0.7 * split);
                shaded.terrain.morph_end = static_cast<float>(split);
            }
            else
            {
                shaded.terrain.morph_start = 1.0e30f;
                shaded.terrain.morph_end = 2.0e30f;
            }
            ok = draw_mesh(it->second.mesh, model, shaded) && ok;
        }
        return ok;
    }

    void Terrain::wait_until_ready(const TerrainView &view, int max_updates)
    {
        for (int i = 0; i < max_updates; ++i)
        {
            impl_->update(impl_->body_view(view));
            bool complete = true;
            for (const TerrainNodeKey &key : select_nodes(view))
            {
                const auto it = impl_->nodes.find(key);
                if (it == impl_->nodes.end() || !it->second.mesh || it->second.stale)
                {
                    complete = false;
                    break;
                }
            }
            if (complete)
                return;
            if (impl_->thread_count > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    std::vector<TerrainNodeKey> Terrain::select_nodes(const TerrainView &view) const
    {
        std::vector<TerrainNodeKey> keys;
        const Frustum frustum = impl_->make_frustum(impl_->body_view(view));
        const double hmin = impl_->source->min_height();
        const double hmax = impl_->source->max_height();
        for (const TerrainNodeKey &root : impl_->roots())
            impl_->visit(root, hmin, hmax, frustum, false, &keys);
        return keys;
    }

    TerrainStats Terrain::stats() const { return impl_->stats; }

    double Terrain::height_at(double x, double z) const
    {
        return impl_->config.origin.y + impl_->source->height(x, z);
    }

    dvec3 Terrain::normal_at(double x, double z) const
    {
        return impl_->geo.normal({x, 0.0, z}, std::max(impl_->config.finest_cell_size * 0.5, 1.0e-3));
    }

    double Terrain::height_at_direction(const dvec3 &direction) const
    {
        const double length = glm::length(direction);
        if (length <= 0.0)
            return 0.0;
        return impl_->source->height_on_sphere(impl_->to_body(direction / length), impl_->config.radius);
    }

    dvec3 Terrain::normal_at_direction(const dvec3 &direction) const
    {
        const double length = glm::length(direction);
        if (length <= 0.0)
            return {0.0, 1.0, 0.0};
        return impl_->to_world(impl_->geo.normal(impl_->to_body(direction / length), std::max(impl_->config.finest_cell_size * 0.5, 1.0e-3)));
    }

    dvec3 Terrain::surface_point(const dvec3 &direction) const
    {
        const double length = glm::length(direction);
        if (length <= 0.0)
            return impl_->config.origin;
        const dvec3 unit = impl_->to_body(direction / length);
        const dvec3 point = impl_->geo.position(unit, impl_->source->height_on_sphere(unit, impl_->config.radius));
        return impl_->turning() ? impl_->config.origin + impl_->to_world(point - impl_->config.origin) : point;
    }

    double Terrain::altitude_of(const dvec3 &position) const
    {
        if (!impl_->geo.sphere)
            return position.y - height_at(position.x, position.z);
        const dvec3 offset = position - impl_->config.origin;
        const double distance = glm::length(offset);
        if (distance <= 0.0)
            return -impl_->config.radius;
        return distance - impl_->config.radius - height_at_direction(offset);
    }

    dvec3 Terrain::normal_below(const dvec3 &position) const
    {
        if (!impl_->geo.sphere)
            return normal_at(position.x, position.z);
        return normal_at_direction(position - impl_->config.origin);
    }

    TerrainHit Terrain::raycast(const dvec3 &origin, const dvec3 &direction, double max_distance) const
    {
        TerrainHit hit;
        const double length = glm::length(direction);
        if (length <= 0.0 || !(max_distance > 0.0))
            return hit;
        const dvec3 dir = direction / length;
        const double minimum_step = std::max(impl_->config.finest_cell_size * 0.5, 1.0e-3);
        auto altitude = [&](double t) { return altitude_of(origin + dir * t); };

        double t = 0.0;
        double previous_t = 0.0;
        double f = altitude(0.0);
        if (f <= 0.0)
        {
            hit.hit = true;
            hit.position = origin;
            hit.normal = normal_below(origin);
            hit.distance = 0.0;
            return hit;
        }
        for (int iteration = 0; iteration < 200000 && t < max_distance; ++iteration)
        {
            // Stepping a fraction of the height above the surface cannot jump over terrain gentler than about 60 degrees.
            const double step = std::max(minimum_step, 0.3 * std::fabs(f));
            previous_t = t;
            t = std::min(t + step, max_distance);
            f = altitude(t);
            if (f <= 0.0)
            {
                double low = previous_t;
                double high = t;
                for (int k = 0; k < 40; ++k)
                {
                    const double mid = 0.5 * (low + high);
                    if (altitude(mid) <= 0.0)
                        high = mid;
                    else
                        low = mid;
                }
                hit.hit = true;
                hit.distance = high;
                hit.position = origin + dir * high;
                hit.normal = normal_below(hit.position);
                return hit;
            }
            if (t >= max_distance)
                break;
        }
        return hit;
    }

    void Terrain::invalidate_region(double x0, double z0, double x1, double z1)
    {
        impl_->invalidate_region(std::min(x0, x1), std::min(z0, z1), std::max(x0, x1), std::max(z0, z1));
    }

    void Terrain::invalidate_all() { impl_->invalidate_all(); }

} // namespace sl
