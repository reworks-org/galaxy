#include "fluid3d.h"

#include <algorithm>
#include <cstddef>

namespace sl
{
    namespace
    {
        using Sim = FluidSimulation3D;

        constexpr int BLOCK = 8;
        constexpr int H4[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        constexpr int H8[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        constexpr int N6[6][3] = {{0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}};

        inline std::size_t cell_index(const Sim &s, int x, int y, int z)
        {
            return (static_cast<std::size_t>(y) * s.depth + z) * s.width + x;
        }

        inline bool in_grid(const Sim &s, int x, int y, int z)
        {
            return x >= 0 && y >= 0 && z >= 0 && x < s.width && y < s.height && z < s.depth;
        }

        // Cells on the outermost layer are context only and never simulated or moved into.
        inline bool inside(const Sim &s, int x, int y, int z)
        {
            return x > 0 && y > 0 && z > 0 && x < s.width - 1 && y < s.height - 1 && z < s.depth - 1;
        }

        inline FluidCell &at(Sim &s, int x, int y, int z)
        {
            return s.cells[cell_index(s, x, y, z)];
        }

        inline std::uint32_t rnd(Sim &s)
        {
            s.rng_state = s.rng_state * 1103515245U + 12345U;
            return (s.rng_state / 65536U) % 32768U;
        }

        inline int block_id(const Sim &s, int bx, int by, int bz)
        {
            return (by * s.blocks_z + bz) * s.blocks_x + bx;
        }

        // Wakes the block containing the cell and any neighbouring block the cell sits on the edge of.
        void touch(Sim &s, int x, int y, int z)
        {
            const int bx = x / BLOCK, by = y / BLOCK, bz = z / BLOCK;
            const int lx = x % BLOCK, ly = y % BLOCK, lz = z % BLOCK;
            for (int dy = -1; dy <= 1; ++dy)
            {
                if ((dy < 0 && ly != 0) || (dy > 0 && ly != BLOCK - 1))
                    continue;
                for (int dz = -1; dz <= 1; ++dz)
                {
                    if ((dz < 0 && lz != 0) || (dz > 0 && lz != BLOCK - 1))
                        continue;
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        if ((dx < 0 && lx != 0) || (dx > 0 && lx != BLOCK - 1))
                            continue;
                        const int nx = bx + dx, ny = by + dy, nz = bz + dz;
                        if (nx < 0 || ny < 0 || nz < 0 || nx >= s.blocks_x || ny >= s.blocks_y || nz >= s.blocks_z)
                            continue;
                        s.awake[block_id(s, nx, ny, nz)] = 1;
                    }
                }
            }
        }

        inline bool empty_at(Sim &s, int x, int y, int z)
        {
            return inside(s, x, y, z) && at(s, x, y, z).type == FluidElement::empty;
        }

        void swap_cells(Sim &s, int x0, int y0, int z0, int x1, int y1, int z1)
        {
            FluidCell &a = at(s, x0, y0, z0);
            FluidCell &b = at(s, x1, y1, z1);
            std::swap(a, b);
            a.turn = s.turn;
            b.turn = s.turn;
            touch(s, x0, y0, z0);
            touch(s, x1, y1, z1);
        }

        void move_cell(Sim &s, int x0, int y0, int z0, int x1, int y1, int z1)
        {
            FluidCell &from = at(s, x0, y0, z0);
            FluidCell &to = at(s, x1, y1, z1);
            to = from;
            from = {};
            to.turn = s.turn;
            touch(s, x0, y0, z0);
            touch(s, x1, y1, z1);
        }

        inline bool is_liquid_element(FluidElement type)
        {
            return type == FluidElement::water || type == FluidElement::oil || type == FluidElement::acid || type == FluidElement::lava;
        }

        inline float liquid_density(FluidElement type)
        {
            switch (type)
            {
            case FluidElement::water:
                return 1.0f;
            case FluidElement::oil:
                return 0.65f;
            case FluidElement::acid:
                return 1.25f;
            case FluidElement::lava:
                return 2.4f;
            default:
                return 0.0f;
            }
        }

        inline bool powder_passable(FluidElement type)
        {
            return type == FluidElement::empty || type == FluidElement::water || type == FluidElement::oil ||
                   type == FluidElement::acid || type == FluidElement::smoke || type == FluidElement::steam ||
                   type == FluidElement::fire;
        }

        // Elements that react without outside contact, so their block can never sleep.
        inline bool always_active(FluidElement type)
        {
            switch (type)
            {
            case FluidElement::fire:
            case FluidElement::lava:
            case FluidElement::smoke:
            case FluidElement::steam:
            case FluidElement::plant:
            case FluidElement::pump:
            case FluidElement::acid:
                return true;
            default:
                return false;
            }
        }

        void ignite(FluidCell &n, std::uint8_t life, float temp)
        {
            n.type = FluidElement::fire;
            n.life = life;
            n.temp = temp;
        }

        void update_heat(Sim &s, int x, int y, int z)
        {
            FluidCell &cell = at(s, x, y, z);
            if (cell.temp <= 20.0f && cell.type != FluidElement::fire && cell.type != FluidElement::lava)
                return;

            // Heat convects upwards: up 0.40, horizontal 0.10 each, down 0.05.
            static constexpr float weights[6] = {0.40f, 0.05f, 0.10f, 0.10f, 0.10f, 0.10f};
            for (int i = 0; i < 6; ++i)
            {
                const int nx = x + N6[i][0], ny = y + N6[i][1], nz = z + N6[i][2];
                if (!inside(s, nx, ny, nz))
                    continue;
                FluidCell &neigh = at(s, nx, ny, nz);
                if (neigh.type == FluidElement::solid)
                    continue;
                const float diff = cell.temp - neigh.temp;
                if (diff <= 0.0f)
                    continue;
                const float transfer = diff * weights[i] * 0.5f;
                cell.temp -= transfer;
                neigh.temp += transfer;
                if (neigh.temp > 21.0f)
                    touch(s, nx, ny, nz);

                if ((neigh.type == FluidElement::wood || neigh.type == FluidElement::plant || neigh.type == FluidElement::oil ||
                     neigh.type == FluidElement::gunpowder) &&
                    neigh.temp >= 200.0f)
                {
                    if (neigh.type == FluidElement::gunpowder)
                        ignite(neigh, 50, 800.0f);
                    else
                        ignite(neigh, 60, std::max(neigh.temp, 500.0f));
                }
            }

            if (cell.type != FluidElement::fire && cell.type != FluidElement::lava)
            {
                cell.temp += (20.0f - cell.temp) * 0.02f;
                if (cell.temp < 20.5f)
                    cell.temp = 20.0f;
            }
        }

        void update_wood(Sim &s, int x, int y, int z)
        {
            FluidCell &cell = at(s, x, y, z);
            if (cell.temp > 220.0f && (rnd(s) % 6) == 0)
            {
                ignite(cell, 80, 500.0f);
                touch(s, x, y, z);
            }
        }

        void update_sand(Sim &s, int x, int y, int z)
        {
            if (y - 1 < 1)
                return;
            if (powder_passable(at(s, x, y - 1, z).type))
            {
                swap_cells(s, x, y, z, x, y - 1, z);
                return;
            }

            // Slide down-and-sideways in a random order; both the diagonal target and the side cell must be passable.
            const int start = static_cast<int>(rnd(s) % 8);
            for (int k = 0; k < 8; ++k)
            {
                const int *d = H8[(start + k) % 8];
                const int nx = x + d[0], nz = z + d[1];
                if (!inside(s, nx, y - 1, nz) || !powder_passable(at(s, nx, y - 1, nz).type))
                    continue;
                if (d[0] != 0 && (!inside(s, nx, y, z) || !powder_passable(at(s, nx, y, z).type)))
                    continue;
                if (d[1] != 0 && (!inside(s, x, y, nz) || !powder_passable(at(s, x, y, nz).type)))
                    continue;
                swap_cells(s, x, y, z, nx, y - 1, nz);
                return;
            }
        }

        void update_liquid(Sim &s, int x, int y, int z)
        {
            FluidCell &cell = at(s, x, y, z);

            if (cell.type == FluidElement::lava)
            {
                for (const auto &d : N6)
                {
                    const int nx = x + d[0], ny = y + d[1], nz = z + d[2];
                    if (!inside(s, nx, ny, nz))
                        continue;
                    FluidCell &n = at(s, nx, ny, nz);
                    if (n.type == FluidElement::water)
                    {
                        cell.type = FluidElement::solid;
                        cell.temp = 300.0f;
                        n.type = FluidElement::steam;
                        n.life = 60;
                        touch(s, x, y, z);
                        touch(s, nx, ny, nz);
                        return;
                    }
                    if (n.type == FluidElement::wood || n.type == FluidElement::plant || n.type == FluidElement::oil)
                        ignite(n, 70, 800.0f);
                    else if (n.type == FluidElement::gunpowder)
                        ignite(n, 50, 900.0f);
                }
            }

            if (cell.type == FluidElement::acid)
            {
                for (const auto &d : N6)
                {
                    const int nx = x + d[0], ny = y + d[1], nz = z + d[2];
                    if (!inside(s, nx, ny, nz))
                        continue;
                    FluidCell &n = at(s, nx, ny, nz);
                    const bool corrodes_solid = n.type == FluidElement::solid && nx > 1 && ny > 1 && nz > 1 &&
                                                nx < s.width - 2 && ny < s.height - 2 && nz < s.depth - 2;
                    if ((n.type == FluidElement::wood || n.type == FluidElement::plant || n.type == FluidElement::sand || corrodes_solid) &&
                        (rnd(s) % 3) == 0)
                    {
                        n.type = FluidElement::smoke;
                        n.life = 35;
                        cell.type = FluidElement::empty;
                        cell.mass = 0.0f;
                        touch(s, x, y, z);
                        touch(s, nx, ny, nz);
                        return;
                    }
                }
            }

            if (y - 1 < 1)
                return;
            FluidCell &below = at(s, x, y - 1, z);
            if (below.type == FluidElement::empty)
            {
                swap_cells(s, x, y, z, x, y - 1, z);
                return;
            }
            // Only water quenches fire; lava and oil sit on top of it.
            if (below.type == FluidElement::fire && cell.type == FluidElement::water)
            {
                below.type = FluidElement::steam;
                below.life = 40;
                cell.type = FluidElement::empty;
                cell.mass = 0.0f;
                touch(s, x, y, z);
                return;
            }
            if (is_liquid_element(below.type) && liquid_density(below.type) < liquid_density(cell.type))
            {
                swap_cells(s, x, y, z, x, y - 1, z);
                return;
            }

            const int start = static_cast<int>(rnd(s) % 4);
            for (int k = 0; k < 4; ++k)
            {
                const int *d = H4[(start + k) % 4];
                const int nx = x + d[0], nz = z + d[1];
                if (empty_at(s, nx, y - 1, nz) && empty_at(s, nx, y, nz))
                {
                    swap_cells(s, x, y, z, nx, y - 1, nz);
                    return;
                }
            }

            // Lava is viscous and slides sideways only every other tick.
            const int flow_period = cell.type == FluidElement::lava ? 2 : 1;
            if (s.turn % flow_period == 0)
            {
                for (int k = 0; k < 4; ++k)
                {
                    const int *d = H4[(start + k) % 4];
                    const int nx = x + d[0], nz = z + d[1];
                    if (empty_at(s, nx, y, nz))
                    {
                        swap_cells(s, x, y, z, nx, y, nz);
                        return;
                    }
                }
            }
        }

        void update_gas(Sim &s, int x, int y, int z)
        {
            FluidCell &cell = at(s, x, y, z);
            if (empty_at(s, x, y + 1, z))
            {
                move_cell(s, x, y, z, x, y + 1, z);
                return;
            }
            const int start = static_cast<int>(rnd(s) % 4);
            for (int k = 0; k < 4; ++k)
            {
                const int *d = H4[(start + k) % 4];
                if (empty_at(s, x + d[0], y, z + d[1]))
                {
                    move_cell(s, x, y, z, x + d[0], y, z + d[1]);
                    return;
                }
            }
            if (cell.life > 0 && --cell.life == 0)
            {
                cell = {};
                touch(s, x, y, z);
            }
        }

        void update_fire(Sim &s, int x, int y, int z)
        {
            FluidCell &cell = at(s, x, y, z);
            cell.temp = std::max(cell.temp, 650.0f);

            if (cell.life == 0)
            {
                if ((rnd(s) % 3) == 0)
                {
                    cell.type = FluidElement::smoke;
                    cell.life = 40;
                }
                else
                {
                    cell.type = FluidElement::empty;
                }
                touch(s, x, y, z);
                return;
            }
            --cell.life;

            for (const auto &d : N6)
            {
                const int nx = x + d[0], ny = y + d[1], nz = z + d[2];
                if (!inside(s, nx, ny, nz))
                    continue;
                FluidCell &n = at(s, nx, ny, nz);
                if (n.type == FluidElement::oil || n.type == FluidElement::plant)
                    ignite(n, 70, 800.0f);
                else if (n.type == FluidElement::gunpowder)
                    ignite(n, 50, 900.0f);
                else if (n.type == FluidElement::wood && (rnd(s) % 5) == 0)
                    ignite(n, 90, 500.0f);
                else if (n.type == FluidElement::water)
                {
                    cell.type = FluidElement::steam;
                    cell.life = 50;
                    cell.temp = 200.0f;
                    touch(s, x, y, z);
                    return;
                }
                else
                    continue;
                touch(s, nx, ny, nz);
            }

            if ((rnd(s) % 3) == 0)
            {
                const int nx = x + static_cast<int>(rnd(s) % 3) - 1;
                const int nz = z + static_cast<int>(rnd(s) % 3) - 1;
                if (empty_at(s, nx, y + 1, nz))
                    swap_cells(s, x, y, z, nx, y + 1, nz);
            }
        }

        void update_pump(Sim &s, int x, int y, int z)
        {
            if (!inside(s, x, y + 1, z) || !inside(s, x, y - 1, z))
                return;
            const FluidElement src = at(s, x, y - 1, z).type;
            if (at(s, x, y + 1, z).type == FluidElement::empty &&
                (src == FluidElement::water || src == FluidElement::oil || src == FluidElement::acid ||
                 src == FluidElement::sand || src == FluidElement::gunpowder || src == FluidElement::smoke))
                move_cell(s, x, y - 1, z, x, y + 1, z);
        }

        void update_plant(Sim &s, int x, int y, int z)
        {
            for (const auto &d : N6)
            {
                const int nx = x + d[0], ny = y + d[1], nz = z + d[2];
                if (!inside(s, nx, ny, nz))
                    continue;
                FluidCell &n = at(s, nx, ny, nz);
                if (n.type != FluidElement::water)
                    continue;
                n.type = FluidElement::empty;
                n.mass = 0.0f;
                touch(s, nx, ny, nz);
                const int gx = x + static_cast<int>(rnd(s) % 3) - 1;
                const int gz = z + static_cast<int>(rnd(s) % 3) - 1;
                if (empty_at(s, gx, y + 1, gz))
                {
                    FluidCell &grow = at(s, gx, y + 1, gz);
                    grow.type = FluidElement::plant;
                    grow.turn = s.turn;
                    touch(s, gx, y + 1, gz);
                }
                return;
            }
        }

        void clear_wake_state(Sim &s)
        {
            s.blocks_x = (s.width + BLOCK - 1) / BLOCK;
            s.blocks_y = (s.height + BLOCK - 1) / BLOCK;
            s.blocks_z = (s.depth + BLOCK - 1) / BLOCK;
            s.awake.assign(static_cast<std::size_t>(s.blocks_x) * s.blocks_y * s.blocks_z, 1);
            s.block_cursor = 0;
        }

        bool element_from_block(const FluidVoxelBlocks &b, BlockId id, FluidElement &out)
        {
            if (id == b.water) out = FluidElement::water;
            else if (id == b.oil) out = FluidElement::oil;
            else if (id == b.acid) out = FluidElement::acid;
            else if (id == b.lava) out = FluidElement::lava;
            else if (id == b.sand) out = FluidElement::sand;
            else if (id == b.gunpowder) out = FluidElement::gunpowder;
            else if (id == b.wood) out = FluidElement::wood;
            else if (id == b.plant) out = FluidElement::plant;
            else if (id == b.pump) out = FluidElement::pump;
            else if (id == b.fire) out = FluidElement::fire;
            else if (id == b.smoke) out = FluidElement::smoke;
            else if (id == b.steam) out = FluidElement::steam;
            else return false;
            return true;
        }

        float default_temp(FluidElement type)
        {
            switch (type)
            {
            case FluidElement::lava:
                return 1200.0f;
            case FluidElement::fire:
                return 650.0f;
            case FluidElement::steam:
                return 100.0f;
            default:
                return 20.0f;
            }
        }
    } // namespace

    FluidSimulation3D *create_fluid_simulation_3d(int width, int height, int depth)
    {
        if (width < 3 || height < 3 || depth < 3)
            return nullptr;
        auto *simulation = new FluidSimulation3D;
        simulation->width = width;
        simulation->height = height;
        simulation->depth = depth;
        simulation->cells.resize(static_cast<std::size_t>(width) * height * depth);
        simulation->synced_blocks.assign(simulation->cells.size(), BLOCK_AIR);
        clear_wake_state(*simulation);
        return simulation;
    }

    void destroy_fluid_simulation_3d(FluidSimulation3D *simulation)
    {
        delete simulation;
    }

    void fluid3d_clear(FluidSimulation3D *simulation)
    {
        if (!simulation)
            return;
        std::fill(simulation->cells.begin(), simulation->cells.end(), FluidCell{});
        std::fill(simulation->synced_blocks.begin(), simulation->synced_blocks.end(), BLOCK_AIR);
        simulation->turn = 0;
        clear_wake_state(*simulation);
    }

    bool fluid3d_in_bounds(const FluidSimulation3D *simulation, int x, int y, int z)
    {
        return simulation && in_grid(*simulation, x, y, z);
    }

    bool fluid3d_set_cell(FluidSimulation3D *simulation, int x, int y, int z, const FluidCell &cell)
    {
        if (!fluid3d_in_bounds(simulation, x, y, z))
            return false;
        simulation->cells[cell_index(*simulation, x, y, z)] = cell;
        touch(*simulation, x, y, z);
        return true;
    }

    bool fluid3d_set_cell(FluidSimulation3D *simulation, int x, int y, int z, FluidElement type,
                          float mass, float temp, std::uint8_t life, std::int8_t variation)
    {
        FluidCell cell{};
        cell.type = type;
        cell.mass = mass;
        cell.temp = temp;
        cell.life = life;
        cell.variation = variation;
        cell.turn = simulation ? simulation->turn : 0;
        return fluid3d_set_cell(simulation, x, y, z, cell);
    }

    FluidCell fluid3d_get_cell(const FluidSimulation3D *simulation, int x, int y, int z)
    {
        if (!fluid3d_in_bounds(simulation, x, y, z))
            return {};
        return simulation->cells[cell_index(*simulation, x, y, z)];
    }

    std::uint32_t fluid3d_turn(const FluidSimulation3D *simulation)
    {
        return simulation ? simulation->turn : 0;
    }

    std::size_t fluid3d_count_non_empty(const FluidSimulation3D *simulation)
    {
        if (!simulation)
            return 0;
        return static_cast<std::size_t>(std::count_if(simulation->cells.begin(), simulation->cells.end(),
                                                      [](const FluidCell &c) { return c.type != FluidElement::empty; }));
    }

    void fluid3d_set_origin(FluidSimulation3D *simulation, int x, int y, int z)
    {
        if (!simulation)
            return;
        simulation->origin_x = x;
        simulation->origin_y = y;
        simulation->origin_z = z;
    }

    void fluid3d_step(FluidSimulation3D *simulation)
    {
        if (!simulation)
            return;
        Sim &s = *simulation;
        ++s.turn;
        s.rng_state = 0x12345678 ^ s.turn;

        // Pick the awake blocks to simulate. Under a budget, rotate the start point so no block starves.
        const int total_blocks = static_cast<int>(s.awake.size());
        std::vector<int> chosen;
        for (int b = 0; b < total_blocks; ++b)
            if (s.awake[b])
                chosen.push_back(b);
        if (s.max_blocks_per_step > 0 && static_cast<int>(chosen.size()) > s.max_blocks_per_step)
        {
            auto first = std::lower_bound(chosen.begin(), chosen.end(), s.block_cursor);
            std::rotate(chosen.begin(), first, chosen.end());
            chosen.resize(static_cast<std::size_t>(s.max_blocks_per_step));
            s.block_cursor = (chosen.back() + 1) % total_blocks;
            std::sort(chosen.begin(), chosen.end());
        }
        for (int b : chosen)
            s.awake[b] = 0;

        auto for_block_cells = [&](int b, auto &&fn)
        {
            const int bx = b % s.blocks_x;
            const int bz = (b / s.blocks_x) % s.blocks_z;
            const int by = b / (s.blocks_x * s.blocks_z);
            const int x0 = std::max(1, bx * BLOCK), x1 = std::min(s.width - 1, (bx + 1) * BLOCK);
            const int y0 = std::max(1, by * BLOCK), y1 = std::min(s.height - 1, (by + 1) * BLOCK);
            const int z0 = std::max(1, bz * BLOCK), z1 = std::min(s.depth - 1, (bz + 1) * BLOCK);
            for (int y = y0; y < y1; ++y)
                for (int z = z0; z < z1; ++z)
                    for (int x = x0; x < x1; ++x)
                        fn(x, y, z);
        };

        for (int b : chosen)
        {
            for_block_cells(b, [&](int x, int y, int z)
                            {
                const FluidCell &cell = at(s, x, y, z);
                if (cell.type != FluidElement::empty && cell.type != FluidElement::solid)
                    update_heat(s, x, y, z); });
        }

        for (int b : chosen)
        {
            for_block_cells(b, [&](int x, int y, int z)
                            {
                FluidCell &cell = at(s, x, y, z);
                if (cell.type == FluidElement::empty || cell.type == FluidElement::solid || cell.turn == s.turn)
                    return;
                cell.turn = s.turn;
                if (always_active(cell.type) || cell.temp > 21.0f)
                    touch(s, x, y, z);
                switch (cell.type)
                {
                case FluidElement::sand:
                case FluidElement::gunpowder:
                    update_sand(s, x, y, z);
                    break;
                case FluidElement::water:
                case FluidElement::oil:
                case FluidElement::acid:
                case FluidElement::lava:
                    update_liquid(s, x, y, z);
                    break;
                case FluidElement::smoke:
                case FluidElement::steam:
                    update_gas(s, x, y, z);
                    break;
                case FluidElement::fire:
                    update_fire(s, x, y, z);
                    break;
                case FluidElement::pump:
                    update_pump(s, x, y, z);
                    break;
                case FluidElement::plant:
                    update_plant(s, x, y, z);
                    break;
                case FluidElement::wood:
                    update_wood(s, x, y, z);
                    break;
                default:
                    break;
                } });
        }
    }

    void fluid3d_register_blocks(const FluidVoxelBlocks &blocks)
    {
        struct Entry
        {
            BlockId id;
            bool solid;
            bool transparent;
            Colour tint;
            std::uint8_t emission;
            Colour light;
        };
        const Entry entries[] = {
            {blocks.water, false, true, Colour{50, 110, 220, 160}, 0, {}},
            {blocks.oil, false, true, Colour{70, 50, 30, 190}, 0, {}},
            {blocks.acid, false, true, Colour{120, 230, 60, 170}, 4, Colour{120, 230, 60, 255}},
            {blocks.lava, false, true, Colour{255, 110, 20, 255}, 14, Colour{255, 110, 20, 255}},
            {blocks.sand, true, false, Colour{222, 200, 130, 255}, 0, {}},
            {blocks.gunpowder, true, false, Colour{60, 60, 66, 255}, 0, {}},
            {blocks.wood, true, false, Colour{140, 95, 50, 255}, 0, {}},
            {blocks.plant, true, false, Colour{60, 170, 60, 255}, 0, {}},
            {blocks.pump, true, false, Colour{170, 170, 190, 255}, 0, {}},
            {blocks.fire, false, true, Colour{255, 170, 40, 200}, 13, Colour{255, 160, 40, 255}},
            {blocks.smoke, false, true, Colour{90, 90, 90, 120}, 0, {}},
            {blocks.steam, false, true, Colour{230, 230, 240, 110}, 0, {}},
            {blocks.solidified, true, false, Colour{70, 62, 64, 255}, 0, {}},
        };
        for (const Entry &e : entries)
        {
            BlockDefinition def;
            def.is_solid = e.solid;
            def.is_transparent = e.transparent;
            def.tint = e.tint;
            def.light_emission = e.emission;
            def.light_colour = e.light;
            register_block_definition(e.id, def);
        }
    }

    BlockId fluid3d_block_for_element(const FluidVoxelBlocks &b, FluidElement element)
    {
        switch (element)
        {
        case FluidElement::water: return b.water;
        case FluidElement::oil: return b.oil;
        case FluidElement::acid: return b.acid;
        case FluidElement::lava: return b.lava;
        case FluidElement::sand: return b.sand;
        case FluidElement::gunpowder: return b.gunpowder;
        case FluidElement::wood: return b.wood;
        case FluidElement::plant: return b.plant;
        case FluidElement::pump: return b.pump;
        case FluidElement::fire: return b.fire;
        case FluidElement::smoke: return b.smoke;
        case FluidElement::steam: return b.steam;
        case FluidElement::solid: return b.solidified;
        default: return BLOCK_AIR;
        }
    }

    void fluid3d_import_from_voxels(FluidSimulation3D *simulation, const VoxelWorld &world, const FluidVoxelBlocks &blocks)
    {
        if (!simulation)
            return;
        Sim &s = *simulation;
        for (int y = 0; y < s.height; ++y)
            for (int z = 0; z < s.depth; ++z)
                for (int x = 0; x < s.width; ++x)
                {
                    const std::size_t i = cell_index(s, x, y, z);
                    const BlockId id = world.get_block(s.origin_x + x, s.origin_y + y, s.origin_z + z);
                    FluidCell cell{};
                    FluidElement element;
                    if (id == BLOCK_AIR)
                        element = FluidElement::empty;
                    else if (!element_from_block(blocks, id, element))
                        element = FluidElement::solid;
                    cell.type = element;
                    cell.temp = default_temp(element);
                    if (element == FluidElement::fire || element == FluidElement::smoke || element == FluidElement::steam)
                        cell.life = 40;
                    s.cells[i] = cell;
                    s.synced_blocks[i] = id;
                }
        s.turn = 0;
        clear_wake_state(s);
    }

    std::size_t fluid3d_export_to_voxels(FluidSimulation3D *simulation, VoxelWorld &world, const FluidVoxelBlocks &blocks)
    {
        if (!simulation)
            return 0;
        Sim &s = *simulation;
        std::size_t changed = 0;
        for (int y = 0; y < s.height; ++y)
            for (int z = 0; z < s.depth; ++z)
                for (int x = 0; x < s.width; ++x)
                {
                    const std::size_t i = cell_index(s, x, y, z);
                    const FluidCell &cell = s.cells[i];
                    BlockId want = fluid3d_block_for_element(blocks, cell.type);
                    FluidElement ignored;
                    // Imported solids keep their original block; only newly created solids use `solidified`.
                    if (cell.type == FluidElement::solid && s.synced_blocks[i] != BLOCK_AIR &&
                        !element_from_block(blocks, s.synced_blocks[i], ignored))
                        want = s.synced_blocks[i];
                    if (want == s.synced_blocks[i])
                        continue;
                    world.set_block(s.origin_x + x, s.origin_y + y, s.origin_z + z, want);
                    s.synced_blocks[i] = want;
                    ++changed;
                }
        return changed;
    }

    void fluid3d_move_region(FluidSimulation3D *simulation, VoxelWorld &world, int origin_x, int origin_y, int origin_z,
                             const FluidVoxelBlocks &blocks)
    {
        if (!simulation)
            return;
        fluid3d_export_to_voxels(simulation, world, blocks);
        fluid3d_set_origin(simulation, origin_x, origin_y, origin_z);
        fluid3d_import_from_voxels(simulation, world, blocks);
    }

} // namespace sl
