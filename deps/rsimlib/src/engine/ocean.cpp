#include "ocean.h"

#include "terrain.h"

#include <algorithm>
#include <cmath>

namespace sl
{
    namespace
    {
        constexpr double pi = 3.14159265358979323846;
        constexpr double gravity = 9.80665;
        constexpr double no_shore = 1.0e9;

        double smoothstep(double a, double b, double t)
        {
            const double x = std::clamp((t - a) / (b - a), 0.0, 1.0);
            return x * x * (3.0 - 2.0 * x);
        }

        struct Wave
        {
            double wavelength, steepness, dir_x, dir_z, phase, k, omega;
        };

        std::vector<OceanWave> default_waves()
        {
            const double lengths[6] = {140.0, 78.0, 44.0, 24.0, 13.0, 7.0};
            const double steepness[6] = {0.11, 0.10, 0.09, 0.08, 0.07, 0.06};
            const double spread[6] = {0.0, 0.35, -0.5, 0.75, -0.9, 1.3};
            std::vector<OceanWave> waves;
            for (int i = 0; i < 6; ++i)
                waves.push_back({lengths[i], steepness[i], 0.4 + spread[i], 1.7 * i});
            return waves;
        }
    } // namespace

    struct Ocean::Impl
    {
        OceanConfig config;
        std::vector<Wave> waves;
        double state = 1.0;
        const Terrain *terrain = nullptr;
        Mesh *mesh = nullptr;
        std::vector<Vertex3D> vertices;

        double amplitude(const Wave &w) const { return state * w.steepness / w.k; }

        double depth_at(double x, double z, double range_limit) const
        {
            if (!terrain || range_limit < 0.0)
                return no_shore;
            return config.sea_level - terrain->height_at(x, z);
        }

        /** The displaced point for a rest position. `lod` and the shoaling by `depth` scale each wave. */
        Ocean::Surface surface(double px, double pz, double time, const double *lod, double depth) const
        {
            Ocean::Surface s;
            s.position = {px, config.sea_level, pz};
            s.depth = depth;
            double nx = 0.0, nz = 0.0, pinch = 0.0;
            const auto count = static_cast<double>(waves.size());
            for (std::size_t i = 0; i < waves.size(); ++i)
            {
                const Wave &w = waves[i];
                // Waves flatten as the water shoals, the longest first.
                const double shoal = depth >= no_shore ? 1.0 : smoothstep(0.0, 0.2 * w.wavelength, depth);
                const double f = (lod ? lod[i] : 1.0) * shoal;
                if (f <= 0.0)
                    continue;
                const double a = amplitude(w) * f;
                const double q = std::min(1.0, 0.8 / (count * std::max(w.k * a, 0.1)));
                const double theta = w.k * (w.dir_x * px + w.dir_z * pz) - w.omega * time + w.phase;
                const double c = std::cos(theta), sn = std::sin(theta);
                s.position.x += q * a * w.dir_x * c;
                s.position.z += q * a * w.dir_z * c;
                s.position.y += a * sn;
                nx -= w.dir_x * w.k * a * c;
                nz -= w.dir_z * w.k * a * c;
                pinch += q * w.k * a * sn;
            }
            s.normal = glm::normalize(glm::vec3(static_cast<float>(nx), static_cast<float>(1.0 - pinch), static_cast<float>(nz)));
            s.foam = static_cast<float>(smoothstep(0.35, 0.6, pinch));
            return s;
        }

        double shore_foam(double depth, double time, double width) const
        {
            if (depth >= width)
                return 0.0;
            const double surge = 0.5 + 0.5 * std::sin(time * 0.9 + depth * 0.9);
            return (1.0 - smoothstep(0.0, width, depth)) * (0.55 + 0.45 * surge);
        }
    };

    Ocean::Ocean(const OceanConfig &config) : impl_(new Impl)
    {
        Impl &m = *impl_;
        m.config = config;
        m.config.rings = std::max(m.config.rings, 4);
        m.config.segments = std::max(m.config.segments, 8);
        m.config.inner_radius = std::max(m.config.inner_radius, 0.01);
        m.config.outer_radius = std::max(m.config.outer_radius, 2.0 * m.config.inner_radius);
        const std::vector<OceanWave> source = config.waves.empty() ? default_waves() : config.waves;
        for (const OceanWave &wave : source)
        {
            Wave w;
            w.wavelength = std::max(wave.wavelength, 0.1);
            w.steepness = wave.steepness;
            w.dir_x = std::cos(wave.direction);
            w.dir_z = std::sin(wave.direction);
            w.phase = wave.phase;
            w.k = 2.0 * pi / w.wavelength;
            w.omega = std::sqrt(gravity * w.k);
            m.waves.push_back(w);
        }

        const int rings = m.config.rings, segments = m.config.segments;
        m.vertices.resize(static_cast<std::size_t>(rings + 1) * static_cast<std::size_t>(segments));
        std::vector<std::uint32_t> indices;
        for (int r = 0; r < rings; ++r)
        {
            for (int s = 0; s < segments; ++s)
            {
                const auto a = static_cast<std::uint32_t>(r * segments + s), b = static_cast<std::uint32_t>(r * segments + (s + 1) % segments);
                const auto c = static_cast<std::uint32_t>((r + 1) * segments + s), d = static_cast<std::uint32_t>((r + 1) * segments + (s + 1) % segments);
                indices.insert(indices.end(), {a, c, b, b, c, d});
            }
        }
        for (Vertex3D &v : m.vertices)
        {
            v.tangent[0] = 1.0f;
            v.tangent[3] = 1.0f;
        }
        m.mesh = create_mesh(m.vertices, indices);
    }

    Ocean::~Ocean()
    {
        if (impl_->mesh)
            destroy_mesh(impl_->mesh);
        delete impl_;
    }

    bool Ocean::valid() const { return impl_->mesh != nullptr; }
    void Ocean::set_sea_state(double state) { impl_->state = std::max(state, 0.0); }
    double Ocean::sea_state() const { return impl_->state; }
    void Ocean::set_terrain(const Terrain *terrain) { impl_->terrain = terrain; }
    void Ocean::set_sea_level(double y) { impl_->config.sea_level = y; }
    double Ocean::sea_level() const { return impl_->config.sea_level; }

    Ocean::Surface Ocean::surface_at(double x, double z, double time) const
    {
        const Impl &m = *impl_;
        double px = x, pz = z;
        Surface s;
        for (int i = 0; i < 3; ++i)
        {
            s = m.surface(px, pz, time, nullptr, m.depth_at(px, pz, 0.0));
            px += x - s.position.x;
            pz += z - s.position.z;
        }
        s = m.surface(px, pz, time, nullptr, m.depth_at(px, pz, 0.0));
        if (s.depth < no_shore)
            s.foam = std::max(s.foam, static_cast<float>(m.shore_foam(s.depth, time, m.config.foam_depth)));
        return s;
    }

    double Ocean::height_at(double x, double z, double time) const { return surface_at(x, z, time).position.y; }

    double Ocean::swell_height() const
    {
        double total = 0.0;
        for (std::size_t i = 0; i < std::min<std::size_t>(2, impl_->waves.size()); ++i)
            total += 2.0 * impl_->amplitude(impl_->waves[i]);
        return total;
    }

    void Ocean::update(double cx, double cz, const dvec3 &eye, double time)
    {
        Impl &m = *impl_;
        if (!m.mesh)
            return;
        const OceanConfig &c = m.config;
        const double ratio = c.outer_radius / c.inner_radius;
        const bool transparent = m.terrain && c.clear_depth > 0.0;
        std::vector<double> lod(m.waves.size());
        for (int r = 0; r <= c.rings; ++r)
        {
            const double radius = r == 0 ? 0.0 : c.inner_radius * std::pow(ratio, static_cast<double>(r - 1) / (c.rings - 1));
            // Radial spacing of the ring, used to fade waves the grid is too coarse to represent, and to widen
            // shore features until the grid can show them.
            const double cell = std::max(radius * std::log(ratio) / (c.rings - 1), 0.5);
            for (std::size_t i = 0; i < m.waves.size(); ++i)
                lod[i] = std::clamp((m.waves[i].wavelength / cell - 3.0) / 5.0, 0.0, 1.0);
            const double shore_width = std::max(c.foam_depth, 0.0);
            const double range = radius <= c.shore_range ? 0.0 : -1.0;
            for (int s = 0; s < c.segments; ++s)
            {
                const double angle = 2.0 * pi * s / c.segments;
                const double px = cx + radius * std::cos(angle), pz = cz + radius * std::sin(angle);
                double depth = m.depth_at(px, pz, range);
                if (depth < no_shore && radius > 50.0)
                {
                    // The terrain is noisy at a finer scale than the grid, so a single sample flickers as the grid
                    // slides. Averaging over the cell's footprint makes tint, shoaling and alpha change smoothly.
                    const double half = 0.5 * std::max(cell, radius * 2.0 * pi / c.segments);
                    const double ca = std::cos(angle), sa = std::sin(angle);
                    depth = 0.4 * depth + 0.15 * (m.depth_at(px + ca * half, pz + sa * half, range) + m.depth_at(px - ca * half, pz - sa * half, range) +
                                                  m.depth_at(px - sa * half, pz + ca * half, range) + m.depth_at(px + sa * half, pz - ca * half, range));
                }
                const Surface p = m.surface(px, pz, time, lod.data(), depth);
                Vertex3D &v = m.vertices[static_cast<std::size_t>(r) * static_cast<std::size_t>(c.segments) + static_cast<std::size_t>(s)];
                v.position[0] = static_cast<float>(p.position.x - eye.x);
                v.position[1] = static_cast<float>(p.position.y - eye.y);
                v.position[2] = static_cast<float>(p.position.z - eye.z);
                v.normal[0] = p.normal.x;
                v.normal[1] = p.normal.y;
                v.normal[2] = p.normal.z;

                const float height = static_cast<float>(p.position.y - c.sea_level);
                const float lift = std::clamp(height * 0.12f + 0.4f, 0.0f, 1.0f);
                float foam = p.foam * static_cast<float>(lod[0] * 0.6 + 0.4);
                glm::vec3 base = c.deep_colour;
                float alpha = 1.0f;
                if (depth < no_shore)
                {
                    const auto shallow = static_cast<float>(1.0 - smoothstep(0.0, std::max(c.shallow_depth, 0.1), depth));
                    // Far vertices are too coarse to follow the seabed, so the tint fades to plain deep water.
                    const auto tint_range = static_cast<float>(1.0 - smoothstep(120.0, 600.0, radius));
                    base = glm::mix(c.deep_colour, c.shallow_colour, shallow * tint_range);
                    // A coarse ring cannot hold a thin line of foam, so the band widens with the grid.
                    // Foam and clear-water alpha are sampled per vertex, so on the coarse far rings they flicker as the
                    // grid slides. Past ~50 m they fade out and the terrain's depth test draws the exact shoreline.
                    const float near_shore = static_cast<float>(1.0 - smoothstep(50.0, 220.0, radius));
                    foam = std::max(foam, near_shore * static_cast<float>(m.shore_foam(depth, time, std::max(shore_width, 0.7 * cell))));
                    if (transparent)
                        alpha = static_cast<float>(glm::mix(1.0, 0.25 + 0.75 * smoothstep(0.0, c.clear_depth, depth), near_shore));
                    // Vertices under land are hidden by the terrain, so keep them continuous with the water's edge
                    // rather than dropping to zero: that stops the shoreline crawling as the grid slides.
                    if (depth <= 0.0)
                        alpha = transparent ? 0.25f + 0.75f * (1.0f - near_shore) : 1.0f;
                }
                v.colour[0] = base.x + 0.07f * lift + 0.85f * foam;
                v.colour[1] = base.y + 0.26f * lift + 0.85f * foam;
                v.colour[2] = base.z + 0.26f * lift + 0.85f * foam;
                v.colour[3] = std::max(alpha, foam);
            }
        }
        update_mesh_vertices(m.mesh, m.vertices);
    }

    Material Ocean::default_material() const
    {
        Material water;
        water.pbr = true;
        water.metallic = 0.0f;
        water.roughness = 0.14f;
        water.cull = CullMode::none;
        if (impl_->terrain && impl_->config.clear_depth > 0.0)
            water.blend = MaterialBlend::alpha;
        return water;
    }

    bool Ocean::draw(const Material &material) const
    {
        if (!impl_->mesh)
            return false;
        draw_mesh(impl_->mesh, glm::mat4(1.0f), material);
        return true;
    }
} // namespace sl
