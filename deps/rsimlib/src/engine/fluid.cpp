#include "fluid.h"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace sl
{
    namespace
    {
        static inline std::size_t cell_index(const FluidSimulation *simulation, int x, int y)
        {
            return static_cast<std::size_t>(y * simulation->width + x);
        }

        static inline bool is_empty(const FluidCell &cell)
        {
            return cell.type == FluidElement::empty;
        }

        static inline bool is_solid(const FluidCell &cell)
        {
            return cell.type == FluidElement::solid;
        }

        static inline bool is_liquid(const FluidCell &cell)
        {
            switch (cell.type)
            {
            case FluidElement::water:
            case FluidElement::oil:
            case FluidElement::acid:
            case FluidElement::lava:
                return true;
            default:
                return false;
            }
        }

        static inline bool is_gas(const FluidCell &cell)
        {
            switch (cell.type)
            {
            case FluidElement::smoke:
            case FluidElement::steam:
                return true;
            default:
                return false;
            }
        }

        static inline bool can_move_into(const FluidSimulation *simulation, int x, int y)
        {
            if (!simulation || x <= 0 || y <= 0 || x >= simulation->width - 1 || y >= simulation->height - 1)
            {
                return false;
            }
            return simulation->cells[cell_index(simulation, x, y)].type == FluidElement::empty;
        }

        static inline void swap_cells(FluidSimulation *simulation, int x0, int y0, int x1, int y1)
        {
            if (!simulation || !fluid_in_bounds(simulation, x0, y0) || !fluid_in_bounds(simulation, x1, y1))
            {
                return;
            }

            const std::size_t i0 = cell_index(simulation, x0, y0);
            const std::size_t i1 = cell_index(simulation, x1, y1);
            std::swap(simulation->cells[i0], simulation->cells[i1]);
            simulation->cells[i0].turn = simulation->turn;
            simulation->cells[i1].turn = simulation->turn;
        }

        static inline void move_cell(FluidSimulation *simulation, int from_x, int from_y, int to_x, int to_y)
        {
            if (!simulation || !fluid_in_bounds(simulation, from_x, from_y) || !fluid_in_bounds(simulation, to_x, to_y))
            {
                return;
            }

            const std::size_t from_index = cell_index(simulation, from_x, from_y);
            const std::size_t to_index = cell_index(simulation, to_x, to_y);
            simulation->cells[to_index] = simulation->cells[from_index];
            simulation->cells[from_index] = {};
            simulation->cells[to_index].turn = simulation->turn;
        }

        static inline bool can_replace(const FluidCell &a, const FluidCell &b)
        {
            if (a.type == FluidElement::empty)
            {
                return true;
            }
            if (a.type == FluidElement::water && b.type == FluidElement::oil)
            {
                return true;
            }
            if (a.type == FluidElement::oil && b.type == FluidElement::water)
            {
                return false;
            }
            return false;
        }

        static std::uint32_t prng_state = 0x12345678;

        static inline std::uint32_t prng_next()
        {
            prng_state = prng_state * 1103515245U + 12345U;
            return (prng_state / 65536U) % 32768U;
        }

        static void update_heat(FluidSimulation *simulation, int x, int y)
        {
            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];
            if (cell.temp <= 20.0f && cell.type != FluidElement::fire && cell.type != FluidElement::lava)
            {
                return;
            }

            // Thermal conduction with upward convection bias
            const int dxs[] = {0, -1, 1, 0};
            const int dys[] = {-1, 0, 0, 1}; // -1 is UP
            const float weights[] = {0.40f, 0.15f, 0.15f, 0.05f};

            for (int i = 0; i < 4; ++i)
            {
                const int nx = x + dxs[i];
                const int ny = y + dys[i];
                if (nx <= 0 || nx >= simulation->width - 1 || ny <= 0 || ny >= simulation->height - 1)
                {
                    continue;
                }

                FluidCell &neigh = simulation->cells[cell_index(simulation, nx, ny)];
                if (neigh.type == FluidElement::solid)
                {
                    continue;
                }

                const float diff = cell.temp - neigh.temp;
                if (diff > 0.0f)
                {
                    const float transfer = diff * weights[i] * 0.5f;
                    cell.temp -= transfer;
                    neigh.temp += transfer;

                    // Ignition logic: check if neighbor reaches flashpoint
                    if ((neigh.type == FluidElement::wood || neigh.type == FluidElement::plant || neigh.type == FluidElement::oil || neigh.type == FluidElement::gunpowder) && neigh.temp >= 200.0f)
                    {
                        if (neigh.type == FluidElement::gunpowder)
                        {
                            // Trigger explosion: vaporize gunpowder into smoke/heat burst
                            neigh.type = FluidElement::fire;
                            neigh.life = 50;
                            neigh.temp = 800.0f;
                        }
                        else if (neigh.type != FluidElement::fire)
                        {
                            neigh.type = FluidElement::fire;
                            neigh.life = 60;
                            neigh.temp = std::max(neigh.temp, 500.0f);
                        }
                    }
                }
            }

            // Natural cooling
            if (cell.type != FluidElement::fire && cell.type != FluidElement::lava)
            {
                cell.temp += (20.0f - cell.temp) * 0.02f;
            }
        }

        static void update_wood(FluidSimulation *simulation, int x, int y)
        {
            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];
            if (cell.temp > 220.0f && ((prng_next() % 6) == 0))
            {
                cell.type = FluidElement::fire;
                cell.life = 80;
                cell.temp = 500.0f;
            }
        }

        static inline bool can_powder_displace(FluidElement type)
        {
            return type == FluidElement::empty || type == FluidElement::water || type == FluidElement::oil || type == FluidElement::acid ||
                   type == FluidElement::smoke || type == FluidElement::steam || type == FluidElement::fire;
        }

        static inline bool can_powder_pass_through(FluidElement type)
        {
            return type == FluidElement::empty || type == FluidElement::water || type == FluidElement::oil || type == FluidElement::acid ||
                   type == FluidElement::smoke || type == FluidElement::steam || type == FluidElement::fire;
        }

        static void update_sand(FluidSimulation *simulation, int x, int y)
        {
            const int down_y = y + 1;
            if (down_y >= simulation->height - 1)
            {
                return;
            }

            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];
            FluidCell &below = simulation->cells[cell_index(simulation, x, down_y)];

            // Sand falls through liquids and gases
            if (can_powder_displace(below.type))
            {
                swap_cells(simulation, x, y, x, down_y);
                return;
            }

            // Try diagonal displacement with randomness
            const bool try_left_first = ((prng_next() % 2) == 0);
            const int dx1 = try_left_first ? -1 : 1;
            const int dx2 = -dx1;

            if (x + dx1 > 0 && x + dx1 < simulation->width - 1)
            {
                FluidCell &diag1 = simulation->cells[cell_index(simulation, x + dx1, down_y)];
                FluidCell &side1 = simulation->cells[cell_index(simulation, x + dx1, y)];
                if (can_powder_displace(diag1.type) && can_powder_pass_through(side1.type))
                {
                    swap_cells(simulation, x, y, x + dx1, down_y);
                    return;
                }
            }

            if (x + dx2 > 0 && x + dx2 < simulation->width - 1)
            {
                FluidCell &diag2 = simulation->cells[cell_index(simulation, x + dx2, down_y)];
                FluidCell &side2 = simulation->cells[cell_index(simulation, x + dx2, y)];
                if (can_powder_displace(diag2.type) && can_powder_pass_through(side2.type))
                {
                    swap_cells(simulation, x, y, x + dx2, down_y);
                    return;
                }
            }
        }

        static inline bool is_liquid_element(FluidElement type)
        {
            return type == FluidElement::water || type == FluidElement::oil || type == FluidElement::acid || type == FluidElement::lava;
        }

        static inline float get_liquid_density(FluidElement type)
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

        static void update_liquid(FluidSimulation *simulation, int x, int y)
        {
            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];

            // Lava reactions: solidifies with water, ignites materials
            if (cell.type == FluidElement::lava)
            {
                const int ncoords[4][2] = {{x, y + 1}, {x - 1, y}, {x + 1, y}, {x, y - 1}};
                for (const auto &coord : ncoords)
                {
                    const int nx = coord[0], ny = coord[1];
                    if (nx <= 0 || nx >= simulation->width - 1 || ny <= 0 || ny >= simulation->height - 1)
                        continue;
                    FluidCell &n = simulation->cells[cell_index(simulation, nx, ny)];
                    if (n.type == FluidElement::water)
                    {
                        cell.type = FluidElement::solid;
                        cell.temp = 300.0f;
                        n.type = FluidElement::steam;
                        n.life = 60;
                        return;
                    }
                    else if (n.type == FluidElement::wood || n.type == FluidElement::plant || n.type == FluidElement::oil)
                    {
                        n.type = FluidElement::fire;
                        n.life = 70;
                        n.temp = 800.0f;
                    }
                    else if (n.type == FluidElement::gunpowder)
                    {
                        n.type = FluidElement::fire;
                        n.life = 50;
                        n.temp = 900.0f;
                    }
                }
            }

            // Acid chemistry: corrodes materials
            if (cell.type == FluidElement::acid)
            {
                const int ncoords[4][2] = {{x, y + 1}, {x - 1, y}, {x + 1, y}, {x, y - 1}};
                for (const auto &coord : ncoords)
                {
                    const int nx = coord[0], ny = coord[1];
                    if (nx <= 0 || nx >= simulation->width - 1 || ny <= 0 || ny >= simulation->height - 1)
                        continue;
                    FluidCell &n = simulation->cells[cell_index(simulation, nx, ny)];
                    if (n.type == FluidElement::wood || n.type == FluidElement::plant || n.type == FluidElement::sand ||
                        (n.type == FluidElement::solid && nx > 1 && nx < simulation->width - 2 && ny > 1 && ny < simulation->height - 2))
                    {
                        if ((prng_next() % 3) == 0)
                        {
                            n.type = FluidElement::smoke;
                            n.life = 35;
                            cell.type = FluidElement::empty;
                            cell.mass = 0.0f;
                            return;
                        }
                    }
                }
            }

            // Tom Forsyth compressible liquid model with density-based buoyancy
            const int down_y = y + 1;
            if (down_y >= simulation->height - 1)
                return;

            FluidCell &below = simulation->cells[cell_index(simulation, x, down_y)];
            if (below.type == FluidElement::empty)
            {
                swap_cells(simulation, x, y, x, down_y);
                return;
            }
            if (below.type == FluidElement::fire)
            {
                below.type = FluidElement::steam;
                below.life = 40;
                cell.type = FluidElement::empty;
                cell.mass = 0.0f;
                return;
            }

            // Density-based swapping: heavier liquids sink below lighter ones
            if (is_liquid_element(below.type) && get_liquid_density(below.type) < get_liquid_density(cell.type))
            {
                swap_cells(simulation, x, y, x, down_y);
                return;
            }

            // Horizontal spreading with alternating direction
            const bool try_left_first = ((x + y + simulation->turn) % 2 == 0);
            const int dx1 = try_left_first ? -1 : 1;
            const int dx2 = -dx1;

            if (x + dx1 > 0 && x + dx1 < simulation->width - 1 &&
                simulation->cells[cell_index(simulation, x + dx1, down_y)].type == FluidElement::empty &&
                simulation->cells[cell_index(simulation, x + dx1, y)].type == FluidElement::empty)
            {
                swap_cells(simulation, x, y, x + dx1, down_y);
                return;
            }
            if (x + dx2 > 0 && x + dx2 < simulation->width - 1 &&
                simulation->cells[cell_index(simulation, x + dx2, down_y)].type == FluidElement::empty &&
                simulation->cells[cell_index(simulation, x + dx2, y)].type == FluidElement::empty)
            {
                swap_cells(simulation, x, y, x + dx2, down_y);
                return;
            }

            // Lava flows less frequently due to high viscosity
            const int flow_chance = (cell.type == FluidElement::lava) ? 2 : 1;
            if (simulation->turn % flow_chance == 0)
            {
                if (x + dx1 > 0 && x + dx1 < simulation->width - 1 && simulation->cells[cell_index(simulation, x + dx1, y)].type == FluidElement::empty)
                {
                    swap_cells(simulation, x, y, x + dx1, y);
                    return;
                }
                if (x + dx2 > 0 && x + dx2 < simulation->width - 1 && simulation->cells[cell_index(simulation, x + dx2, y)].type == FluidElement::empty)
                {
                    swap_cells(simulation, x, y, x + dx2, y);
                    return;
                }
            }

            // Pressure equalization for water mass
            if (cell.type == FluidElement::water && cell.mass > 0.8f)
            {
                for (int dir : {dx1, dx2})
                {
                    if (x + dir > 0 && x + dir < simulation->width - 1)
                    {
                        FluidCell &side = simulation->cells[cell_index(simulation, x + dir, y)];
                        if (side.type == FluidElement::water && side.mass < cell.mass - 0.05f)
                        {
                            const float flow = (cell.mass - side.mass) * 0.25f;
                            cell.mass -= flow;
                            side.mass += flow;
                            return;
                        }
                    }
                }
            }
        }

        static void update_gas(FluidSimulation *simulation, int x, int y)
        {
            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];
            const int up = y - 1;
            if (up > 0 && simulation->cells[cell_index(simulation, x, up)].type == FluidElement::empty)
            {
                move_cell(simulation, x, y, x, up);
                return;
            }

            const int dir = ((x + simulation->turn) % 2 == 0) ? -1 : 1;
            const int side1 = x + dir;
            const int side2 = x - dir;
            if (side1 > 0 && side1 < simulation->width - 1 && simulation->cells[cell_index(simulation, side1, y)].type == FluidElement::empty)
            {
                move_cell(simulation, x, y, side1, y);
                return;
            }
            if (side2 > 0 && side2 < simulation->width - 1 && simulation->cells[cell_index(simulation, side2, y)].type == FluidElement::empty)
            {
                move_cell(simulation, x, y, side2, y);
                return;
            }

            if (cell.life > 0)
            {
                --cell.life;
                if (cell.life == 0)
                {
                    cell = {};
                }
            }
        }

        static void update_fire(FluidSimulation *simulation, int x, int y)
        {
            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];
            cell.temp = std::max(cell.temp, 650.0f);

            if (cell.life == 0)
            {
                if ((prng_next() % 3) == 0)
                {
                    cell.type = FluidElement::smoke;
                    cell.life = 40;
                }
                else
                {
                    cell.type = FluidElement::empty;
                }
                return;
            }
            --cell.life;

            // Ignite adjacent flammable materials
            const int ncoords[4][2] = {{x, y - 1}, {x - 1, y}, {x + 1, y}, {x, y + 1}};
            for (const auto &coord : ncoords)
            {
                const int nx = coord[0];
                const int ny = coord[1];
                if (nx <= 0 || nx >= simulation->width - 1 || ny <= 0 || ny >= simulation->height - 1)
                {
                    continue;
                }

                FluidCell &n = simulation->cells[cell_index(simulation, nx, ny)];
                if (n.type == FluidElement::oil || n.type == FluidElement::plant)
                {
                    n.type = FluidElement::fire;
                    n.life = 70;
                    n.temp = 800.0f;
                }
                else if (n.type == FluidElement::gunpowder)
                {
                    n.type = FluidElement::fire;
                    n.life = 50;
                    n.temp = 900.0f;
                }
                else if (n.type == FluidElement::wood && (prng_next() % 5) == 0)
                {
                    n.type = FluidElement::fire;
                    n.life = 90;
                    n.temp = 500.0f;
                }
                else if (n.type == FluidElement::water)
                {
                    cell.type = FluidElement::steam;
                    cell.life = 50;
                    cell.temp = 200.0f;
                    return;
                }
            }

            // Fire drifts upward occasionally
            const int up_y = y - 1;
            if (up_y > 0 && (prng_next() % 3) == 0)
            {
                const int drift_x = x + (static_cast<int>(prng_next() % 3) - 1);
                if (drift_x > 0 && drift_x < simulation->width - 1 && simulation->cells[cell_index(simulation, drift_x, up_y)].type == FluidElement::empty)
                {
                    swap_cells(simulation, x, y, drift_x, up_y);
                }
            }
        }

        static void update_pump(FluidSimulation *simulation, int x, int y)
        {
            const int above = y - 1;
            const int below = y + 1;
            if (above <= 0 || below >= simulation->height - 1)
            {
                return;
            }

            FluidCell &dest = simulation->cells[cell_index(simulation, x, above)];
            FluidCell &src = simulation->cells[cell_index(simulation, x, below)];
            if (dest.type == FluidElement::empty && (src.type == FluidElement::water || src.type == FluidElement::oil || src.type == FluidElement::acid || src.type == FluidElement::sand || src.type == FluidElement::gunpowder || src.type == FluidElement::smoke))
            {
                move_cell(simulation, x, below, x, above);
            }
        }

        static void update_plant(FluidSimulation *simulation, int x, int y)
        {
            FluidCell &cell = simulation->cells[cell_index(simulation, x, y)];

            // Look for water in orthogonal neighbors
            const int ncoords[4][2] = {{x, y - 1}, {x - 1, y}, {x + 1, y}, {x, y + 1}};
            for (const auto &coord : ncoords)
            {
                const int nx = coord[0];
                const int ny = coord[1];
                if (nx <= 0 || nx >= simulation->width - 1 || ny <= 0 || ny >= simulation->height - 1)
                {
                    continue;
                }

                FluidCell &n = simulation->cells[cell_index(simulation, nx, ny)];
                if (n.type == FluidElement::water)
                {
                    n.type = FluidElement::empty;

                    // Grow new plant nearby (upward with random horizontal offset)
                    const int offset_x = static_cast<int>(prng_next() % 3) - 1;
                    const int gx = std::max(1, std::min(simulation->width - 2, x + offset_x));
                    const int gy = std::max(1, y - 1);
                    if (gy >= 1 && gy < simulation->height - 1 && gx > 0 && gx < simulation->width - 1)
                    {
                        FluidCell &grow = simulation->cells[cell_index(simulation, gx, gy)];
                        if (grow.type == FluidElement::empty)
                        {
                            grow.type = FluidElement::plant;
                            grow.turn = simulation->turn;
                        }
                    }
                    return;
                }
            }
        }

        static void update_lava(FluidSimulation *simulation, int x, int y)
        {
            // Lava is handled as a special liquid in update_liquid now
            // This is kept as a fallback but the main logic is in update_liquid
            update_liquid(simulation, x, y);
        }
    }

    FluidSimulation *create_fluid_simulation(int width, int height)
    {
        if (width <= 0 || height <= 0)
        {
            return nullptr;
        }

        auto *simulation = new FluidSimulation;
        simulation->width = width;
        simulation->height = height;
        simulation->cells.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
        fluid_clear(simulation);
        return simulation;
    }

    void destroy_fluid_simulation(FluidSimulation *simulation)
    {
        delete simulation;
    }

    void fluid_reset_bounds(FluidSimulation *simulation)
    {
        if (!simulation)
        {
            return;
        }

        for (int y = 0; y < simulation->height; ++y)
        {
            for (int x = 0; x < simulation->width; ++x)
            {
                const bool border = x == 0 || y == 0 || x == simulation->width - 1 || y == simulation->height - 1;
                if (border)
                {
                    simulation->cells[cell_index(simulation, x, y)] = FluidCell{FluidElement::solid};
                }
            }
        }
    }

    void fluid_clear(FluidSimulation *simulation)
    {
        if (!simulation)
        {
            return;
        }

        std::fill(simulation->cells.begin(), simulation->cells.end(), FluidCell{});
        simulation->turn = 0;
        fluid_reset_bounds(simulation);
    }

    bool fluid_in_bounds(const FluidSimulation *simulation, int x, int y)
    {
        return simulation != nullptr && x >= 0 && y >= 0 && x < simulation->width && y < simulation->height;
    }

    bool fluid_set_cell(FluidSimulation *simulation, int x, int y, const FluidCell &cell)
    {
        if (!fluid_in_bounds(simulation, x, y))
        {
            return false;
        }

        simulation->cells[cell_index(simulation, x, y)] = cell;
        return true;
    }

    bool fluid_set_cell(FluidSimulation *simulation, int x, int y, FluidElement type,
                        float mass, float temp, std::uint8_t life, std::int8_t variation)
    {
        FluidCell cell{};
        cell.type = type;
        cell.mass = mass;
        cell.temp = temp;
        cell.life = life;
        cell.variation = variation;
        cell.turn = simulation ? simulation->turn : 0;
        return fluid_set_cell(simulation, x, y, cell);
    }

    FluidCell fluid_get_cell(const FluidSimulation *simulation, int x, int y)
    {
        if (!fluid_in_bounds(simulation, x, y))
        {
            return {};
        }
        return simulation->cells[cell_index(simulation, x, y)];
    }

    int fluid_width(const FluidSimulation *simulation)
    {
        return simulation ? simulation->width : 0;
    }

    int fluid_height(const FluidSimulation *simulation)
    {
        return simulation ? simulation->height : 0;
    }

    std::uint32_t fluid_turn(const FluidSimulation *simulation)
    {
        return simulation ? simulation->turn : 0;
    }

    void fluid_step(FluidSimulation *simulation)
    {
        if (!simulation)
        {
            return;
        }

        if (simulation->width <= 2 || simulation->height <= 2)
        {
            return;
        }

        ++simulation->turn;
        prng_state = 0x12345678 ^ simulation->turn;

        const int width = simulation->width;
        const int height = simulation->height;

        // First pass: thermal conduction and heat propagation
        for (int y = 1; y < height - 1; ++y)
        {
            for (int x = 1; x < width - 1; ++x)
            {
                const std::size_t index = cell_index(simulation, x, y);
                FluidCell &cell = simulation->cells[index];
                if (cell.type != FluidElement::empty && cell.type != FluidElement::solid)
                {
                    update_heat(simulation, x, y);
                }
            }
        }

        // Second pass: element-specific behavior
        for (int y = std::max(1, height - 2); y >= 1; --y)
        {
            for (int x = 1; x < width - 1; ++x)
            {
                const std::size_t index = cell_index(simulation, x, y);
                FluidCell &cell = simulation->cells[index];
                if (cell.type == FluidElement::empty || cell.type == FluidElement::solid || cell.turn == simulation->turn)
                {
                    continue;
                }
                cell.turn = simulation->turn;

                switch (cell.type)
                {
                case FluidElement::sand:
                case FluidElement::gunpowder:
                    update_sand(simulation, x, y);
                    break;
                case FluidElement::water:
                case FluidElement::oil:
                case FluidElement::acid:
                    update_liquid(simulation, x, y);
                    break;
                case FluidElement::smoke:
                case FluidElement::steam:
                    update_gas(simulation, x, y);
                    break;
                case FluidElement::fire:
                    update_fire(simulation, x, y);
                    break;
                case FluidElement::pump:
                    update_pump(simulation, x, y);
                    break;
                case FluidElement::plant:
                    update_plant(simulation, x, y);
                    break;
                case FluidElement::lava:
                    update_lava(simulation, x, y);
                    break;
                case FluidElement::wood:
                    update_wood(simulation, x, y);
                    break;
                default:
                    break;
                }
            }
        }
    }

} // namespace sl
