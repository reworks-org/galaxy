#pragma once

#include <cstdint>
#include <vector>

namespace sl
{
    /** Element kinds understood by the cellular-automata fluid simulation. */
    enum class FluidElement : std::uint8_t
    {
        empty = 0,
        solid,
        wood,
        sand,
        water,
        oil,
        acid,
        lava,
        gunpowder,
        plant,
        pump,
        fire,
        smoke,
        steam
    };

    /** Material and per-cell state stored at one grid coordinate. */
    struct FluidCell
    {
        /** Material occupying this cell. */
        FluidElement type = FluidElement::empty;
        /** Material mass used by movement and pressure calculations. */
        float mass = 0.0f;
        /** Cell temperature in degrees Celsius. */
        float temp = 20.0f;
        /** Remaining lifetime for transient elements such as fire or smoke. */
        std::uint8_t life = 0;
        /** Small per-cell visual or simulation variation. */
        std::int8_t variation = 0;
        /** Tick on which this cell was last processed. */
        std::uint32_t turn = 0;
    };

    /** Fixed-size cellular-automata grid and its simulation tick. */
    struct FluidSimulation
    {
        /** Grid width in cells. */
        int width = 0;
        /** Grid height in cells. */
        int height = 0;
        /** Cells in row-major order, indexed as y * width + x. */
        std::vector<FluidCell> cells;
        /** Number of completed simulation steps. */
        std::uint32_t turn = 0;
    };

    /** Create a fixed-size cellular automata simulation grid. */
    FluidSimulation *create_fluid_simulation(int width = 200, int height = 150);
    /** Destroy a fluid simulation and release all state owned by it. */
    void destroy_fluid_simulation(FluidSimulation *simulation);

    /** Clear all cells to empty and restore the default solid border. */
    void fluid_clear(FluidSimulation *simulation);
    /** Restore the default solid border without clearing remaining cells. */
    void fluid_reset_bounds(FluidSimulation *simulation);
    /** Advance the simulation by one update tick. */
    void fluid_step(FluidSimulation *simulation);

    /** Fill a cell with an explicit fluid state. Returns false when coordinates are out of range. */
    bool fluid_set_cell(FluidSimulation *simulation, int x, int y, const FluidCell &cell);
    /** Convenience overload for setting a cell by element type and simple scalar state. */
    bool fluid_set_cell(FluidSimulation *simulation, int x, int y, FluidElement type,
                        float mass = 0.0f, float temp = 20.0f,
                        std::uint8_t life = 0, std::int8_t variation = 0);
    /** Read the current cell value. Returns an empty/default cell for invalid coordinates. */
    FluidCell fluid_get_cell(const FluidSimulation *simulation, int x, int y);

    /** Return whether a coordinate lies within the active simulation bounds. */
    bool fluid_in_bounds(const FluidSimulation *simulation, int x, int y);
    /** Return the simulation dimensions in cells. */
    int fluid_width(const FluidSimulation *simulation);
    int fluid_height(const FluidSimulation *simulation);
    /** Return the current step index for the simulation. */
    std::uint32_t fluid_turn(const FluidSimulation *simulation);

} // namespace sl
