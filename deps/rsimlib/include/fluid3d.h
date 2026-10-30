#pragma once

#include "fluid.h"
#include "voxel.h"

#include <cstdint>
#include <vector>

namespace sl
{
    /**
     * Three-dimensional cellular-automata fluid grid. It reuses FluidElement and FluidCell from fluid.h.
     * X and Z are horizontal and Y is up (matching the voxel world). The outermost cell layer of the grid is
     * read-only context: it is never simulated, and elements never move into it. Place solids there (or let
     * fluid3d_import_solids do it) to contain fluid.
     */
    struct FluidSimulation3D
    {
        /** Grid size in cells along X, Y (up) and Z. */
        int width = 0;
        int height = 0;
        int depth = 0;
        /** World block coordinates of cell (0, 0, 0); used by the VoxelWorld bridge. */
        int origin_x = 0;
        int origin_y = 0;
        int origin_z = 0;
        /** Cells indexed as (y * depth + z) * width + x. */
        std::vector<FluidCell> cells;
        /** Number of completed simulation steps. */
        std::uint32_t turn = 0;
        /** Cap on 8x8x8 blocks simulated per step (0 = unlimited). Skipped blocks stay awake for later steps. */
        int max_blocks_per_step = 0;

        /** Internal: deterministic PRNG state. */
        std::uint32_t rng_state = 0x12345678;
        /** Internal: per-block wake flags (blocks of resting cells are skipped). */
        std::vector<std::uint8_t> awake;
        int blocks_x = 0;
        int blocks_y = 0;
        int blocks_z = 0;
        int block_cursor = 0;
        /** Internal: the voxel block last written or read for each cell, so export only writes changes. */
        std::vector<BlockId> synced_blocks;
    };

    /** Voxel block ids used to represent each fluid element inside a VoxelWorld. */
    struct FluidVoxelBlocks
    {
        BlockId water = 200;
        BlockId oil = 201;
        BlockId acid = 202;
        BlockId lava = 203;
        BlockId sand = 204;
        BlockId gunpowder = 205;
        BlockId wood = 206;
        BlockId plant = 207;
        BlockId pump = 208;
        BlockId fire = 209;
        BlockId smoke = 210;
        BlockId steam = 211;
        /** Block written where a fluid reaction creates solid material (e.g. lava cooled by water). */
        BlockId solidified = 212;
    };

    /** Create a 3D simulation grid. Each dimension must be at least 3. Returns nullptr on invalid sizes. */
    FluidSimulation3D *create_fluid_simulation_3d(int width = 48, int height = 48, int depth = 48);
    void destroy_fluid_simulation_3d(FluidSimulation3D *simulation);

    /** Clear all cells to empty. */
    void fluid3d_clear(FluidSimulation3D *simulation);
    /** Advance the simulation by one tick. */
    void fluid3d_step(FluidSimulation3D *simulation);

    bool fluid3d_in_bounds(const FluidSimulation3D *simulation, int x, int y, int z);
    bool fluid3d_set_cell(FluidSimulation3D *simulation, int x, int y, int z, const FluidCell &cell);
    bool fluid3d_set_cell(FluidSimulation3D *simulation, int x, int y, int z, FluidElement type,
                          float mass = 0.0f, float temp = 20.0f,
                          std::uint8_t life = 0, std::int8_t variation = 0);
    /** Read a cell; out-of-range coordinates return an empty cell. */
    FluidCell fluid3d_get_cell(const FluidSimulation3D *simulation, int x, int y, int z);
    std::uint32_t fluid3d_turn(const FluidSimulation3D *simulation);
    /** Number of cells that are not empty. */
    std::size_t fluid3d_count_non_empty(const FluidSimulation3D *simulation);
    /** Set the world position of the grid's (0, 0, 0) cell. Prefer fluid3d_move_region when a world is attached. */
    void fluid3d_set_origin(FluidSimulation3D *simulation, int x, int y, int z);

    /** Register BlockDefinitions (translucent liquids and gases, opaque powders, emissive lava and fire) for every fluid block. */
    void fluid3d_register_blocks(const FluidVoxelBlocks &blocks = {});
    /** Map an element to its voxel block (BLOCK_AIR for empty, `solidified` for solid). */
    BlockId fluid3d_block_for_element(const FluidVoxelBlocks &blocks, FluidElement element);

    /**
     * Read the world region covered by the grid. Fluid blocks become fluid cells, any other non-air block becomes
     * solid, and air becomes empty. Resets the grid contents and wakes every block.
     */
    void fluid3d_import_from_voxels(FluidSimulation3D *simulation, const VoxelWorld &world,
                                    const FluidVoxelBlocks &blocks = {});
    /**
     * Write cells that changed since the last import/export into the world as fluid blocks (only differences are written,
     * so dirty chunks stay minimal). Returns the number of voxels changed. Call VoxelWorld::update_dirty_chunks afterwards.
     */
    std::size_t fluid3d_export_to_voxels(FluidSimulation3D *simulation, VoxelWorld &world,
                                         const FluidVoxelBlocks &blocks = {});
    /**
     * Export the current region (leaving it frozen in the world), move the grid origin and import the new region.
     * Use this to make the simulated box follow a player or area of interest.
     */
    void fluid3d_move_region(FluidSimulation3D *simulation, VoxelWorld &world, int origin_x, int origin_y, int origin_z,
                             const FluidVoxelBlocks &blocks = {});

} // namespace sl
