#include "clutter.h"

#include "error.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <unordered_map>

namespace sl
{
    namespace
    {
        constexpr double pi = 3.14159265358979323846;

        struct CellKey
        {
            int face = 0;
            int x = 0;
            int z = 0;
            bool operator==(const CellKey &other) const { return face == other.face && x == other.x && z == other.z; }
        };

        struct CellKeyHash
        {
            std::size_t operator()(const CellKey &key) const noexcept
            {
                std::uint64_t h = static_cast<std::uint64_t>(key.face) * 0x9E3779B97F4A7C15ull;
                h ^= static_cast<std::uint64_t>(static_cast<std::uint32_t>(key.x)) * 0xC2B2AE3D27D4EB4Full + (h << 6) + (h >> 2);
                h ^= static_cast<std::uint64_t>(static_cast<std::uint32_t>(key.z)) * 0x165667B19E3779F9ull + (h << 6) + (h >> 2);
                return static_cast<std::size_t>(h);
            }
        };

        std::uint64_t mix64(std::uint64_t z)
        {
            z += 0x9E3779B97F4A7C15ull;
            z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
            z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
            return z ^ (z >> 31);
        }

        /** Deterministic stream of values in [0, 1). */
        struct Random
        {
            std::uint64_t state;
            double next()
            {
                state = mix64(state);
                return static_cast<double>(state >> 11) * (1.0 / 9007199254740992.0);
            }
        };

        struct FaceBasis
        {
            dvec3 n, u, v;
        };

        FaceBasis face_basis(int face)
        {
            static const dvec3 normals[6] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
            FaceBasis basis;
            basis.n = normals[std::clamp(face, 0, 5)];
            basis.u = std::fabs(basis.n.y) > 0.5 ? dvec3(1, 0, 0) : dvec3(0, 1, 0);
            basis.v = glm::cross(basis.u, basis.n);
            return basis;
        }

        /** Unit direction of the point (s, t) in -1..1 on a cube face, with the tangent warp that evens out cell sizes. */
        dvec3 face_direction(int face, double s, double t)
        {
            const FaceBasis basis = face_basis(face);
            const dvec3 cube = basis.n + s * basis.u + t * basis.v;
            return glm::normalize(dvec3(std::tan(cube.x * pi * 0.25), std::tan(cube.y * pi * 0.25), std::tan(cube.z * pi * 0.25)));
        }

        struct SpeciesCell
        {
            bool generated = false;
            std::vector<Instance3D> instances;
            Mesh *near_mesh = nullptr;
            std::vector<Mesh *> view_meshes;
        };

        struct Cell
        {
            CellKey key;
            dvec3 origin{0.0};
            dvec3 up{0.0, 1.0, 0.0};
            std::uint64_t used = 0;
            std::vector<SpeciesCell> species;
        };

        struct Candidate
        {
            Cell *cell;
            double distance;
            double priority;
        };

        struct DrawItem
        {
            Mesh *mesh;
            dvec3 origin;
            Material material;
            bool billboard = false;
        };
    } // namespace

    struct ClutterLayer::Impl
    {
        struct Species
        {
            ClutterSpecies spec;
            Impostor *impostor = nullptr;
            double radius = 0.0, height = 0.0;
        };

        const Terrain &terrain;
        double turn = 0.0;
        float impostor_light = 1.0f;
        dvec3 axis{0.0, 1.0, 0.0};

        // Cells live in the body-fixed frame but the terrain's direction queries take and return world vectors, so a
        // turning planet needs the turn applied going in and undone coming out.
        glm::dquat turn_quat() const { return glm::angleAxis(turn, axis); }
        dvec3 query_direction(const dvec3 &body_direction) const { return turn != 0.0 ? turn_quat() * body_direction : body_direction; }
        dvec3 body_vector(const dvec3 &world_vector) const { return turn != 0.0 ? glm::inverse(turn_quat()) * world_vector : world_vector; }
        dvec3 body_point(const dvec3 &world_point) const
        {
            if (turn == 0.0)
                return world_point;
            const dvec3 centre = terrain.config().origin;
            return centre + glm::inverse(turn_quat()) * (world_point - centre);
        }
        ClutterConfig config;
        bool sphere;
        int grid = 1;
        std::vector<Species> species;
        std::unordered_map<CellKey, Cell, CellKeyHash> cells;
        std::vector<DrawItem> items;
        ClutterStats stats;
        std::uint64_t frame = 0;

        Impl(const Terrain &t, const ClutterConfig &c)
            : terrain(t), config(c), sphere(t.config().shape == TerrainShape::sphere)
        {
            config.cell_size = std::max(config.cell_size, 1.0f);
            config.time_budget_ms = std::max(config.time_budget_ms, 0.0);
            if (sphere)
                grid = std::max(1, static_cast<int>(std::ceil(terrain.config().radius * pi * 0.5 / config.cell_size)));
        }

        ~Impl()
        {
            for (auto &entry : cells)
                destroy_cell_meshes(entry.second);
            for (Species &s : species)
                destroy_impostor(s.impostor);
        }

        static void destroy_cell_meshes(Cell &cell)
        {
            for (SpeciesCell &sc : cell.species)
            {
                if (sc.near_mesh)
                    destroy_mesh(sc.near_mesh);
                sc.near_mesh = nullptr;
                for (Mesh *&mesh : sc.view_meshes)
                {
                    if (mesh)
                        destroy_mesh(mesh);
                    mesh = nullptr;
                }
            }
        }

        double max_far() const
        {
            double far = 0.0;
            for (const Species &s : species)
                far = std::max(far, static_cast<double>(s.spec.far_distance));
            return far;
        }

        Cell *find_or_create(const CellKey &key)
        {
            auto it = cells.find(key);
            if (it != cells.end())
                return &it->second;
            Cell cell;
            cell.key = key;
            if (sphere)
            {
                const double s = -1.0 + 2.0 * (key.x + 0.5) / grid;
                const double t = -1.0 + 2.0 * (key.z + 0.5) / grid;
                const dvec3 direction = face_direction(key.face, s, t);
                cell.origin = body_point(terrain.surface_point(query_direction(direction)));
                cell.up = direction;
            }
            else
            {
                const double x = (key.x + 0.5) * config.cell_size;
                const double z = (key.z + 0.5) * config.cell_size;
                cell.origin = {x, terrain.height_at(x, z), z};
            }
            return &cells.emplace(key, std::move(cell)).first->second;
        }

        void generate(Cell &cell, std::size_t index)
        {
            const ClutterSpecies &spec = species[index].spec;
            SpeciesCell &out = cell.species[index];
            out.generated = true;
            out.instances.clear();
            const TerrainConfig &tc = terrain.config();
            const double area = static_cast<double>(config.cell_size) * config.cell_size;
            Random random{mix64(spec.seed * 0x9E3779B1ull) ^ CellKeyHash{}(cell.key)};
            const double expected = std::max(0.0, static_cast<double>(spec.density)) * area;
            int count = static_cast<int>(std::floor(expected));
            if (random.next() < expected - count)
                ++count;
            count = std::min(count, 20000);
            for (int k = 0; k < count; ++k)
            {
                // Every candidate draws the same number of values, so the rules of one do not shift the others.
                const double u1 = random.next(), u2 = random.next(), yaw = random.next(), size = random.next();
                const double shade = random.next(), roll = random.next();
                dvec3 position, normal, up;
                double height;
                if (sphere)
                {
                    const double s = -1.0 + 2.0 * (cell.key.x + u1) / grid;
                    const double t = -1.0 + 2.0 * (cell.key.z + u2) / grid;
                    const dvec3 direction = face_direction(cell.key.face, s, t);
                    const dvec3 query = query_direction(direction);
                    height = terrain.height_at_direction(query);
                    position = body_point(terrain.surface_point(query));
                    normal = body_vector(terrain.normal_at_direction(query));
                    up = direction;
                }
                else
                {
                    const double x = (cell.key.x + u1) * config.cell_size;
                    const double z = (cell.key.z + u2) * config.cell_size;
                    if (x < tc.origin.x || z < tc.origin.z || x > tc.origin.x + tc.size || z > tc.origin.z + tc.size)
                        continue;
                    const double y = terrain.height_at(x, z);
                    height = y - tc.origin.y;
                    position = {x, y, z};
                    normal = terrain.normal_at(x, z);
                    up = {0.0, 1.0, 0.0};
                }
                const double slope = glm::degrees(std::acos(std::clamp(glm::dot(normal, up), -1.0, 1.0)));
                if (height < spec.min_height || height > spec.max_height || slope < spec.min_slope || slope > spec.max_slope)
                    continue;
                if (spec.suitability)
                {
                    TerrainSample sample;
                    sample.position = sphere ? up : position;
                    sample.height = height;
                    sample.normal = normal;
                    sample.slope_degrees = slope;
                    if (roll >= static_cast<double>(spec.suitability(sample)))
                        continue;
                }
                Instance3D instance;
                instance.position = glm::vec3(position - cell.origin);
                instance.yaw = static_cast<float>(yaw * 2.0 * pi);
                instance.scale = static_cast<float>(spec.min_scale + (spec.max_scale - spec.min_scale) * size);
                const double lean = std::clamp(static_cast<double>(spec.lean), 0.0, 1.0);
                instance.up = glm::vec3(glm::normalize(glm::mix(up, normal, lean)));
                const float brightness = 1.0f - std::clamp(spec.tint_variation, 0.0f, 1.0f) * static_cast<float>(shade);
                const auto channel = static_cast<std::uint8_t>(std::lround(255.0f * brightness));
                instance.tint = {channel, channel, channel, 255};
                out.instances.push_back(instance);
            }
        }

        void collect(const TerrainView &view, double reach, std::vector<Cell *> &found)
        {
            const double cs = config.cell_size;
            const TerrainConfig &tc = terrain.config();
            const int radius = static_cast<int>(std::ceil(reach / cs)) + 1;
            if (!sphere)
            {
                const int cx = static_cast<int>(std::floor(view.position.x / cs));
                const int cz = static_cast<int>(std::floor(view.position.z / cs));
                const int min_x = static_cast<int>(std::floor(tc.origin.x / cs));
                const int max_x = static_cast<int>(std::floor((tc.origin.x + tc.size) / cs));
                const int min_z = static_cast<int>(std::floor(tc.origin.z / cs));
                const int max_z = static_cast<int>(std::floor((tc.origin.z + tc.size) / cs));
                for (int z = std::max(cz - radius, min_z); z <= std::min(cz + radius, max_z); ++z)
                    for (int x = std::max(cx - radius, min_x); x <= std::min(cx + radius, max_x); ++x)
                    {
                        const double dx = (x + 0.5) * cs - view.position.x;
                        const double dz = (z + 0.5) * cs - view.position.z;
                        if (std::sqrt(dx * dx + dz * dz) <= reach + cs)
                            found.push_back(find_or_create({0, x, z}));
                    }
                return;
            }
            const dvec3 offset = view.position - tc.origin;
            const double length = glm::length(offset);
            if (length <= 0.0)
                return;
            const dvec3 direction = offset / length;
            for (int face = 0; face < 6; ++face)
            {
                const FaceBasis basis = face_basis(face);
                const double along = glm::dot(direction, basis.n);
                if (along <= 1.0e-6)
                    continue;
                const dvec3 cube = direction / along;
                const double s = std::clamp(std::atan(glm::dot(cube, basis.u)) * 4.0 / pi, -1.0, 1.0);
                const double t = std::clamp(std::atan(glm::dot(cube, basis.v)) * 4.0 / pi, -1.0, 1.0);
                const int ci = std::clamp(static_cast<int>(std::floor((s + 1.0) * 0.5 * grid)), 0, grid - 1);
                const int cj = std::clamp(static_cast<int>(std::floor((t + 1.0) * 0.5 * grid)), 0, grid - 1);
                for (int j = std::max(cj - radius, 0); j <= std::min(cj + radius, grid - 1); ++j)
                    for (int i = std::max(ci - radius, 0); i <= std::min(ci + radius, grid - 1); ++i)
                    {
                        // Ground distance along the unperturbed sphere rejects most cells without sampling the height.
                        const dvec3 centre = face_direction(face, -1.0 + 2.0 * (i + 0.5) / grid, -1.0 + 2.0 * (j + 0.5) / grid);
                        const double ground = std::acos(std::clamp(glm::dot(centre, direction), -1.0, 1.0)) * tc.radius;
                        if (ground <= reach + cs)
                            found.push_back(find_or_create({face, i, j}));
                    }
            }
        }

        void update(const TerrainView &view)
        {
            ++frame;
            items.clear();
            stats = {};
            if (species.empty())
                return;
            std::vector<Cell *> found;
            collect(view, max_far(), found);

            const double half_diagonal = std::atan(std::tan(glm::radians(view.fov_y_degrees) * 0.5) *
                                                   std::sqrt(1.0 + view.aspect * view.aspect));
            const double cell_radius = config.cell_size * 0.75;
            std::vector<Candidate> candidates;
            candidates.reserve(found.size());
            for (Cell *cell : found)
            {
                cell->used = frame;
                if (cell->species.size() < species.size())
                    cell->species.resize(species.size());
                const dvec3 to_cell = cell->origin - view.position;
                const double distance = glm::length(to_cell);
                // Cells in front are built first; those behind wait but are kept so turning round is cheap.
                const double facing = distance > 1.0e-6 ? glm::dot(to_cell / distance, view.forward) : 1.0;
                candidates.push_back({cell, distance, distance * (facing > 0.2 ? 1.0 : 3.0)});
            }
            std::sort(candidates.begin(), candidates.end(),
                      [](const Candidate &a, const Candidate &b) { return a.priority < b.priority; });

            // At least one piece of work is always allowed, so progress continues however small the budget is.
            const auto started = std::chrono::steady_clock::now();
            int done = 0;
            auto has_budget = [&]()
            {
                return done == 0 || std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count() <
                                        config.time_budget_ms;
            };
            for (const Candidate &candidate : candidates)
            {
                Cell &cell = *candidate.cell;
                const dvec3 to_cell = cell.origin - view.position;
                const double distance = candidate.distance;
                const double reach = std::min(half_diagonal + std::asin(std::min(1.0, cell_radius / std::max(distance, 1.0e-6))), pi);
                const bool visible = distance <= cell_radius ||
                                     glm::dot(to_cell / distance, view.forward) >= std::cos(reach);
                for (std::size_t s = 0; s < species.size(); ++s)
                {
                    const Species &sp = species[s];
                    SpeciesCell &sc = cell.species[s];
                    const double far = sp.spec.far_distance;
                    const double near = sp.impostor && sp.impostor->views.size() > 0 ? sp.spec.near_distance : far;
                    if (distance > far + cell_radius)
                        continue;
                    if (!sc.generated)
                    {
                        if (!has_budget())
                            continue;
                        generate(cell, s);
                        ++done;
                    }
                    if (sc.instances.empty())
                        continue;
                    if (distance <= near)
                    {
                        if (!sc.near_mesh && has_budget())
                        {
                            sc.near_mesh = bake_instances(sp.spec.mesh, sc.instances);
                            ++done;
                        }
                        if (sc.near_mesh && visible)
                        {
                            items.push_back({sc.near_mesh, cell.origin, sp.spec.material});
                            ++stats.drawn_meshes;
                            stats.drawn_objects += static_cast<int>(sc.instances.size());
                        }
                    }
                    else if (sp.impostor && distance <= far)
                    {
                        const int views = static_cast<int>(sp.impostor->views.size());
                        const int index = impostor_view(*sp.impostor, glm::vec3(-to_cell / distance), glm::vec3(cell.up));
                        if (sc.view_meshes.size() != static_cast<std::size_t>(views))
                            sc.view_meshes.assign(static_cast<std::size_t>(views), nullptr);
                        Mesh *&mesh = sc.view_meshes[static_cast<std::size_t>(index)];
                        if (!mesh && has_budget())
                        {
                            mesh = bake_billboards(*sp.impostor, sc.instances, index);
                            ++done;
                        }
                        if (mesh && visible)
                        {
                            Material material = impostor_material(*sp.impostor, index);
                            material.fog = sp.spec.material.fog;
                            items.push_back({mesh, cell.origin, material, true});
                            ++stats.drawn_meshes;
                            stats.drawn_billboards += static_cast<int>(sc.instances.size());
                        }
                    }
                }
            }
            evict(found.size());
            stats.cached_cells = static_cast<int>(cells.size());
        }

        void evict(std::size_t in_range)
        {
            // Never evict the cells in range this frame, however small the configured limit is.
            const std::size_t limit = std::max(static_cast<std::size_t>(std::max(config.max_cached_cells, 1)),
                                               in_range + in_range / 4);
            if (cells.size() <= limit)
                return;
            std::vector<std::pair<std::uint64_t, CellKey>> stale;
            for (const auto &entry : cells)
                if (entry.second.used != frame)
                    stale.emplace_back(entry.second.used, entry.first);
            std::sort(stale.begin(), stale.end(), [](const auto &a, const auto &b) { return a.first < b.first; });
            for (const auto &old : stale)
            {
                if (cells.size() <= limit)
                    break;
                const auto it = cells.find(old.second);
                destroy_cell_meshes(it->second);
                cells.erase(it);
            }
        }
    };

    ClutterLayer::ClutterLayer(const Terrain &terrain, const ClutterConfig &config)
        : impl_(std::make_unique<Impl>(terrain, config))
    {
    }

    ClutterLayer::~ClutterLayer() = default;

    int ClutterLayer::add_species(const ClutterSpecies &species)
    {
        Impl::Species entry;
        entry.spec = species;
        if (species.mesh.vertices.empty())
        {
            detail::set_error("ClutterLayer::add_species needs a mesh");
            return -1;
        }
        if (species.impostor)
        {
            entry.impostor = bake_impostor(species.mesh, species.material, species.impostor_views, species.impostor_tile);
            if (!entry.impostor)
                return -1;
        }
        {
            // The trunk or base: the horizontal reach of the vertices in the lowest third of the object.
            float top = 0.0f;
            for (const Vertex3D &v : species.mesh.vertices)
                top = std::max(top, v.position[1]);
            float reach = 0.0f;
            for (const Vertex3D &v : species.mesh.vertices)
                if (v.position[1] <= top * 0.3f)
                    reach = std::max(reach, std::hypot(v.position[0], v.position[2]));
            entry.radius = std::max(reach, 0.1f);
            entry.height = top;
        }
        impl_->species.push_back(std::move(entry));
        return static_cast<int>(impl_->species.size()) - 1;
    }

    int ClutterLayer::species_count() const { return static_cast<int>(impl_->species.size()); }

    void ClutterLayer::update(const TerrainView &view)
    {
        // Cells are built in the planet's body-fixed frame, so a turning planet takes the view in that frame and
        // draw() turns the result back into the world.
        const Terrain &terrain = impl_->terrain;
        impl_->turn = terrain.config().shape == TerrainShape::sphere ? terrain.rotation() : 0.0;
        if (impl_->turn == 0.0)
        {
            impl_->update(view);
            return;
        }
        impl_->axis = terrain.rotation_axis();
        const dvec3 centre = terrain.config().origin;
        const glm::dquat inverse = glm::inverse(glm::angleAxis(impl_->turn, impl_->axis));
        TerrainView v = view;
        v.position = centre + inverse * (view.position - centre);
        v.forward = inverse * view.forward;
        v.up = inverse * view.up;
        impl_->update(v);
    }

    void ClutterLayer::set_impostor_light(float brightness) { impl_->impostor_light = std::clamp(brightness, 0.0f, 1.0f); }

    bool ClutterLayer::draw(const dvec3 &origin) const
    {
        bool ok = true;
        const bool turning = impl_->turn != 0.0;
        const dvec3 centre = impl_->terrain.config().origin;
        const glm::dquat turn = glm::angleAxis(impl_->turn, impl_->axis);
        for (const DrawItem &item : impl_->items)
        {
            const dvec3 placed = turning ? centre + turn * (item.origin - centre) : item.origin;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(placed - origin));
            if (turning)
                model = glm::rotate(model, static_cast<float>(impl_->turn), glm::vec3(impl_->axis));
            if (item.billboard && impl_->impostor_light < 1.0f)
            {
                Material dimmed = item.material;
                const float k = impl_->impostor_light;
                dimmed.tint.red = static_cast<std::uint8_t>(dimmed.tint.red * k);
                dimmed.tint.green = static_cast<std::uint8_t>(dimmed.tint.green * k);
                dimmed.tint.blue = static_cast<std::uint8_t>(dimmed.tint.blue * k);
                ok = draw_mesh(item.mesh, model, dimmed) && ok;
            }
            else
                ok = draw_mesh(item.mesh, model, item.material) && ok;
        }
        return ok;
    }

    void ClutterLayer::invalidate_all()
    {
        for (auto &entry : impl_->cells)
            Impl::destroy_cell_meshes(entry.second);
        impl_->cells.clear();
        impl_->items.clear();
    }

    void ClutterLayer::invalidate_region(double x0, double z0, double x1, double z1)
    {
        if (impl_->sphere)
        {
            invalidate_all();
            return;
        }
        const double cs = impl_->config.cell_size;
        impl_->items.clear();
        for (auto it = impl_->cells.begin(); it != impl_->cells.end();)
        {
            const double cx0 = it->first.x * cs, cz0 = it->first.z * cs;
            if (cx0 <= x1 && cx0 + cs >= x0 && cz0 <= z1 && cz0 + cs >= z0)
            {
                Impl::destroy_cell_meshes(it->second);
                it = impl_->cells.erase(it);
            }
            else
                ++it;
        }
    }

    ClutterStats ClutterLayer::stats() const { return impl_->stats; }

    void ClutterLayer::obstacles(const dvec3 &point, double range, std::vector<ClutterObstacle> &out) const
    {
        const Impl &impl = *impl_;
        const dvec3 centre = impl.terrain.config().origin;
        const dvec3 here = impl.body_point(point);
        const glm::dquat turn = impl.turn_quat();
        for (const auto &entry : impl.cells)
        {
            const Cell &cell = entry.second;
            if (glm::length(cell.origin - here) > range + impl.config.cell_size * 1.5)
                continue;
            for (std::size_t i = 0; i < cell.species.size() && i < impl.species.size(); ++i)
            {
                const Impl::Species &sp = impl.species[i];
                if (!sp.spec.solid)
                    continue;
                for (const Instance3D &inst : cell.species[i].instances)
                {
                    const dvec3 body_position = cell.origin + dvec3(inst.position);
                    if (glm::length(body_position - here) > range)
                        continue;
                    ClutterObstacle o;
                    o.position = impl.turn != 0.0 ? centre + turn * (body_position - centre) : body_position;
                    o.radius = sp.radius * inst.scale;
                    o.height = sp.height * inst.scale;
                    o.species = static_cast<int>(i);
                    out.push_back(o);
                }
            }
        }
    }

} // namespace sl
