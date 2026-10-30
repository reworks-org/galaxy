#include "gas_vessel.h"

#include <algorithm>
#include <cmath>

namespace sl
{
    namespace
    {
        int idx(const GasVessel *v, int x, int y, int z) { return (z * v->size + y) * v->size + x; }
        bool in_range(const GasVessel *v, int x, int y, int z)
        {
            return x >= 0 && y >= 0 && z >= 0 && x < v->size && y < v->size && z < v->size;
        }
        int clamp_inner(const GasVessel *v, int a) { return std::clamp(a, 1, v->size - 2); }
    } // namespace

    GasVessel *create_gas_vessel(int size)
    {
        if (size < 3)
            return nullptr;
        auto *v = new GasVessel();
        v->size = size;
        const std::size_t n = static_cast<std::size_t>(size) * size * size;
        for (auto &m : v->mass)
            m.assign(n, 0.0f);
        v->open.assign(n, 0);
        v->vent.assign(n, 0.0f);
        for (int z = 1; z < size - 1; ++z)
            for (int y = 1; y < size - 1; ++y)
                for (int x = 1; x < size - 1; ++x)
                    v->open[idx(v, x, y, z)] = 1;
        return v;
    }

    void destroy_gas_vessel(GasVessel *vessel) { delete vessel; }

    int gas_vessel_open_cells(const GasVessel *v)
    {
        if (!v)
            return 0;
        return static_cast<int>(std::count(v->open.begin(), v->open.end(), static_cast<unsigned char>(1)));
    }

    void gas_vessel_fill(GasVessel *v, VesselGas gas, float kg)
    {
        const int cells = gas_vessel_open_cells(v);
        if (cells == 0)
            return;
        auto &m = v->mass[static_cast<int>(gas)];
        const float each = kg / static_cast<float>(cells);
        for (std::size_t i = 0; i < m.size(); ++i)
            if (v->open[i])
                m[i] += each;
    }

    void gas_vessel_inject(GasVessel *v, int x, int y, int z, VesselGas gas, float kg)
    {
        if (!v || kg <= 0.0f)
            return;
        const int i = idx(v, clamp_inner(v, x), clamp_inner(v, y), clamp_inner(v, z));
        if (v->open[i])
            v->mass[static_cast<int>(gas)][i] += kg;
    }

    float gas_vessel_consume(GasVessel *v, int x, int y, int z, VesselGas gas, float kg)
    {
        if (!v || kg <= 0.0f)
            return 0.0f;
        const int i = idx(v, clamp_inner(v, x), clamp_inner(v, y), clamp_inner(v, z));
        float &m = v->mass[static_cast<int>(gas)][i];
        const float taken = std::min(m, kg);
        m -= taken;
        return taken;
    }

    void gas_vessel_set_vent(GasVessel *v, int x, int y, int z, float rate)
    {
        if (!v || !in_range(v, x, y, z))
            return;
        v->vent[idx(v, x, y, z)] = std::max(0.0f, rate);
    }

    float gas_vessel_step(GasVessel *v, float dt)
    {
        if (!v || dt <= 0.0f)
            return 0.0f;
        const int n = v->size;
        const float d = std::min(v->diffusion, 1.0f / 6.0f);
        const int steps = std::max(1, static_cast<int>(std::ceil(dt / 0.25f)));
        const float sub = dt / static_cast<float>(steps);
        static const int dirs[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
        float vented = 0.0f;

        for (int s = 0; s < steps; ++s)
        {
            for (int g = 0; g < kVesselGasCount; ++g)
            {
                auto &m = v->mass[g];
                // Exchange across each interior face (each pair visited once); symmetric, so mass is conserved.
                for (int z = 1; z < n - 1; ++z)
                    for (int y = 1; y < n - 1; ++y)
                        for (int x = 1; x < n - 1; ++x)
                        {
                            const int a = idx(v, x, y, z);
                            if (!v->open[a])
                                continue;
                            for (const auto &dir : dirs)
                            {
                                const int b = idx(v, x + dir[0], y + dir[1], z + dir[2]);
                                if (!v->open[b])
                                    continue;
                                const float flow = d * (m[a] - m[b]);
                                m[a] -= flow;
                                m[b] += flow;
                            }
                        }
                // Vents sit in hull cells and drain every open neighbour toward vacuum.
                for (int z = 0; z < n; ++z)
                    for (int y = 0; y < n; ++y)
                        for (int x = 0; x < n; ++x)
                        {
                            const float rate = v->vent[idx(v, x, y, z)];
                            if (rate <= 0.0f)
                                continue;
                            const float keep = std::exp(-rate * sub);
                            for (const auto &dir : dirs)
                                for (int sign = -1; sign <= 1; sign += 2)
                                {
                                    const int nx = x + sign * dir[0], ny = y + sign * dir[1], nz = z + sign * dir[2];
                                    if (!in_range(v, nx, ny, nz))
                                        continue;
                                    const int b = idx(v, nx, ny, nz);
                                    if (!v->open[b])
                                        continue;
                                    const float lost = m[b] * (1.0f - keep);
                                    m[b] -= lost;
                                    vented += lost;
                                }
                        }
            }
        }
        return vented;
    }

    float gas_vessel_total(const GasVessel *v, VesselGas gas)
    {
        if (!v)
            return 0.0f;
        double sum = 0.0;
        for (float m : v->mass[static_cast<int>(gas)])
            sum += m;
        return static_cast<float>(sum);
    }

    float gas_vessel_cell(const GasVessel *v, int x, int y, int z, VesselGas gas)
    {
        if (!v || !in_range(v, x, y, z))
            return 0.0f;
        return v->mass[static_cast<int>(gas)][idx(v, x, y, z)];
    }

    void gas_vessel_set_total(GasVessel *v, VesselGas gas, float kg)
    {
        if (!v)
            return;
        const float cur = gas_vessel_total(v, gas);
        if (cur <= 1e-9f)
        {
            gas_vessel_fill(v, gas, kg);
            return;
        }
        const float k = kg / cur;
        for (float &m : v->mass[static_cast<int>(gas)])
            m *= k;
    }

} // namespace sl
