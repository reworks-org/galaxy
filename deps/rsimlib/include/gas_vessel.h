#pragma once

#include <array>
#include <vector>

namespace sl
{
    /** Gases tracked inside a pressure vessel. */
    enum class VesselGas
    {
        Oxygen = 0,
        Nitrogen = 1,
        CarbonDioxide = 2,
    };
    constexpr int kVesselGasCount = 3;

    /**
     * Cellular gas grid for a pressure vessel: a cube of N x N x N cells (default 12) whose outermost layer is the hull.
     * Gas density is stored per cell per species. Gas mixes by neighbour exchange (no buoyancy, mass conserving) and
     * leaves through vent cells to vacuum. Coordinates are 0..N-1 on each axis; hull cells (index 0 or N-1) hold no gas,
     * except vent cells which sit in the hull layer and drain their neighbours.
     */
    struct GasVessel
    {
        int size = 12;
        /** Diffusion per step as a fraction of the density difference moved per face (stable for <= 1/6). */
        float diffusion = 0.15f;
        /** Cell volume in cubic metres (set by the caller from the vessel box; used for pressure). */
        float cell_volume = 1.0f;
        /** Gas density per cell in kg, indexed [gas][(z * size + y) * size + x]. */
        std::array<std::vector<float>, kVesselGasCount> mass;
        /** 1 for cells open to gas (interior), 0 for hull / obstacles. */
        std::vector<unsigned char> open;
        /** Vent flow rate per cell: fraction of the adjacent gas removed per second. 0 = no vent. */
        std::vector<float> vent;
    };

    GasVessel *create_gas_vessel(int size = 12);
    void destroy_gas_vessel(GasVessel *vessel);

    /** Spread `kg` of a gas evenly over all open cells (adds to what is there). */
    void gas_vessel_fill(GasVessel *vessel, VesselGas gas, float kg);
    /** Add gas at one cell (clamped into the interior). */
    void gas_vessel_inject(GasVessel *vessel, int x, int y, int z, VesselGas gas, float kg);
    /** Remove up to `kg` of a gas from one cell; returns the mass removed. */
    float gas_vessel_consume(GasVessel *vessel, int x, int y, int z, VesselGas gas, float kg);
    /** Open a vent in the hull at a cell. `rate` is the fraction of adjacent gas lost per second (0 closes it). */
    void gas_vessel_set_vent(GasVessel *vessel, int x, int y, int z, float rate);
    /** Mix and vent for `dt` seconds (split into stable sub-steps). Returns the total mass vented. */
    float gas_vessel_step(GasVessel *vessel, float dt);
    /** Total mass of one gas, in kg. */
    float gas_vessel_total(const GasVessel *vessel, VesselGas gas);
    /** Gas mass in one cell. */
    float gas_vessel_cell(const GasVessel *vessel, int x, int y, int z, VesselGas gas);
    /** Number of open (gas-holding) cells. */
    int gas_vessel_open_cells(const GasVessel *vessel);
    /** Rescale a gas so its total becomes `kg`, keeping its distribution (used to sync with lumped totals). */
    void gas_vessel_set_total(GasVessel *vessel, VesselGas gas, float kg);

} // namespace sl
